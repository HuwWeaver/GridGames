// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePiece.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGamePiece::AGamePiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	RootComponent = PieceMesh;
	PieceMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	PieceMesh->SetSimulatePhysics(false);

	NameDisplay = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PieceName"));
	NameDisplay->SetupAttachment(PieceMesh);
	NameDisplay->SetText(FText::FromString("Name"));
	NameDisplay->SetHorizontalAlignment(EHTA_Center);
	NameDisplay->SetVerticalAlignment(EVRTA_TextCenter);
	NameDisplay->SetRelativeLocation(FVector(0.0f, 0.0f, 55.0f));
	NameDisplay->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));

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

void AGamePiece::PromotePiece()
{
	// Default implementation does nothing, override in derived classes if needed
	UE_LOG(LogTemp, Warning, TEXT("PromotePiece called on %s, but no promotion logic implemented."), *GetName());
}

// Called every frame
void AGamePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGamePiece::Init(const FName& Name, const FPieceSetupProperties& SetupData, const FPieceMovementData& MoveData)
{
	PieceName = Name;
	NameDisplay->SetText(FText::FromName(Name));
	SetupProperties = SetupData;
	CurrentCoordinate = SetupProperties.StartingCoordinates;
	MovementData = MoveData;

	PieceMesh->SetStaticMesh(SetupProperties.PieceMesh);
	PieceMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	PieceMesh->SetMaterial(0, SetupProperties.Material);
	PieceMesh->SetSimulatePhysics(true);

	if (SetupProperties.bWhite)
	{

		NameDisplay->SetTextRenderColor(FColor(0, 0, 0));
	}
	else
	{
		NameDisplay->SetTextRenderColor(FColor(255, 255, 255));
	}
}

void AGamePiece::Move(const AGridTile* TargetTile, const float& TileSize)
{
	FVector TargetLocation = TargetTile->GetActorLocation() + FVector(TileSize / 2, TileSize / 2, 100);
	SetActorLocation(TargetLocation, false, 0, ETeleportType::ResetPhysics);

	PastCoordinates.Add(CurrentCoordinate);
	CurrentCoordinate = TargetTile->GetCoordinates();

	NumMovesMade++;

	if(CanPromote()) PromotePiece();
}

void AGamePiece::PieceCaptured()
{
	Destroy();
}
