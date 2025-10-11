// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GridGamePawn.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class AGamePiece;
class AGameBoard;
class AGridTile;
class AGridGameGameMode;

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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Init(AGameBoard* InGameBoard, AGridGameGameMode* GameMode);

	void MoveInput(const FInputActionValue& Value);
	void SelectInput();
	void DeselectInput();

	void TryMovePiece(AGamePiece* Piece, AGridTile* TargetTile);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsWhite{ true };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bGamePieceSelected{ false };
	AGamePiece* SelectedPiece{ nullptr };

private:
	UPROPERTY()
	APlayerController* PlayerController{ nullptr };
	UPROPERTY()
	AGameBoard* GameBoard{ nullptr };
};
