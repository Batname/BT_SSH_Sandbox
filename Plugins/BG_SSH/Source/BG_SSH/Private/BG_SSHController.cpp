// Fill out your copyright notice in the Description page of Project Settings.

#include "BG_SSHController.h"
#include "BG_SSHImpl.h"
#include "IBG_SSH.h"

// Sets default values for this component's properties
UBG_SSHController::UBG_SSHController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
	bAutoActivate = true;
}

void UBG_SSHController::InitSSH()
{
	// Binding Delegates
	SHHConnectionSuccess.BindUObject(this, &UBG_SSHController::OnSHHConnectionSuccess);
	SHHConnectionFalue.BindUObject(this, &UBG_SSHController::OnSHHConnectionFalue);
	SHHCommandResponseFalue.AddUObject(this, &UBG_SSHController::OnSHHCommandResponseFalue);
	SHHCommandResponseString.AddUObject(this, &UBG_SSHController::OnSHHCommandResponseString);

	// Initialize our implementation
	BG_SSHImpl = new FBG_SSHImpl(Hostname, Username, Password, SHHConnectionSuccess, SHHConnectionFalue, SHHCommandResponseFalue, SHHCommandResponseString);
}

void UBG_SSHController::OnSHHConnectionSuccess(int Code)
{
	UE_LOG(BG_SSH_LOG, Warning, TEXT("UBG_SSHController::OnSHHConnectionSuccess"));
}

void UBG_SSHController::OnSHHConnectionFalue(int Code)
{
	UE_LOG(BG_SSH_LOG, Warning, TEXT("UBG_SSHController::OnSHHConnectionFalue"));
}

void UBG_SSHController::OnSHHCommandResponseFalue(int Code)
{
	UE_LOG(BG_SSH_LOG, Warning, TEXT("UBG_SSHController::OnSHHCommandResponseFalue, Code %d"), Code);
}

void UBG_SSHController::OnSHHCommandResponseString(const FString & Response)
{
	UE_LOG(BG_SSH_LOG, Warning, TEXT("UBG_SSHController::OnSHHCommandResponseString %s"), *Response);
}

UBG_SSHController::~UBG_SSHController()
{
	delete BG_SSHImpl;
}


// Called when the game starts
void UBG_SSHController::BeginPlay()
{
	Super::BeginPlay();

	// ...

	BG_SSHImpl->Connect();
}

void UBG_SSHController::ConnectSSH()
{
}

void UBG_SSHController::RunSSHCommand(const TArray<FString>& Args)
{
	if (!Args.Num())
	{
		// No arguments
		UE_LOG(BG_SSH_LOG, Warning, TEXT("No arguments ERROR"));
		return;
	}

	if (BG_SSHImpl->IsConnected())
	{
		FString SSHCommand = Args[0];
		if (!SSHCommand.IsEmpty())
		{
			// Do something here
			BG_SSHImpl->ExecuteCommand(SSHCommand);
		}
	}
	else
	{
		UE_LOG(BG_SSH_LOG, Warning, TEXT("There is no SSH connection"));
	}
}


// Called every frame
void UBG_SSHController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

