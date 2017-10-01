// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class BG_SSH : ModuleRules
{
	private string ModulePath
	{
	    get { return ModuleDirectory; }
	}

	private string ThirdPartyPath
	{
	    get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
	}

	private string BinariesPath
	{
	    get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/")); }
	}

	private string LibraryPath
	{
	    get { return Path.GetFullPath(Path.Combine(ThirdPartyPath, "LibSSH2","Lib")); }
	}

    public BG_SSH(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"BG_SSH/Public",
                 Path.Combine(ThirdPartyPath, "LibSSH2", "Include"),

				// ... add public include paths required here ...
			}
            );
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"BG_SSH/Private",

				// ... add other private include paths required here ...
			}
            );
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
				"Projects",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "CoreUObject",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		LoadLibSSH2(Target);
	}

	public bool LoadLibSSH2(ReadOnlyTargetRules Target)
	{
	    bool isLibrarySupported = false;

	    if (Target.Platform == UnrealTargetPlatform.Win64)
		{
	    	isLibrarySupported = true;

			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "Win64", "libssh2.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("libssh2.dll");

		}

	    return isLibrarySupported;
	}
}
