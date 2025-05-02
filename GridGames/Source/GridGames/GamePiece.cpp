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

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	PieceMesh->SetupAttachment(Root);
	PieceMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

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

// Called every frame
void AGamePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGamePiece::Init(const FText& Name, const FGameSetup& SetupProperties, const FPieceProperties& MoveData)
{
	NameDisplay->SetText(Name);
	StartingProperties = SetupProperties;
	CurrentCoordinate = StartingProperties.StartingCoordinates;
	MovementData = MoveData;

	if (StartingProperties.bWhite)
	{
		PieceMesh->SetMaterial(0, WhiteMaterial);
		NameDisplay->SetTextRenderColor(FColor(0, 0, 0));
	}
	else
	{
		PieceMesh->SetMaterial(0, BlackMaterial);
		NameDisplay->SetTextRenderColor(FColor(255, 255, 255));
	}
}

void AGamePiece::Move(const AGridTile* TargetTile, const float& TileSize)
{
	FVector TargetLocation = TargetTile->GetActorLocation() + FVector(TileSize / 2, TileSize / 2, 100);
	SetActorLocation(TargetLocation);

	CurrentCoordinate = TargetTile->Coordinates;

	MovesMade++;

	UE_LOG(LogTemp, Display, TEXT("Piece Moved!"));
}
