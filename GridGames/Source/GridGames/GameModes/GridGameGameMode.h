// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGameTracker.h"
#include "GameFramework/GameModeBase.h"
#include "GameBoard/GameBoard.h"
#include "GridGames/GameStateInterface.h"
#include "GridGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnStart);

class AGamePiece;
class AGridTile;
class AGridGamePlayerController;

UCLASS()
class GRIDGAMES_API AGridGameGameMode : public AGameModeBase, public IGameStateInterface
{
	GENERATED_BODY()	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Grid")
	int GridRows{ 8 };
	UPROPERTY(EditDefaultsOnly, Category = "Grid")
	int GridColumns{ 8 };
	UPROPERTY(EditDefaultsOnly, Category = "Grid")
	int GridLayers{ 1 };

	UPROPERTY(EditDefaultsOnly, Category = "Pieces")
	UDataTable* PiecesSetupData;

	ETurnPhase CurrentTurnPhase;

	UPROPERTY()
	AGameBoard* GameBoard{ nullptr };
	UPROPERTY()
	AGridGamePlayerController* PlayerController{ nullptr };

private:
	UFUNCTION()
	void SetUpPlayers();

	void GameStart();

public:
	void GoToPreTurn() override;
	void PreTurn() override;
	void GoToMainTurn() override;
	void MainTurn() override;
	void GoToPostTurn() override;
	void PostTurn() override;

	UPROPERTY()
	FTurnStart TurnStart;
};
