// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRIDGAMES_API IGameStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GoToPreTurn() = 0;
	virtual void PreTurn() = 0;
	virtual void GoToMainTurn() = 0;
	virtual void MainTurn() = 0;
	virtual void GoToPostTurn() = 0;
	virtual void PostTurn() = 0;
};
