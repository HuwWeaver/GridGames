// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGamePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GridGames/Player/GridGamePlayerController.h"
#include "GridGames/GameModes/GridGameGameMode.h"
#include "GridGames/GamePieces/GamePiece.h"
#include "GridGames/GameBoard/GridTile.h"

// Sets default values
AGridGamePawn::AGridGamePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGridGamePawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGridGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridGamePawn::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	PlayerController = Cast<AGridGamePlayerController>(Controller);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void AGridGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGridGamePawn::MoveInput);

		// Selection
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &AGridGamePawn::SelectInput);

		// Deselection
		EnhancedInputComponent->BindAction(DeselectAction, ETriggerEvent::Triggered, this, &AGridGamePawn::DeselectInput);
	}

}

void AGridGamePawn::Init(AGameBoard* InGameBoard, AGridGameGameMode* InGameMode)
{
	GameBoard = InGameBoard;

	GameMode = InGameMode;
	GameMode->TurnEnd.AddDynamic(this, &AGridGamePawn::MovePiece);
}

void AGridGamePawn::MoveInput(const FInputActionValue& Value)
{
}

void AGridGamePawn::SelectInput()
{	
	FHitResult HitResult;

    if (bGamePieceSelected)
    {
        // Find Tile
        if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel2, false, HitResult))
        {
			AGridTile* HitTile = Cast<AGridTile>(HitResult.GetActor());
			if (HitTile)
			{
				CheckMoveValidity(SelectedPiece, HitTile);
			}
        }
    }
    else
    {
		// Find Piece
        if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult))
        {
            SelectedPiece = Cast<AGamePiece>(HitResult.GetActor());
            if (SelectedPiece)
            {
				if (SelectedPiece->GetSetupProperties().bWhite == bIsWhite)
				{
					bGamePieceSelected = true;
					SelectedPiece->PieceSelected();
				}
            }
        }
    }
}

// This function attempts to move a game piece to a target tile, checking if the move is valid based on the valid move destinations and outcomes.
// If the target tile is not in the valid move destinations or outcomes, it logs an error and returns without moving the piece.
// If the move is valid, it retrieves the move outcome and moves the piece(s) accordingly, capturing any pieces that are part of the outcome, before deselecting the piece and logging the completed move.
void AGridGamePawn::CheckMoveValidity(AGamePiece* Piece, AGridTile* TargetTile)
{
	if (!Piece->GetValidMoveDestinations().Contains(TargetTile->GetCoordinates()))
	{
		UE_LOG(LogGridGameError, Error, TEXT("Target Coordinate not contained in Valid Move Destinations"));
		return;
	}

	if (!Piece->GetValidMoveOutcomes().Contains(TargetTile->GetCoordinates()))
	{
		UE_LOG(LogGridGameError, Error, TEXT("Target Coordinate not contained in Valid Move Outcomes"));
		return;
	}

	CurrentMoveOutcome = Piece->GetValidMoveOutcomes().FindRef(TargetTile->GetCoordinates());
	if(CurrentMoveOutcome.MovedPieces.Num() != CurrentMoveOutcome.TargetCoordinates.Num())
	{
		UE_LOG(LogGridGameError, Error, TEXT("MovedPieces and TargetCoordinates count do not match in MoveOutcome"));
		return;
	}

	//Move Valid - Go to Post Turn phase to Execute Move
	GameMode->GoToPostTurn();
}

void AGridGamePawn::MovePiece()
{
	if (!PlayerController || PlayerController->GetIsWhite() != bIsWhite)
	{
		return;
	}

	for (int i = 0; i < CurrentMoveOutcome.MovedPieces.Num(); i++)
	{
		AGamePiece* PieceToMove = CurrentMoveOutcome.MovedPieces[i];
		FVector TargetCoordinate = CurrentMoveOutcome.TargetCoordinates[i];
		PieceToMove->Move(GameBoard->GetGridMap().FindRef(TargetCoordinate), 200);
	}

	for (AGamePiece* PieceToCapture : CurrentMoveOutcome.CapturedPieces)
	{
		PieceToCapture->PieceCaptured();
	}

	GameBoard->PieceMoved(SelectedPiece);

	DeselectInput();
}

void AGridGamePawn::DeselectInput()
{
	SelectedPiece->PieceDeselected();
	bGamePieceSelected = false;
	SelectedPiece = nullptr;
}
