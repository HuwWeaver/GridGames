// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGameGlobals.h"

class GRIDGAMES_API GridGameTracker
{
public:
	GridGameTracker();
	~GridGameTracker();

	void LogCompletedMove(const FMoveOutcome &MoveOutcome);
	char NthLetter(int n);
	char PieceInitial(FString PieceName);
	void LogPromotion(const FName& PromotedPieceName);

	FString GameLog{};
	FString DisambiguatedGameLog{};
	int TurnNumber{ 0 };
	bool bWhiteTurn{ true };
};
