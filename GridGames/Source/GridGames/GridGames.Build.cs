// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GridGames : ModuleRules
{
	public GridGames(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });

        PublicIncludePaths.AddRange(
            new string[] {
            "GridGames",
            //"GridGames/GameBoard",
            //"GridGames/GameModes",
            //"GridGames/GamePieces",
            //"GridGames/Player",
            }
        );
    }
}
