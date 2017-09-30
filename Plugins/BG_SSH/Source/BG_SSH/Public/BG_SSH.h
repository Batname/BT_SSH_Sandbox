// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IBG_SSH.h"

class FBG_SSHModule : public IBG_SSHModule
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	void*	LibSSH_DLLHandle;
};