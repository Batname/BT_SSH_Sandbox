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
	// Initialize our implementation
	BG_SSHImpl = new FBG_SSHImpl(Hostname, Username, Password);
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

	ConnectedCode = BG_SSHImpl->Connect();

	BG_SSHImpl->ExecuteCommand();
	
}

void UBG_SSHController::ConnectSSH()
{
}


// Called every frame
void UBG_SSHController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

