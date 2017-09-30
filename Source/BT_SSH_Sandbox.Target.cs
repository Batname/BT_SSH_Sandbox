// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BT_SSH_SandboxTarget : TargetRules
{
	public BT_SSH_SandboxTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("BT_SSH_Sandbox");
	}
}
