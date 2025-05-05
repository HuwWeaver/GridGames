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
	
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* CoordinateDisplay;

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;

public:	
	// Sets default values for this actor's properties
	AGridTile();

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
	AGamePiece* OccupyingPiece{ nullptr };

	FVector Coordinates;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//TODO: Send via Pointer / Refernce instead?
	bool GetOccupied() const { return bOccupied; }
	FVector GetCoordinates() const { return Coordinates; };
	
	AGamePiece* GetOccupyingPiece() const { return OccupyingPiece; }

	void Init(const FVector& InCoordinates);
	void ShowValidMove(bool bShow);
};
