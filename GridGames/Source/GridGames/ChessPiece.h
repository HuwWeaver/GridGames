// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePiece.h"
#include "ChessPiece.generated.h"

/**
 * 
 */
UCLASS()
class GRIDGAMES_API AChessPiece : public AGamePiece
{
	GENERATED_BODY()
	
private:
	virtual bool CanPromote() override;
	virtual void TriggerPromotion() override;

	UFUNCTION(BlueprintCallable)
	virtual void PromotePiece(const FName& NewPieceName) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnTriggerPromotion();
};
