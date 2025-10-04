// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGameCameraActor.h"
#include "Camera/CameraComponent.h"

void AGridGameCameraActor::BeginPlay()
{
	Super::BeginPlay();

	GetCameraComponent()->SetConstraintAspectRatio(false);
}

void AGridGameCameraActor::SwitchPlayer(bool bNewPlayerColour, FVector NewPlayerLoc, FRotator NewPlayerRot)
{
	bIsWhite = bNewPlayerColour;

	SetActorLocationAndRotation(NewPlayerLoc, FRotator(-60, NewPlayerRot.Yaw, NewPlayerRot.Roll));
}
