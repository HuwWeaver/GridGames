// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGameTracker.h"
#include "GameFramework/GameModeBase.h"
#include "GameBoard/GameBoard.h"
#include "GridGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPieceMoved);

class AGamePiece;
class AGridTile;

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
	void GameStart();
	void PreTurn();
	void MainTurn();

protected:
	GridGameTracker GameTracker{};
	TArray<FVector> ValidMoveDestinations;
	TMap<FVector, FMoveOutcome> ValidMoveOutcomes;
	AGamePiece* LastMovedPiece{ nullptr };
	ETurnPhase CurrentTurnPhase;
	AGameBoard* GameBoard{ nullptr };

	void StepMove(AGamePiece* Piece, const FPieceMovementProperties& Move);
	void RangeMove(AGamePiece* Piece, const FPieceMovementProperties& Move, const int& RangeLimit = -99);
	// This function is for other types of moves that may be implemented in derived classes - usually for special moves like castling or en passant in chess.
	virtual void OtherMove(AGamePiece* Piece, const FPieceMovementProperties& Move) PURE_VIRTUAL(AGridGameGameMode::OtherMove);

	void PostTurn();

public:
	void TryMovePiece(AGamePiece* Piece, AGridTile* TargetTile);
	void PieceSelected(AGamePiece* Piece);
	void PieceDeselected();

	void GoToPostTurn();

	UPROPERTY()
	FTurnStart TurnStart;
	UPROPERTY()
	FPieceMoved PieceMoved;
};
