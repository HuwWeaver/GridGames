// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGamePlayerController.h"
#include "GridGamePawn.h"
#include "Kismet/GameplayStatics.h"

void AGridGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridGamePawn::StaticClass(), OutActors);

	for (AActor* PlayerPawn : OutActors)
	{
		if (Cast<AGridGamePawn>(PlayerPawn)->bIsWhite)
		{
			WhitePlayerPawn = Cast<AGridGamePawn>(PlayerPawn);
		}
		else
		{
			BlackPlayerPawn = Cast<AGridGamePawn>(PlayerPawn);
		}
	}

	GameMode = Cast<AGridGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->PieceMoved.AddDynamic(this, &AGridGamePlayerController::SwitchPlayer);

	GameCamera = GetWorld()->SpawnActor<AGridGameCameraActor>();
	GameCamera->SwitchPlayer(bIsWhite, WhitePlayerPawn->GetActorLocation(), WhitePlayerPawn->GetActorRotation());
	SetViewTarget(GameCamera);
}

void AGridGamePlayerController::SwitchPlayer()
{
	APawn* NewPlayerPawn = bIsWhite ? BlackPlayerPawn : WhitePlayerPawn;

	Possess(NewPlayerPawn);
	bIsWhite = !bIsWhite;

	SetViewTarget(GameCamera);

	GameCamera->SwitchPlayer(bIsWhite, NewPlayerPawn->GetActorLocation(), NewPlayerPawn->GetActorRotation());
}
