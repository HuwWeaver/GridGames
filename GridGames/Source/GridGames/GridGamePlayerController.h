// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGameGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GridGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GRIDGAMES_API AGridGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	APawn* WhitePlayerPawn{ nullptr };
	UPROPERTY(BlueprintReadOnly)
	APawn* BlackPlayerPawn{ nullptr };

	UFUNCTION()
	void SwitchPlayer();

private:
	bool bIsWhite{ true };
	AGridGameGameMode* GameMode{ nullptr };
};
