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

bool AGamePiece::CanPromote()
{
	return false;
}

void AGamePiece::TriggerPromotion()
{
	//Tell Gamemode to Start Promotion Process
	AGridGameGameMode* GameMode = Cast<AGridGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnTriggerPromotion(this);
}

// Called every frame
void AGamePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGamePiece::Init(const FPieceSetupProperties& SetupData, const FPieceMovementData& MoveData)
{
	SetupProperties = SetupData;
	CurrentCoordinate = SetupProperties.StartingCoordinates;
	MovementData = MoveData;

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

	//TODO: New Promototion Logic
	if (CanPromote())
	{
		TriggerPromotion();
	}
}

void AGamePiece::PieceCaptured()
{
	Destroy();
}

void AGamePiece::Promote(const FName& NewPieceName, const FPieceMovementData& NewMoveData)
{
	PieceName = NewPieceName;
	MovementData = NewMoveData;

	//TODO: Update Piece Mesh
}
