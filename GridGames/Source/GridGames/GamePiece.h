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

	UPROPERTY(BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PieceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTextRenderComponent* NameDisplay;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* WhiteMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* BlackMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector CurrentCoordinate{0,0,0};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int MovesMade{ 0 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameSetup StartingProperties;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPieceProperties MovementData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(const FText& Name, const FGameSetup& SetupProperties, const FPieceProperties& MoveData);

	UFUNCTION(BlueprintCallable)
	void Move(const AGridTile* TargetTile, const float& TileSize);
};
