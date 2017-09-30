// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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

/**
 * 
 */
class BG_SSH_API FBG_SSHImpl
{
public:
	FBG_SSHImpl(const FString& Hostname, const FString& Username, const FString& Password);
	~FBG_SSHImpl();

	/** Connect call */
	int Connect();

	/** Check our password */
	bool Authenticate();

	/** Set command for execute */
	bool ExecuteCommand();

	/**  Close connection */
	void Shutdown();


private:
	/************************************************************/
	/* Connection Data											*/
	/************************************************************/

	const char* Hostname;
	const char* Username;
	const char* Password;
	const char* Commandline;

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
};
