// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"


// Log Categories
DECLARE_LOG_CATEGORY_EXTERN(BG_SSH_LOG, Log, All);

// Declaration of delegates
DECLARE_DELEGATE_OneParam(FSHHConnectionSuccess, int);
DECLARE_DELEGATE_OneParam(FSHHConnectionFalue, int);
DECLARE_MULTICAST_DELEGATE_OneParam(FSHHCommandResponseFalue, int);
DECLARE_MULTICAST_DELEGATE_OneParam(FSHHCommandResponseString, const FString&);


/**
 * The public interface to this module.  In most cases, this interface is only public to sibling modules 
 * within this plugin.
 */
class IBG_SSHModule : public IModuleInterface
{
public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IBG_SSHModule& Get()
	{
		return FModuleManager::LoadModuleChecked< IBG_SSHModule >( "BG_SSHModule" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "BG_SSHModule" );
	}
};