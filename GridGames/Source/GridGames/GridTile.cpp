// Fill out your copyright notice in the Description page of Project Settings.

#include "GridTile.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AGridTile::AGridTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(Root);
	TileMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	CoordinateDisplay = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CoordinateDisplay"));
	CoordinateDisplay->SetupAttachment(TileMesh);
	CoordinateDisplay->SetText(FText::FromString("(0,0)"));
	CoordinateDisplay->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));
	CoordinateDisplay->SetRelativeLocation(FVector(-100.0f, 100.0f, 10.0f));
	CoordinateDisplay->SetHorizontalAlignment(EHTA_Center);
	CoordinateDisplay->SetVerticalAlignment(EVRTA_TextCenter);
	CoordinateDisplay->SetWorldSize(50.0f);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(TileMesh);
	CollisionBox->SetRelativeLocation(FVector(-100.0f, 100.0f, 10.0f));
	CollisionBox->SetBoxExtent(FVector(75.0f, 75.0f, 10.0f));
}

// Called when the game starts or when spawned
void AGridTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridTile::Init(const FVector& InCoordinates)
{
	Coordinates = InCoordinates;
	CoordinateDisplay->SetText(FText::FromString(FString::Printf(TEXT("(%i,%i)"), FMath::RoundToInt(Coordinates.X), FMath::RoundToInt(Coordinates.Y))));

	if (FMath::RoundToInt(Coordinates.X + Coordinates.Y) % 2 == 0)
	{
		TileMesh->SetMaterial(0, WhiteMaterial);
	}
	else
	{
		TileMesh->SetMaterial(0, BlackMaterial);
	}
}

