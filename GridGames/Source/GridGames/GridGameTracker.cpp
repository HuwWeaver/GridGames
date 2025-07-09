// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGameTracker.h"
#include "GamePiece.h"

GridGameTracker::GridGameTracker()
{
}

GridGameTracker::~GridGameTracker()
{
}

void GridGameTracker::LogCompletedMove(FMoveOutcome& MoveOutcome)
{
	FName MovedPiece = MoveOutcome.MovedPieces[0]->GetPieceName();
	FVector TargetCoords = MoveOutcome.TargetCoordinates[0];
	FString ChessNotationCoords = FString::Printf(TEXT("%s%s"), *FString::Chr(NthLetter(1 + FMath::RoundToInt(TargetCoords.X))), *FString::Printf(TEXT("%i"), 1 + FMath::RoundToInt(TargetCoords.Y)));
	bool bPieceCaptured = !MoveOutcome.CapturedPieces.IsEmpty();

	FString MoveLog = FString::Chr(PieceName(MovedPiece.ToString()));
	if (bPieceCaptured) MoveLog.Append("x");
	MoveLog.Append(ChessNotationCoords);

	if (bWhiteTurn) TurnNumber++;

	if(bWhiteTurn) 	FullGameLog.Append(FString::Printf(TEXT("%i.%s "), TurnNumber, *MoveLog));
	else FullGameLog.Append(FString::Printf(TEXT("%s "), *MoveLog));

	bWhiteTurn = !bWhiteTurn;

	//TODO: Remove LogTemp log, implement more robust method
	UE_LOG(LogTemp, Display, TEXT("MoveLog: %s"), *MoveLog);
	UE_LOG(LogTemp, Display, TEXT("FullGameLog: %s"), *FullGameLog);
}

char GridGameTracker::NthLetter(int n)
{
	return "abcdefghijklmnopqrstuvwxyz"[n - 1];
}

char GridGameTracker::PieceName(FString PieceName)
{
	if (PieceName == "King")
	{
		return 'K';
	}
	else if (PieceName == "Queen")
	{
		return 'Q';
	}
	else if (PieceName == "Bishop")
	{
		return 'B';
	}
	else if (PieceName == "Knight")
	{
		return 'N';
	}
	else if (PieceName == "Rook")
	{
		return 'R';
	}
	else if (PieceName == "Pawn")
	{
		return 'P';
	}

	//TODO: Remove LogTemp log, implement more robust method
	UE_LOG(LogTemp, Error, TEXT("Piece Name not Found"));
	return 0;
}
