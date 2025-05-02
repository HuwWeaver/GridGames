// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridTile.h"
#include "GamePiece.h"
#include "GameFramework/GameModeBase.h"
#include "GridGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GRIDGAMES_API AGridGameGameMode : public AGameModeBase
{
	GENERATED_BODY()	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridRows{ 8 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridColumns{ 8 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridLayers{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize{ 200.0f };

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGridTile> GridTileClass;
	UPROPERTY(BlueprintReadOnly)
	TMap<FVector, AGridTile*> GridMap;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGamePiece> GamePieceClass;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* SetupData;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* PiecesData;

private:
	void CreateGrid();
	void PopulateBoard();
};
