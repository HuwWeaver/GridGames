// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGameData.h"

class GRIDGAMES_API GridGameTracker
{
public:
	GridGameTracker();
	~GridGameTracker();

	void LogCompletedMove(FMoveOutcome &MoveOutcome);
	char NthLetter(int n);
	char PieceName(FString PieceName);

	FString FullGameLog{};
	int TurnNumber{ 0 };
	bool bWhiteTurn{ true };
};
