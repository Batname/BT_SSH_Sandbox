// Fill out your copyright notice in the Description page of Project Settings.

#include "BG_SSHImpl.h"
#include "IBG_SSH.h"

static int waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
	struct timeval timeout;
	int rc;
	fd_set fd;
	fd_set *writefd = NULL;
	fd_set *readfd = NULL;
	int dir;

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	FD_ZERO(&fd);

	FD_SET(socket_fd, &fd);

	/* now make sure we wait in the correct direction */
	dir = libssh2_session_block_directions(session);

	if (dir & LIBSSH2_SESSION_BLOCK_INBOUND)
		readfd = &fd;

	if (dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
		writefd = &fd;

	rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

	return rc;
}

FBG_SSHImpl::FBG_SSHImpl(const FString& Hostname, const FString& Username, const FString& Password) :
	Hostname(Hostname)
	, Username(Username)
	, Password(Password)
{
}

FBG_SSHImpl::~FBG_SSHImpl()
{
}

int FBG_SSHImpl::Connect()
{
#ifdef WIN32
	WSADATA wsadata;
	int err;

	err = WSAStartup(MAKEWORD(2, 0), &wsadata);

	checkf(err == 0, TEXT("WSAStartup failed with error: %d"), err);
#endif

	rc = libssh2_init(0);
	checkf(rc == 0, TEXT("libssh2 initialization failed (%d)"), rc);


	hostaddr = inet_addr(StringCast<ANSICHAR>(*Hostname).Get());


    /* Ultra basic "connect to port 22 on localhost"
     * Your code is responsible for creating the socket establishing the
     * connection
     */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0) {
		UE_LOG(BG_SSH_LOG, Warning, TEXT(">> failed to socket connect"));
		return -1;
    }

	/* Create a session instance */
	session = libssh2_session_init();
	if (!session)
	{
		return -1;
	}
		

	/* tell libssh2 we want it all done non-blocking */
	libssh2_session_set_blocking(session, 0);

	/* ... start it up. This will trade welcome banners, exchange keys,
	* and setup crypto, compression, and MAC layers
	*/
	while ((rc = libssh2_session_handshake(session, sock)) ==
		LIBSSH2_ERROR_EAGAIN);
	if (rc) {
		UE_LOG(BG_SSH_LOG, Warning, TEXT("Failure establishing SSH session: %d"), rc);
		return -1;
	}

	nh = libssh2_knownhost_init(session);
	if (!nh) {
		/* eeek, do cleanup here */
		return 2;
	}

	/* read all hosts from here */
	libssh2_knownhost_readfile(nh, "known_hosts",
		LIBSSH2_KNOWNHOST_FILE_OPENSSH);

	/* store all known hosts to here */
	libssh2_knownhost_writefile(nh, "dumpfile",
		LIBSSH2_KNOWNHOST_FILE_OPENSSH);

	fingerprint = libssh2_session_hostkey(session, &len, &type);
	if (fingerprint) {
		struct libssh2_knownhost *host;
#if LIBSSH2_VERSION_NUM >= 0x010206
		/* introduced in 1.2.6 */
		int check = libssh2_knownhost_checkp(nh, StringCast<ANSICHAR>(*Hostname).Get(), 22,
			fingerprint, len,
			LIBSSH2_KNOWNHOST_TYPE_PLAIN |
			LIBSSH2_KNOWNHOST_KEYENC_RAW,
			&host);
#else
		/* 1.2.5 or older */
		int check = libssh2_knownhost_check(nh, StringCast<ANSICHAR>(*Hostname).Get(),
			fingerprint, len,
			LIBSSH2_KNOWNHOST_TYPE_PLAIN |
			LIBSSH2_KNOWNHOST_KEYENC_RAW,
			&host);
#endif
		UE_LOG(BG_SSH_LOG, Warning, TEXT("Host check: %d, key: %s"), check, *FString((check <= LIBSSH2_KNOWNHOST_CHECK_MISMATCH) ? host->key : "<none>"));

		/*****
		* At this point, we could verify that 'check' tells us the key is
		* fine or bail out.
		*****/
	}
	else {
		/* eeek, do cleanup here */
		return 3;
	}

	libssh2_knownhost_free(nh);

	if (!Authenticate())
	{
		// Cant make Authentication
		return 4;
	}

#if 0
	libssh2_trace(session, ~0);
#endif

	UE_LOG(BG_SSH_LOG, Warning, TEXT("CONNECTION SUCCESS"));

	return 0;
}

bool FBG_SSHImpl::Authenticate()
{
	if (strlen(StringCast<ANSICHAR>(*Password).Get()) != 0) {
		/* We could authenticate via password */
		while ((rc = libssh2_userauth_password(session, StringCast<ANSICHAR>(*Username).Get(), StringCast<ANSICHAR>(*Password).Get())) ==
			LIBSSH2_ERROR_EAGAIN);
		if (rc) {
			UE_LOG(BG_SSH_LOG, Warning, TEXT("Authentication by password failed. %s here is the pass"), *FString(Password));
			Shutdown();
			return false;
		}
	}
	else {
		/* Or by public key */
		while ((rc = libssh2_userauth_publickey_fromfile(session, StringCast<ANSICHAR>(*Username).Get(),
			"/home/user/"
			".ssh/id_rsa.pub",
			"/home/user/"
			".ssh/id_rsa",
			StringCast<ANSICHAR>(*Password).Get())) ==
			LIBSSH2_ERROR_EAGAIN);
		if (rc) {
			UE_LOG(BG_SSH_LOG, Warning, TEXT("Authentication by public key failed."));
			Shutdown();
			return false;
		}
	}

	return true;
}

bool FBG_SSHImpl::ExecuteCommand(const FString& Command)
{
	/* Exec non-blocking on the remove host */
	while ((channel = libssh2_channel_open_session(session)) == NULL &&
		libssh2_session_last_error(session, NULL, NULL, 0) ==
		LIBSSH2_ERROR_EAGAIN)
	{
		waitsocket(sock, session);
	}
	if (channel == NULL)
	{
		UE_LOG(BG_SSH_LOG, Warning, TEXT("channel == NULL"));
		Shutdown();
		return false;
	}
	while ((rc = libssh2_channel_exec(channel, StringCast<ANSICHAR>(*Command).Get())) ==
		LIBSSH2_ERROR_EAGAIN)
	{
		waitsocket(sock, session);
	}
	if (rc != 0)
	{
		UE_LOG(BG_SSH_LOG, Warning, TEXT("rc != 0"));
		Shutdown();
		return false;
	}

	for (;; )
	{
		/* loop until we block */
		int rc;
		do
		{
			char buffer[0x4000];
			rc = libssh2_channel_read(channel, buffer, sizeof(buffer));
			if (rc > 0)
			{
				int i;
				bytecount += rc;
				UE_LOG(BG_SSH_LOG, Warning, TEXT("We read:"));
				for (i = 0; i < rc; ++i)
				{
					fputc(buffer[i], stderr);
				}

				UE_LOG(BG_SSH_LOG, Warning, TEXT("%s"), *FString(buffer));
			}
			else {
				if (rc != LIBSSH2_ERROR_EAGAIN)
				{
					/* no need to output this for the EAGAIN case */
					UE_LOG(BG_SSH_LOG, Warning, TEXT("libssh2_channel_read returned %d"), rc);
				}
			}
		} while (rc > 0);

		/* this is due to blocking that would occur otherwise so we loop on
		this condition */
		if (rc == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sock, session);
		}
		else
		{
			break;
		}
	}
	exitcode = 127;
	while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
	{
		waitsocket(sock, session);
	}

	if (rc == 0)
	{
		exitcode = libssh2_channel_get_exit_status(channel);
		libssh2_channel_get_exit_signal(channel, &exitsignal,
			NULL, NULL, NULL, NULL, NULL);
	}

	if (exitsignal)
	{
		UE_LOG(BG_SSH_LOG, Warning, TEXT("Got signal: %s"), *FString(exitsignal));

	}
	else
	{
		UE_LOG(BG_SSH_LOG, Warning, TEXT("EXIT: %d bytecount: %d"), exitcode, bytecount);

	}

	libssh2_channel_free(channel);
	channel = NULL;

	Shutdown();

	UE_LOG(BG_SSH_LOG, Warning, TEXT("FINISH COMMAND EXECUTION"));

	return false;
}

void FBG_SSHImpl::Shutdown()
{
	libssh2_session_disconnect(session,
		"Normal Shutdown, Thank you for playing");
	libssh2_session_free(session);

#ifdef WIN32
	closesocket(sock);
#else
	close(sock);
#endif
	UE_LOG(BG_SSH_LOG, Warning, TEXT("Shutdown SSH"));

	libssh2_exit();
}
