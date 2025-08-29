// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGameTracker.h"
#include "GamePiece.h"

DEFINE_LOG_CATEGORY(LogChessGameLog);

GridGameTracker::GridGameTracker()
{
}

GridGameTracker::~GridGameTracker()
{
}

void GridGameTracker::LogCompletedMove(const FMoveOutcome& MoveOutcome)
{
	//Get Piece Name
	FName MovedPiece = MoveOutcome.MovedPieces[0]->GetPieceName();
	//Get Target coordinates and convert to chess notation
	FVector TargetCoords = MoveOutcome.TargetCoordinates[0];
	FString TargetCoordsChessNotation = FString::Printf(TEXT("%s%s"), *FString::Chr(NthLetter(1 + FMath::RoundToInt(TargetCoords.X))), *FString::Printf(TEXT("%i"), 1 + FMath::RoundToInt(TargetCoords.Y)));
	//Get if piece was captured
	bool bPieceCaptured = !MoveOutcome.CapturedPieces.IsEmpty();
	//Get Previous coordinates and convert to chess notation
	FVector PreviousCoords = MoveOutcome.MovedPieces[0]->GetPastCoordinates().Last();
	FString PreviousCoordsChessNotation = FString::Printf(TEXT("%s%s"), *FString::Chr(NthLetter(1 + FMath::RoundToInt(PreviousCoords.X))), *FString::Printf(TEXT("%i"), 1 + FMath::RoundToInt(PreviousCoords.Y)));
	
	//Construct Move Log and Disambiguated Move Log
	FString MoveLog = FString::Chr(PieceInitial(MovedPiece.ToString()));
	FString DisambiguatedMoveLog = MoveLog;
	DisambiguatedMoveLog.Append(PreviousCoordsChessNotation);
	if (bPieceCaptured)
	{
		MoveLog.Append("x");
		DisambiguatedMoveLog.Append("x");
	}
	MoveLog.Append(TargetCoordsChessNotation);
	DisambiguatedMoveLog.Append(TargetCoordsChessNotation);

	if (bWhiteTurn) TurnNumber++;

	if (bWhiteTurn)
	{
		GameLog.Append(FString::Printf(TEXT("%i.%s "), TurnNumber, *MoveLog));
		DisambiguatedGameLog.Append(FString::Printf(TEXT("%i.%s "), TurnNumber, *DisambiguatedMoveLog));
	}
	else
	{
		GameLog.Append(FString::Printf(TEXT("%s "), *MoveLog));
		DisambiguatedGameLog.Append(FString::Printf(TEXT("%s "), *DisambiguatedMoveLog));
	}

	bWhiteTurn = !bWhiteTurn;

	UE_LOG(LogChessGameLog, Display, TEXT("MoveLog: %s"), *MoveLog);
	UE_LOG(LogChessGameLog, Display, TEXT("GameLog: %s"), *GameLog);
	UE_LOG(LogChessGameLog, Display, TEXT("DisambiguatedMoveLog: %s"), *DisambiguatedMoveLog);
	UE_LOG(LogChessGameLog, Display, TEXT("DisambiguatedGameLog: %s"), *DisambiguatedGameLog);
}

char GridGameTracker::NthLetter(int n)
{
	return "abcdefghijklmnopqrstuvwxyz"[n - 1];
}

char GridGameTracker::PieceInitial(FString PieceName)
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
		return '\0';
	}

	UE_LOG(LogGridGameError, Error, TEXT("Piece Name not Found"));
	return 0;
}

void GridGameTracker::LogPromotion(const FName& PromotedPieceName)
{
	FString PromotedPieceInitial = FString::Chr(PieceInitial(PromotedPieceName.ToString()));

	//Remove last character - a ' ' used to separate moves
	GameLog = GameLog.LeftChop(1);
	DisambiguatedGameLog = DisambiguatedGameLog.LeftChop(1);

	//Append the promoted piece initial and a ' ' space
	GameLog.Append(FString::Printf(TEXT("%s "), *PromotedPieceInitial));
	DisambiguatedGameLog.Append(FString::Printf(TEXT("%s "), *PromotedPieceInitial));

	UE_LOG(LogChessGameLog, Display, TEXT("GameLog: %s"), *GameLog);
	UE_LOG(LogChessGameLog, Display, TEXT("DisambiguatedGameLog: %s"), *DisambiguatedGameLog);
}
