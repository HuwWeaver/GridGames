// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GridGames/GridGameGlobals.h"
#include "GridGamePawn.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class AGamePiece;
class AGameBoard;
class AGridTile;
class AGridGameGameMode;
class AGridGamePlayerController;

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
	void Init(AGameBoard* InGameBoard, AGridGameGameMode* InGameMode);

	void MoveInput(const FInputActionValue& Value);
	void SelectInput();
	void DeselectInput();

	void CheckMoveValidity(AGamePiece* Piece, AGridTile* TargetTile);
	UFUNCTION()
	void MovePiece();
	UFUNCTION()
	void PromotePiece();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsWhite{ true };

	//No C++ Function Definition, only Blueprint Implementable Event
	UFUNCTION(BlueprintImplementableEvent, Category = "Promotion")
	void ProvidePromotionChoice(AGamePiece* Piece);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bGamePieceSelected{ false };
	UPROPERTY()
	AGamePiece* SelectedPiece{ nullptr };
	FMoveOutcome CurrentMoveOutcome{};

	TArray<AGamePiece*> PromotablePieces;

private:
	UPROPERTY()
	AGridGamePlayerController* PlayerController{ nullptr };
	UPROPERTY()
	AGameBoard* GameBoard{ nullptr };
	UPROPERTY()
	AGridGameGameMode* GameMode{ nullptr };
};
