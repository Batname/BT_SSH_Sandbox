// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BG_SSH.h"
#include "Core.h"
#include "ModuleManager.h"
#include "IPluginManager.h"

DEFINE_LOG_CATEGORY(BG_SSH_LOG);

#define LOCTEXT_NAMESPACE "FBG_SSHModule"

void FBG_SSHModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("BG_SSH")->GetBaseDir();

	// Load SSH lib dll
	FString LibSHHPath;
#if PLATFORM_WINDOWS
	LibSHHPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/LibSSH2/Win64/libssh2.dll"));
#endif // PLATFORM_WINDOWS

	LibSSH_DLLHandle = !LibSHHPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibSHHPath) : nullptr;

	if (LibSSH_DLLHandle)
	{
		// Do nothing now
		UE_LOG(BG_SSH_LOG, Warning, TEXT(">> LibSSH_DLLHandle Loaded"));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("LibSSH_DLLHandle", "Failed to load example third party library"));
	}
}

void FBG_SSHModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(LibSSH_DLLHandle);
	LibSSH_DLLHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBG_SSHModule, BG_SSH)