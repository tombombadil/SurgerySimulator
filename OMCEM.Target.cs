// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class OMCEMTarget : TargetRules
{
	public OMCEMTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        //bUseUnityBuild = true;
        //bUsePCHFiles = false;
        //MinFilesUsingPrecompiledHeader = 6;
        ExtraModuleNames.AddRange(new string[] { "OMCEM" });

    }

	//
	// TargetRules interface.
	//

	// public override void SetupBinaries(
		// TargetInfo Target,
		// ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		// ref List<string> OutExtraModuleNames
		// )
	// {
		// OutExtraModuleNames.AddRange( new string[] { "OMCEM" } );
	// }
}
