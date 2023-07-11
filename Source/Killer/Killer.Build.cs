// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Killer : ModuleRules
{
	public Killer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "Paper2D", "UMG", "Niagara", "EnhancedInput",
				"GameplayAbilities", "GameplayTags", "GameplayTasks",
				"OnlineSubsystem", "OnlineServicesNull", "OnlineSubsystemUtils" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		//Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}