// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UTextRenderComponent;
class UBoxComponent;
class AGamePiece;

UCLASS()
class GRIDGAMES_API AGridTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridTile();

	UPROPERTY(BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TileMesh;

	UPROPERTY(BlueprintReadOnly)
	UTextRenderComponent* CoordinateDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UMaterialInstance* WhiteMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* BlackMaterial;

protected:
	UPROPERTY(BlueprintReadWrite)
	bool bOccupied{ false };
	UPROPERTY(BlueprintReadWrite)
	bool bValidMove{ false };

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Coordinates;
	UPROPERTY(BlueprintReadWrite)
	AGamePiece* OccupyingPiece{ nullptr };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(const FVector& InCoordinates);
	bool GetOccupied() const { return bOccupied; }
	AGamePiece* GetOccupyingPiece() const { return OccupyingPiece; }
	void ShowValidMove(bool bShow);
};
