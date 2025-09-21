// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGameGlobals.h"
#include "GameFramework/Actor.h"
#include "GamePiece.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

class AGridTile;

UCLASS()
class GRIDGAMES_API AGamePiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGamePiece();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PieceMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector CurrentCoordinate{0,0,0};
	TArray<FVector> PastCoordinates;
	int NumMovesMade{ 0 };

	//TODO: Remove
	FPieceSetupProperties SetupProperties;

	UPROPERTY(EditAnywhere)
	FPieceMovementData MovementData;
	UPROPERTY(EditAnywhere)
	FName PieceName{ "" };
	UPROPERTY(EditAnywhere)
	FName PieceNameAbbr{ "" };

	UPROPERTY(EditAnywhere)
	bool bPromotable{ false };
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AGamePiece>> PromotionOptions;

	virtual bool CanPromote();
	void TriggerPromotion();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	const FVector& GetCurrentCoordinate() const { return CurrentCoordinate; };
	const TArray<FVector>& GetPastCoordinates() const { return PastCoordinates; };
	const FPieceSetupProperties& GetSetupProperties() const { return SetupProperties; };
	const FPieceMovementData& GetMovementData() const { return MovementData; };
	const FName& GetPieceName() const { return PieceName; };
	int GetNumMovesMade() const { return NumMovesMade; };

	virtual void Init(const FPieceSetupProperties& SetupData, const FPieceMovementData& MoveData);
	void Move(const AGridTile* TargetTile, const float& TileSize);
	void PieceCaptured();
	void Promote(const FName& NewPieceName, const FPieceMovementData& NewMoveData);
};
