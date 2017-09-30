// Fill out your copyright notice in the Description page of Project Settings.

#include "BG_SSHActor.h"
#include "BG_SSHController.h"

// Sets default values
ABG_SSHActor::ABG_SSHActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attach SHH menager component
	BG_SSHController = CreateDefaultSubobject<UBG_SSHController>(TEXT("BG_SSHController"));

}

// Called when the game starts or when spawned
void ABG_SSHActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABG_SSHActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BG_SSHController->InitSSH();
}

// Called every frame
void ABG_SSHActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

