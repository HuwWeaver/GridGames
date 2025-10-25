// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePiece.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GridGames/GameModes/GridGameGameMode.h"
#include "GridGames/GameBoard/GridTile.h"

// Sets default values
AGamePiece::AGamePiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	RootComponent = PieceMesh;
	PieceMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	PieceMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AGamePiece::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGamePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGamePiece::Init(const FPieceSetupProperties& SetupData, AGameBoard* InGameBoard)
{
	SetupProperties = SetupData;
	CurrentCoordinate = SetupProperties.StartingCoordinates;

	GameBoard = InGameBoard;

	PieceMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	PieceMesh->SetSimulatePhysics(true);
}

// This function is called when a game piece is selected.
// It retrieves the movement data for the selected piece and calculates all valid move destinations based on the piece's movement properties.
// It then shows the valid move tiles on the grid by calling ShowValidMove(true) on each valid tile.
void AGamePiece::PieceSelected()
{
	ValidMoveDestinations.Empty();
	ValidMoveOutcomes.Empty();

	for (FPieceMovementProperties& Move : MovementData.FullMoveList)
	{
		switch (Move.MoveType)
		{
		default:
			break;

		case EMovementTypes::Step:
			StepMove(Move);
			break;

		case EMovementTypes::LimitedRange:
			RangeMove(Move, Move.RangeLimit);
			break;

		case EMovementTypes::BoundlessRange:
			RangeMove(Move);
			break;

		case EMovementTypes::Other:
			OtherMove(Move);
			break;
		}
	}

	for (const FVector& TileCoordinate : ValidMoveDestinations)
	{
		if (GameBoard->GetGridMap().Contains(TileCoordinate))
		{
			GameBoard->GetGridMap().FindRef(TileCoordinate)->ShowValidMove(true);
		}
	}
}

// This function is called when a game piece is deselected.
// It hides all valid move tiles for that game piece by calling ShowValidMove(false) on each valid tile and clears the list of valid move destinations.
void AGamePiece::PieceDeselected()
{
	//ValidMoveDestinations.Empty();
	//ValidMoveOutcomes.Empty();
}

// Step Moves are moves with a single target tile
void AGamePiece::StepMove(const FPieceMovementProperties& Move)
{
	// Get the movement vector and adjust it based on the piece's team color (white or black).
	FVector MovementVector = Move.MovementVector;
	if (!GetSetupProperties().bWhite) MovementVector *= -1;

	// Get the target coordinate by adding the movement vector to the piece's current coordinate.
	FVector TargetCoordinate = GetCurrentCoordinate() + MovementVector;

	// Check if the target coordinate is within the bounds of the grid.
	if (!GameBoard->GetGridMap().Contains(TargetCoordinate))
	{
		//Target Tile is out of bounds = Invalid Move
		return;
	}

	// Get the target tile from the grid map using the target coordinate.
	AGridTile& TargetTile = *GameBoard->GetGridMap().FindRef(TargetCoordinate);

	// Check if the target tile is occupied by another piece.
	if (!TargetTile.GetOccupied())
	{
		//Empty Tile = Valid Move
		TArray<AGamePiece*> MovedPieces;
		MovedPieces.Add(this);
		TArray<FVector> TargetCoordinates;
		TargetCoordinates.Add(TargetCoordinate);

		// Add the target coordinate to the list of valid move destinations and outcomes.
		ValidMoveDestinations.Add(TargetCoordinate);
		ValidMoveOutcomes.Add(TargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates));

		return;
	}

	// If the target tile is occupied, get the piece occupying it.
	AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
	if (OccupyingPiece == nullptr)
	{
		UE_LOG(LogGridGameError, Error, TEXT("OccupyingPiece is nullptr"));
		return;
	}

	// Check if the occupying piece is on the same team as the moving piece.
	if (OccupyingPiece->GetSetupProperties().bWhite == GetSetupProperties().bWhite)
	{
		//Tile Occupied by Same Team = Invalid Move
		return;
	}

	// Check if the move can capture an opposing piece.
	if (Move.bCanCapture)
	{
		//Tile occupied by opposing team && Move can capture = Valid Move
		TArray<AGamePiece*> MovedPieces;
		MovedPieces.Add(this);
		TArray<FVector> TargetCoordinates;
		TargetCoordinates.Add(TargetCoordinate);
		TArray<AGamePiece*> CapturedPieces;
		CapturedPieces.Add(OccupyingPiece);

		// Add the target coordinate to the list of valid move destinations and outcomes.
		ValidMoveDestinations.Add(TargetCoordinate);
		ValidMoveOutcomes.Add(TargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates, CapturedPieces));
		return;
	}

	//Move cannot capture = Invalid Move
	return;
}

// Range Moves are moves with a movement vector that can be applied multiple times, up to a range limit, to reach multiple target tiles.
void AGamePiece::RangeMove(const FPieceMovementProperties& Move, const int& RangeLimit)
{
	// Get the movement vector and adjust it based on the piece's team color (white or black).
	FVector MovementVector = Move.MovementVector;
	if (GetSetupProperties().bWhite) MovementVector *= -1;

	// Get the target coordinate by adding the movement vector to the piece's current coordinate, then reduce the remaining range.
	FVector TargetCoordinate = GetCurrentCoordinate() + MovementVector;
	int RemainingRange = RangeLimit - 1;

	// Check if the target coordinate is within the bounds of the grid.
	while (GameBoard->GetGridMap().Contains(TargetCoordinate))
	{
		// Get the target tile from the grid map using the target coordinate.
		AGridTile& TargetTile = *GameBoard->GetGridMap().FindRef(TargetCoordinate);

		// Check if the target tile is occupied by another piece.
		if (!TargetTile.GetOccupied())
		{
			//Empty Tile = Valid Move
			TArray<AGamePiece*> MovedPieces;
			MovedPieces.Add(this);
			TArray<FVector> TargetCoordinates;
			TargetCoordinates.Add(TargetCoordinate);

			// Add the target coordinate to the list of valid move destinations and outcomes.
			ValidMoveDestinations.Add(TargetCoordinate);
			ValidMoveOutcomes.Add(TargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates));

			// Check if the range limit is -99 (indicating no range limit).
			if (RangeLimit == -99)
			{
				//Move to Next Tile
				TargetCoordinate = TargetCoordinate + MovementVector;
				continue;
			}

			// Check if the remaining range is less than or equal to 0, in which case the range limit has been reached.
			if (RemainingRange <= 0)
			{
				//Range Limit Reached
				return;
			}
			else
			{
				//Range Remaining - Move to Next Tile
				TargetCoordinate = TargetCoordinate + MovementVector;
				RemainingRange--;
				continue;
			}
		}

		// If the target tile is occupied, check the piece occupying it.
		AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
		if (OccupyingPiece == nullptr)
		{
			UE_LOG(LogGridGameError, Error, TEXT("OccupyingPiece is nullptr"));
			return;
		}

		// Check if the occupying piece is on the same team as the moving piece.
		if (OccupyingPiece->GetSetupProperties().bWhite == GetSetupProperties().bWhite)
		{
			//Tile Occupied by Same Team = Invalid Move
			return;
		}

		// Check if the move can capture an opposing piece.
		if (Move.bCanCapture)
		{
			//Tile occupied by opposing team && Move can capture = Valid Move
			TArray<AGamePiece*> MovedPieces;
			MovedPieces.Add(this);
			TArray<FVector> TargetCoordinates;
			TargetCoordinates.Add(TargetCoordinate);
			TArray<AGamePiece*> CapturedPieces;
			CapturedPieces.Add(OccupyingPiece);

			// Add the target coordinate to the list of valid move destinations and outcomes.
			ValidMoveDestinations.Add(TargetCoordinate);
			ValidMoveOutcomes.Add(TargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates, CapturedPieces));
			return;
		}

		//Move cannot capture = Invalid Move
		return;
	}
}

void AGamePiece::Move(const AGridTile* TargetTile, const float& TileSize)
{
	FVector TargetLocation = TargetTile->GetActorLocation() + FVector(TileSize / 2, TileSize / 2, 100);
	SetActorLocation(TargetLocation, false, 0, ETeleportType::ResetPhysics);

	PastCoordinates.Add(CurrentCoordinate);
	CurrentCoordinate = TargetTile->GetCoordinates();

	NumMovesMade++;

	CheckPromotion();
}

void AGamePiece::CheckPromotion()
{
	if (!bPromotable || PromotionOptions.IsEmpty())
	{
		return;
	}
}

void AGamePiece::PieceCaptured()
{
	Destroy();
}