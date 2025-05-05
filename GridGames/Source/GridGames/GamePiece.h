// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridTile.h"
#include "GridGameData.h"
#include "GameFramework/Actor.h"
#include "GamePiece.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class GRIDGAMES_API AGamePiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGamePiece();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PieceMesh;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* NameDisplay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector CurrentCoordinate{0,0,0};
	int MovesMade{ 0 };
	FPieceSetupProperties SetupProperties;
	FPieceMovementData MovementData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//TODO: Send via Pointer / Refernce instead?
	FVector GetCurrentCoordinate() const { return CurrentCoordinate; };
	FPieceSetupProperties GetSetupProperties() const { return SetupProperties; };
	FPieceMovementData GetMovementData() const { return MovementData; };

	void Init(const FText& Name, const FPieceSetupProperties& SetupData, const FPieceMovementData& MoveData);
	void Move(const AGridTile* TargetTile, const float& TileSize);
	void PieceCaptured();
};
