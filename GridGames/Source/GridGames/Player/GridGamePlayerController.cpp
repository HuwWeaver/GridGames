// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGamePlayerController.h"
#include "GridGamePawn.h"
#include "Kismet/GameplayStatics.h"
#include "GridGameCameraActor.h"
#include "GridGames/GameModes/GridGameGameMode.h"

void AGridGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AGridGamePlayerController::Init(AGameBoard* GameBoard, AGridGameGameMode* InGameMode)
{
	bShowMouseCursor = true;

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridGamePawn::StaticClass(), OutActors);

	for (AActor* PlayerPawn : OutActors)
	{
		if (Cast<AGridGamePawn>(PlayerPawn)->bIsWhite)
		{
			WhitePlayerPawn = Cast<AGridGamePawn>(PlayerPawn);
			if( WhitePlayerPawn == nullptr )
			{
				UE_LOG(LogGridGameError, Error, TEXT("WhitePlayerPawn is nullptr"));
				return;
			}

			WhitePlayerPawn->Init(GameBoard, InGameMode);
		}
		else
		{
			BlackPlayerPawn = Cast<AGridGamePawn>(PlayerPawn);
			if ( BlackPlayerPawn == nullptr )
			{
				UE_LOG(LogGridGameError, Error, TEXT("BlackPlayerPawn is nullptr"));
				return;
			}

			BlackPlayerPawn->Init(GameBoard, InGameMode);
		}
	}

	GameMode = InGameMode;
	if (GameMode == nullptr)
	{
		UE_LOG(LogGridGameError, Error, TEXT("GameMode is nullptr"));
		return;
	}

	GameMode->TurnStart.AddDynamic(this, &AGridGamePlayerController::SwitchPlayer);

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
