// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "GridGameCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class GRIDGAMES_API AGridGameCameraActor : public ACameraActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	UFUNCTION()
	void SwitchPlayer(bool newPlayerColour, FVector newPlayerLoc, FRotator newPlayerRot);

private:
	bool bIsWhite{ true };
};
