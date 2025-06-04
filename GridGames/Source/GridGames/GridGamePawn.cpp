// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGamePawn.h"
#include "GridGameGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGridGamePawn::AGridGamePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(Root);
	CameraComponent->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AGridGamePawn::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AGridGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->PieceMoved.AddDynamic(this, &AGridGamePawn::OnPieceMoved);
}

// Called every frame
void AGridGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridGamePawn::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void AGridGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGridGamePawn::MoveInput);

		// Selection
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &AGridGamePawn::SelectInput);

		// Deselection
		EnhancedInputComponent->BindAction(DeselectAction, ETriggerEvent::Triggered, this, &AGridGamePawn::DeselectInput);
	}

}

void AGridGamePawn::MoveInput(const FInputActionValue& Value)
{
}

void AGridGamePawn::SelectInput()
{	
	FHitResult HitResult;

    if (bGamePieceSelected)
    {
        // Find Tile
        if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel2, false, HitResult))
        {
			AGridTile* HitTile = Cast<AGridTile>(HitResult.GetActor());
			if (HitTile)
			{
				GameMode->TryMovePiece(SelectedPiece, HitTile);
			}
        }
    }
    else
    {
        if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult))
        {
            SelectedPiece = Cast<AGamePiece>(HitResult.GetActor());
            if (SelectedPiece)
            {
				if (SelectedPiece->GetSetupProperties().bWhite == bIsWhite)
				{
					bGamePieceSelected = true;
					GameMode->PieceSelected(SelectedPiece);
				}
            }
        }
    }
}

void AGridGamePawn::DeselectInput()
{
	bGamePieceSelected = false;
	SelectedPiece = nullptr;
	GameMode->PieceDeselected();
}

void AGridGamePawn::OnPieceMoved()
{
	bGamePieceSelected = false;
	SelectedPiece = nullptr;
}
