// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGameTracker.h"
#include "GameFramework/GameModeBase.h"
#include "GameBoard/GameBoard.h"
#include "GridGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnStart);


class AGamePiece;
class AGridTile;
class AGridGamePlayerController;

UCLASS()
class GRIDGAMES_API AGridGameGameMode : public AGameModeBase
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

private:
	UFUNCTION()
	void SetUpPlayers();

	void GameStart();
	void PreTurn();
	void MainTurn();

protected:
	GridGameTracker GameTracker{};
	TArray<FVector> ValidMoveDestinations;
	TMap<FVector, FMoveOutcome> ValidMoveOutcomes;
	AGamePiece* LastMovedPiece{ nullptr };
	ETurnPhase CurrentTurnPhase;

	UPROPERTY()
	AGameBoard* GameBoard{ nullptr };
	UPROPERTY()
	AGridGamePlayerController* PlayerController{ nullptr };

	void PostTurn();

public:
	UFUNCTION()
	void GoToPostTurn();

	UPROPERTY()
	FTurnStart TurnStart;

};
