// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridManager.h"
#include "CoreMinimal.h"
#include "GridTile.h"
#include "GameFramework/GameModeBase.h"
#include "GridGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GRIDGAMES_API AGridGameGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite);
	const AGridManager* GridManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Rows{ 8 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Columns{ 8 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize{ 200.0f };

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGridTile> GridTileClass;

private:
	void CreateGrid();
};
