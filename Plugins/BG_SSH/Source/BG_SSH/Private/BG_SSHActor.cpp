// Fill out your copyright notice in the Description page of Project Settings.

#include "BG_SSHActor.h"
#include "BG_SSHController.h"
#include "Engine.h"

// Sets default values
ABG_SSHActor::ABG_SSHActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attach SHH menager component
	BG_SSHController = CreateDefaultSubobject<UBG_SSHController>(TEXT("BG_SSHController"));

	TerminalHistory = ">";
}

// Called when the game starts or when spawned
void ABG_SSHActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	CanvasRenderTarget2D = UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(World, UCanvasRenderTarget2D::StaticClass(), 2400, 1600);

	CanvasRenderTarget2D->OnCanvasRenderTargetUpdate.AddDynamic(this, &ABG_SSHActor::OnCanvasRenderTargetUpdate);

	CanvasRenderTarget2D->UpdateResource();

	// Input handle test
	GameViewportClient = GetWorld()->GetGameViewport();

	GameViewportClient->OnGameViewportInputKey().BindLambda([&](FKey Key, FModifierKeysState ModifierKeysState, EInputEvent InputEvent) -> bool
	{
		UE_LOG(LogTemp, Warning, TEXT(">> OnGameViewportInputKey %s"), *Key.ToString());

		GameViewportClient->OnGameViewportInputKey().Unbind();

		return true;
	});
}

void ABG_SSHActor::OnCanvasRenderTargetUpdate(UCanvas * Canvas, int32 Width, int32 Height)
{
	Canvas->DrawText(GEngine->GetMediumFont(), FText::FromString(TerminalHistory), 0.f, 0.f, 10.0f, 10.0f, FFontRenderInfo());
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


	if (GameViewportClient)
	{
		//UE_LOG(LogTemp, Warning, TEXT("GetMousePosition() %s"), *GameViewportClient->GetMousePosition().ToString());
	}
}

