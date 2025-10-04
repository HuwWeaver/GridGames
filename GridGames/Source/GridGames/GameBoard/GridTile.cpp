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

	CoordinateDisplayFront = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CoordinateDisplayFront"));
	CoordinateDisplayFront->SetupAttachment(TileMesh);
	CoordinateDisplayFront->SetText(FText::FromString("X1"));
	CoordinateDisplayFront->SetRelativeRotation(FRotator(90.0f, 0.0f, 90.0f));
	CoordinateDisplayFront->SetRelativeLocation(FVector(-100.0f, 25.0f, 10.0f));
	CoordinateDisplayFront->SetHorizontalAlignment(EHTA_Center);
	CoordinateDisplayFront->SetVerticalAlignment(EVRTA_TextCenter);
	CoordinateDisplayFront->SetWorldSize(25.0f);

	CoordinateDisplayBack = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CoordinateDisplayBack"));
	CoordinateDisplayBack->SetupAttachment(TileMesh);
	CoordinateDisplayBack->SetText(FText::FromString("X1"));
	CoordinateDisplayBack->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));
	CoordinateDisplayBack->SetRelativeLocation(FVector(-100.0f, 175.0f, 10.0f));
	CoordinateDisplayBack->SetHorizontalAlignment(EHTA_Center);
	CoordinateDisplayBack->SetVerticalAlignment(EVRTA_TextCenter);
	CoordinateDisplayBack->SetWorldSize(25.0f);

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

char AGridTile::NthLetter(int n)
{
	return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[n - 1];
}

// Called every frame
void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridTile::Init(const FVector& InCoordinates)
{
	Coordinates = InCoordinates;
	ChessNotationCoordinates = FString::Printf(TEXT("%s%s"), *FString::Chr(NthLetter(1 + FMath::RoundToInt(Coordinates.X))), *FString::Printf(TEXT("%i"), 1 + FMath::RoundToInt(Coordinates.Y)));

	CoordinateDisplayFront->SetText(FText::FromString(ChessNotationCoordinates));
	CoordinateDisplayBack->SetText(FText::FromString(ChessNotationCoordinates));

	if (FMath::RoundToInt(Coordinates.X + Coordinates.Y) % 2 == 0)
	{
		bIsLightTile = true;
		TileMesh->SetMaterial(0, WhiteMaterial);
	}
	else
	{
		bIsLightTile = false;
		TileMesh->SetMaterial(0, BlackMaterial);
	}
}

void AGridTile::ShowValidMove(bool bShow)
{
	if (bShow)
	{
		if (bIsLightTile)
		{
			TileMesh->SetMaterial(0, LightHighlightMaterial);
		}
		else
		{
			TileMesh->SetMaterial(0, DarkHighlightMaterial);
		}
	}
	else
	{
		if (bIsLightTile)
		{
			TileMesh->SetMaterial(0, WhiteMaterial);
		}
		else
		{
			TileMesh->SetMaterial(0, BlackMaterial);
		}
	}
}



