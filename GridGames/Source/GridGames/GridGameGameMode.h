// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePiece.h"
#include "GameFramework/GameModeBase.h"
#include "GridGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPieceMoved);

UCLASS()
class GRIDGAMES_API AGridGameGameMode : public AGameModeBase
{
	GENERATED_BODY()	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	int GridRows{ 8 };
	UPROPERTY(EditDefaultsOnly)
	int GridColumns{ 8 };
	UPROPERTY(EditDefaultsOnly)
	int GridLayers{ 1 };

	UPROPERTY(EditDefaultsOnly)
	float TileSize{ 200.0f };

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGridTile> GridTileClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGamePiece> GamePieceClass;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* PiecesSetupData;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* PiecesMovementData;

private:
	void CreateGrid();
	void PopulateBoard();

protected:
	TMap<FVector, AGridTile*> GridMap;
	TArray<FVector> ValidMoveDestinations;
	TMap<FVector, FMoveOutcome> ValidMoveOutcomes;
	AGamePiece* LastMovedPiece{ nullptr };

	void StepMove(AGamePiece* Piece, const FPieceMovementProperties& Move);
	void RangeMove(AGamePiece* Piece, const FPieceMovementProperties& Move, const int& RangeLimit = -99);
	virtual void OtherMove(AGamePiece* Piece, const FPieceMovementProperties& Move);

public:
	void TryMovePiece(AGamePiece* Piece, AGridTile* TargetTile);
	void PieceSelected(AGamePiece* Piece);
	void PieceDeselected();
	virtual void OnTriggerPromotion(AGamePiece* Piece);

	AGamePiece* GetLastMovedPiece() const { return LastMovedPiece; }

	UPROPERTY()
	FPieceMoved PieceMoved;
};
