// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GridGamePlayerController.generated.h"

class AGridGameGameMode;
class AGridGameCameraActor;
class AGameBoard;
class AGridGamePawn;

UCLASS()
class GRIDGAMES_API AGridGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	AGridGamePawn* WhitePlayerPawn{ nullptr };
	UPROPERTY(BlueprintReadOnly)
	AGridGamePawn* BlackPlayerPawn{ nullptr };

	UFUNCTION()
	void Init(AGameBoard* GameBoard, AGridGameGameMode* InGameMode);

	bool GetIsWhite() const { return bIsWhite; };

	UFUNCTION()
	void SwitchPlayer();

private:
	bool bIsWhite{ true };

	UPROPERTY()
	AGridGameGameMode* GameMode{ nullptr };
	UPROPERTY()
	AGridGameCameraActor* GameCamera{ nullptr };
};
