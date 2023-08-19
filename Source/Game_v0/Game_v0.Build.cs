// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Game_v0 : ModuleRules
{
	public Game_v0(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "Niagara" });
	}
}
