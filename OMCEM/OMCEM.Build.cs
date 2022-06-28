// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class OMCEM : ModuleRules
{
	public OMCEM(ReadOnlyTargetRules Target):base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;
        //PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        //bEnforceIWYU = true;
        //bEnableExceptions = true;

        //PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Json", "InputCore", "UMG", "JsonUtilities", "HeadMountedDisplay", "GameAnalytics" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Http", "Json", "JsonUtilities", "InputCore", "UMG", "JsonUtilities", "HeadMountedDisplay", "Slate", "SlateCore" });
        //PrivatePCHHeaderFile = "OMCEM.h";


    }
}
