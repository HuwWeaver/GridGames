// Fill out your copyright notice in the Description page of Project Settings.

#include "GridGameGameMode.h"
#include "Kismet/GameplayStatics.h"



// Called when the game starts or when spawned
void AGridGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	CreateGrid();
}

void AGridGameGameMode::CreateGrid()
{
	UE_LOG(LogTemp, Display, TEXT("Creating Grid..."));

	for (size_t Column = 0; Column < Columns; Column++){
		for (size_t Row = 0; Row < Rows; Row++) {

			FVector Location = FVector(Row * TileSize, Column * TileSize, 0.0f);
			FRotator Rotation(0,0,0);
			FActorSpawnParameters SpawnInfo;

			AGridTile* Tile = GetWorld()->SpawnActor<AGridTile>(GridTileClass, Location, Rotation, SpawnInfo);
			Tile->Init(FGridCoordinates(Column, Row));
			//GridMap.Add(FGridCoordinates(Column, Row), Tile);
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Grid Created!"));
}
