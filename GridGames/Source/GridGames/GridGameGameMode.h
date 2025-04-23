// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridManager.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GridGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GRIDGAMES_API AGridGameGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly);
	const AActor* GridManager;
};
