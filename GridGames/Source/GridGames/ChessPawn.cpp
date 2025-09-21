// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPawn.h"

void AChessPawn::CheckPromotion()
{
	if (bPromotable && !PromotionOptions.IsEmpty())
	{
		if (SetupProperties.StartingCoordinates.Y - CurrentCoordinate.Y == 6 ||
			SetupProperties.StartingCoordinates.Y - CurrentCoordinate.Y == -6)
		{
			//Can promote, provide options to player
			UE_LOG(LogTemp, Display, TEXT("Providing Promotion Choice..."));
			ProvidePromotionChoice(this);
		}	
	}
}
