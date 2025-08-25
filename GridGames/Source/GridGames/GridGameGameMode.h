// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePiece.h"
#include "GridGameTracker.h"
#include "GameFramework/GameModeBase.h"
#include "GridGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPieceMoved);

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

	UPROPERTY(EditDefaultsOnly, Category = "Grid")
	float TileSize{ 200.0f };

	UPROPERTY(EditDefaultsOnly, Category = "Grid")
	TSubclassOf<AGridTile> GridTileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pieces")
	TSubclassOf<AGamePiece> GamePieceClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pieces")
	UDataTable* PiecesSetupData;

	UPROPERTY(EditDefaultsOnly, Category = "Pieces")
	UDataTable* PiecesMovementData;

private:
	void CreateGrid();
	void PopulateBoard();

	void GameStart();
	void PreTurn();
	void MainTurn();
	void PostTurn();

protected:
	GridGameTracker GameTracker{};
	TMap<FVector, AGridTile*> GridMap;
	TArray<FVector> ValidMoveDestinations;
	TMap<FVector, FMoveOutcome> ValidMoveOutcomes;
	AGamePiece* LastMovedPiece{ nullptr };
	ETurnPhase CurrentTurnPhase;

	void StepMove(AGamePiece* Piece, const FPieceMovementProperties& Move);
	void RangeMove(AGamePiece* Piece, const FPieceMovementProperties& Move, const int& RangeLimit = -99);
	// This function is for other types of moves that may be implemented in derived classes - usually for special moves like castling or en passant in chess.
	virtual void OtherMove(AGamePiece* Piece, const FPieceMovementProperties& Move) PURE_VIRTUAL(AGridGameGameMode::OtherMove,);

public:
	void TryMovePiece(AGamePiece* Piece, AGridTile* TargetTile);
	void PieceSelected(AGamePiece* Piece);
	void PieceDeselected();

	void GoToPostTurn();

	// This function is for derived classes to implement specific promotion logic, such as providing a choice of piece to promote to.
	virtual void OnTriggerPromotion(AGamePiece* Piece) PURE_VIRTUAL(AGridGameGameMode::OnTriggerPromotion, );

	AGamePiece* GetLastMovedPiece() const { return LastMovedPiece; }

	UPROPERTY()
	FTurnStart TurnStart;
	UPROPERTY()
	FPieceMoved PieceMoved;
};
