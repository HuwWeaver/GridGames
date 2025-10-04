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
	UTextRenderComponent* CoordinateDisplayFront;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* CoordinateDisplayBack;

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;

public:	
	// Sets default values for this actor's properties
	AGridTile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetOccupied() const { return bOccupied; }
	const FVector& GetCoordinates() const { return Coordinates; };
	AGamePiece* GetOccupyingPiece() const { return OccupyingPiece; }

	void Init(const FVector& InCoordinates);
	void ShowValidMove(bool bShow);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	bool bOccupied{ false };
	UPROPERTY(BlueprintReadWrite)
	AGamePiece* OccupyingPiece{ nullptr };

	FVector Coordinates{};
	FString ChessNotationCoordinates{};

private:

	char NthLetter(int n);

	UPROPERTY(EditAnywhere)
	UMaterialInstance* WhiteMaterial{};
	UPROPERTY(EditAnywhere)
	UMaterialInstance* BlackMaterial{};
	UPROPERTY(EditAnywhere)
	UMaterialInstance* LightHighlightMaterial{};
	UPROPERTY(EditAnywhere)
	UMaterialInstance* DarkHighlightMaterial{};

	bool bIsLightTile{};
};
