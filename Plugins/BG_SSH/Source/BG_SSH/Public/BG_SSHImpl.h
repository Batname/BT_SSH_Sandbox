// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "IBG_SSH.h"
#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "libssh2.h"


#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
#ifdef HAVE_SYS_SELECT_H
# include <sys/select.h>
#endif
# ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <sys/types.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>


class SSHConnectionTask;
class SSHExecuteCommandTask;

/**
 * 
 */
class BG_SSH_API FBG_SSHImpl
{
	friend SSHConnectionTask;
	friend SSHExecuteCommandTask;

public:
	FBG_SSHImpl(
		const FString& Hostname,
		const FString& Username,
		const FString& Password,
		FSHHConnectionSuccess SHHConnectionSuccess,
		FSHHConnectionFalue SHHConnectionFalue,
		FSHHCommandResponseFalue SHHCommandResponseFalue,
		FSHHCommandResponseString SHHCommandResponseString
	);

	~FBG_SSHImpl();

	/** Connect call */
	void Connect();

	/** Check our password */
	bool Authenticate();

	/** Set command for execute */
	void ExecuteCommand(const FString& Command);

	/**  Close connection */
	void Shutdown();

	/** return status of SSH connection */
	FORCEINLINE bool IsConnected() { return bIsConnected; }

private:
	/************************************************************/
	/* Async Functions											*/
	/************************************************************/

	void ConnectAsync_Internal();
	void ExecuteCommandAsync_Internal(const FString& Command);


	/************************************************************/
	/* Connection Data											*/
	/************************************************************/
	FString Hostname;
	FString Username;
	FString Password;

	unsigned long hostaddr;
	int sock;
	struct sockaddr_in sin;
	const char *fingerprint;
	LIBSSH2_SESSION *session;
	LIBSSH2_CHANNEL *channel;
	int rc;
	int exitcode;
	char *exitsignal = (char *)"none";
	int bytecount = 0;
	size_t len;
	LIBSSH2_KNOWNHOSTS *nh;
	int type;


	/************************************************************/
	/* References to Delegates									*/
	/************************************************************/
	FSHHConnectionSuccess SHHConnectionSuccess;
	FSHHConnectionFalue SHHConnectionFalue;
	FSHHCommandResponseFalue SHHCommandResponseFalue;
	FSHHCommandResponseString SHHCommandResponseString;


	bool bIsConnected;
	bool bIsExecuteCommandNow;

};


/************************************************************/
/* Run connection function in separate thread				*/
/************************************************************/

class SSHConnectionTask : public FNonAbandonableTask
{
	/** Reference to base implementation class */
	FBG_SSHImpl* BG_SSHImpl;

public:
	//Constructor
	SSHConnectionTask(FBG_SSHImpl* BG_SSHImpl)
	{
		this->BG_SSHImpl = BG_SSHImpl;
	}

	/*Function needed by the UE in order to determine what's the tasks' status*/
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UMGAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	/*This function executes each time this thread is active - UE4 searches for a function named DoWord() and executes it*/
	void DoWork()
	{
		BG_SSHImpl->ConnectAsync_Internal();
	}
};

class SSHExecuteCommandTask : public FNonAbandonableTask
{
	/** Reference to base implementation class */
	FBG_SSHImpl* BG_SSHImpl;

	/** Command for Execution */
	FString Command;

public:
	//Constructor
	SSHExecuteCommandTask(FBG_SSHImpl* BG_SSHImpl, const FString& Command)
	{
		this->BG_SSHImpl = BG_SSHImpl;
		this->Command = Command;
	}

	/*Function needed by the UE in order to determine what's the tasks' status*/
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UMGAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	/*This function executes each time this thread is active - UE4 searches for a function named DoWord() and executes it*/
	void DoWork()
	{
		BG_SSHImpl->ExecuteCommandAsync_Internal(Command);
	}
};