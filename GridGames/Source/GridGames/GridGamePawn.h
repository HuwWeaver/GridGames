// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePiece.h"
#include "GridTile.h"
#include "GridGameGameMode.h"
#include "GameFramework/Pawn.h"
#include "GridGamePawn.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class GRIDGAMES_API AGridGamePawn : public APawn
{
	GENERATED_BODY()

	/* MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext{};
	/* Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction{};
	/* Select Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SelectAction{};
	/* Deselect Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DeselectAction{};

public:
	// Sets default values for this pawn's properties
	AGridGamePawn();

	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsWhite{ true };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bGamePieceSelected{ false };
	AGamePiece* SelectedPiece{ nullptr };

private:
	APlayerController* PlayerController{ nullptr };
	AGridGameGameMode* GameMode{ nullptr };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveInput(const FInputActionValue& Value);
	void SelectInput();
	void DeselectInput();

	UFUNCTION()
	void OnTurnStart();
	UFUNCTION()
	void OnPieceMoved();
};
