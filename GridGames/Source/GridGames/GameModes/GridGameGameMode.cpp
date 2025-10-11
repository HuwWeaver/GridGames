// Fill out your copyright notice in the Description page of Project Settings.

#include "GridGameGameMode.h"
#include "GridGameGlobals.h"
#include "GridGames/GamePieces/GamePiece.h"
#include "GridGames/GameBoard/GridTile.h"
#include "GridGames/Player/GridGamePlayerController.h"

#include "Kismet/KismetStringLibrary.h"
#include "Kismet/BlueprintMapLibrary.h"
#include "Kismet/GameplayStatics.h"

// Functions to set up and start the game, such as Begin Play, CreateGrid, and PopulateBoard.
#pragma region Game Start
// Called when the game starts or when spawned
void AGridGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameBoard = Cast<AGameBoard>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameBoard::StaticClass()));
	if (GameBoard == nullptr)
	{
		UE_LOG(LogGridGameError, Error, TEXT("GameBoard is nullptr"));
		return;
	}

	GameBoard->BoardPopulated.AddDynamic(this, &AGridGameGameMode::SetUpPlayers);
	GameBoard->PieceMoved.AddDynamic(this, &AGridGameGameMode::GoToPostTurn);
	GameBoard->CreateGrid(GridRows, GridColumns, GridLayers, PiecesSetupData);
}

void AGridGameGameMode::SetUpPlayers()
{
	PlayerController = Cast<AGridGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr)
	{
		UE_LOG(LogGridGameError, Error, TEXT("PlayerController is nullptr"));
		return;
	}

	PlayerController->Init(GameBoard, this);

	GameStart();
}

void AGridGameGameMode::GameStart()
{
	MainTurn();
}
#pragma endregion

#pragma region PreTurn
void AGridGameGameMode::PreTurn()
{
	CurrentTurnPhase = ETurnPhase::PreTurn;
	TurnStart.Broadcast();

	//TODO: Determine if in Check
	//TODO: Find & Highlight Valid Pieces

	MainTurn();
}
#pragma endregion

// Functions to handle the main actions of a turn, such as Piece Selection, Valid Moved Calculation, Movement, and Piece Deselection.
#pragma region Main Turn
void AGridGameGameMode::MainTurn()
{
	CurrentTurnPhase = ETurnPhase::MainTurn;
	//TODO: Enable Input

	//Now wait for player input...
}
#pragma endregion

#pragma region PostTurn
void AGridGameGameMode::GoToPostTurn()
{
	PostTurn();
}

void AGridGameGameMode::PostTurn()
{
	CurrentTurnPhase = ETurnPhase::PostTurn;

	//TODO: Disable Input

	PreTurn();
}
#pragma endregion