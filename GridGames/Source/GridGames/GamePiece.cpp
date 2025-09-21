// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePiece.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GridGameGameMode.h"
#include "GridTile.h"

// Sets default values
AGamePiece::AGamePiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	RootComponent = PieceMesh;
	PieceMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	PieceMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AGamePiece::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGamePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGamePiece::Init(const FPieceSetupProperties& SetupData)
{
	SetupProperties = SetupData;
	CurrentCoordinate = SetupProperties.StartingCoordinates;

	PieceMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	PieceMesh->SetSimulatePhysics(true);
}

void AGamePiece::Move(const AGridTile* TargetTile, const float& TileSize)
{
	FVector TargetLocation = TargetTile->GetActorLocation() + FVector(TileSize / 2, TileSize / 2, 100);
	SetActorLocation(TargetLocation, false, 0, ETeleportType::ResetPhysics);

	PastCoordinates.Add(CurrentCoordinate);
	CurrentCoordinate = TargetTile->GetCoordinates();

	NumMovesMade++;

	CheckPromotion();
}

void AGamePiece::CheckPromotion()
{
}

void AGamePiece::Promote(const TSubclassOf<AGamePiece>& NewPiece)
{
	UE_LOG(LogTemp, Display, TEXT("Piece Promoted"));
}

void AGamePiece::PieceCaptured()
{
	Destroy();
}