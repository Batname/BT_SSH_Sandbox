// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BT_SSH_SandboxEditorTarget : TargetRules
{
	public BT_SSH_SandboxEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("BT_SSH_Sandbox");
	}
}
