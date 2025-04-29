// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGameGameMode.h"
#include "Kismet/GameplayStatics.h"


// Called when the game starts or when spawned
void AGridGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));

	UE_LOG(LogTemp, Warning, TEXT("GridManager: %s"), *GridManager->GetActorNameOrLabel());
}