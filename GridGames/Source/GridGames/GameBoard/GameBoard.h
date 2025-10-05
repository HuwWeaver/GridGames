// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameBoard/GridTile.h"
#include "GameBoard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBoardPopulated);

UCLASS()
class GRIDGAMES_API AGameBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Grid")
	TSubclassOf<AGridTile> GridTileClass;

	TMap<FVector, AGridTile*> GridMap;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CreateGrid(int rows, int columns, int layers, UDataTable* PieceData);

	const TMap<FVector, AGridTile*>& GetGridMap() const { return GridMap; };

	UPROPERTY()
	FBoardPopulated BoardPopulated;

private:
	void PopulateBoard(UDataTable* PieceData);
};
