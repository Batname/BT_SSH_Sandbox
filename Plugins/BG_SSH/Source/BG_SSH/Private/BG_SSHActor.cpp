// Fill out your copyright notice in the Description page of Project Settings.

#include "BG_SSHActor.h"
#include "BG_SSHController.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ABG_SSHActor::ABG_SSHActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attach SHH menager component
	BG_SSHController = CreateDefaultSubobject<UBG_SSHController>(TEXT("BG_SSHController"));

	CleanTerminalTextureSessionStr();

	bIsGameViewportInputBind = false;
	bIsStartTyping = false;
}

void ABG_SSHActor::CleanTerminalTextureSessionStr(const FString& DefaulStr)
{
	TerminalHistory = DefaulStr;
	if (CanvasRenderTarget2D)
	{
		CanvasRenderTarget2D->UpdateResource();
	}
}

// Called when the game starts or when spawned
void ABG_SSHActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	CanvasRenderTarget2D = UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(World, UCanvasRenderTarget2D::StaticClass(), 2400, 1600);

	CanvasRenderTarget2D->OnCanvasRenderTargetUpdate.AddDynamic(this, &ABG_SSHActor::OnCanvasRenderTargetUpdate);

	CanvasRenderTarget2D->UpdateResource();

	// Read player Controller
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	GameModeBase = UGameplayStatics::GetGameMode(GetWorld());


	// Input handle test
	GameViewportClient = GetWorld()->GetGameViewport();
}

void ABG_SSHActor::OnCanvasRenderTargetUpdate(UCanvas * Canvas, int32 Width, int32 Height)
{
	UE_LOG(LogTemp, Warning, TEXT("ABG_SSHActor::OnCanvasRenderTargetUpdate"));

	Canvas->DrawText(GEngine->GetMediumFont(), FText::FromString(TerminalHistory), 0.f, 0.f, 10.0f, 10.0f, FFontRenderInfo());
}

void ABG_SSHActor::BindGameViewportInputKey()
{
	if (GameViewportClient && !bIsGameViewportInputBind)
	{
		UE_LOG(LogTemp, Warning, TEXT("BindGameViewportInputKey"));

		if (PlayerController)
		{
			PlayerController->PlayerInput->FlushPressedKeys();
		}


		GameViewportClient->OnGameViewportInputKey().BindLambda([&](FKey Key, FModifierKeysState ModifierKeysState, EInputEvent InputEvent) -> bool
		{
			OnGameViewportInputKey(Key, ModifierKeysState, InputEvent);
			bIsGameViewportInputBind = true;
			return true;
		});
	}
}

void ABG_SSHActor::UnBindGameViewportInputKey()
{
	if (GameViewportClient && bIsGameViewportInputBind)
	{
		GameViewportClient->OnGameViewportInputKey().Unbind();
		bIsGameViewportInputBind = false;
		bIsStartTyping = false;
	}
}

void ABG_SSHActor::OnGameViewportInputKey(FKey Key, FModifierKeysState ModifierKeysState, EInputEvent InputEvent)
{
	// Do not handle release events
	if (InputEvent == EInputEvent::IE_Released)
	{
		return;
	}

	if (Key.GetDisplayName().EqualTo(FText::FromString("`")))
	{
		if (!bIsStartTyping)
		{
			bIsStartTyping = true;
		}
		return; // Do not print Tilde
	}

	//Escape
	if (Key.GetDisplayName().EqualTo(FText::FromString("Escape")))
	{
		UnBindGameViewportInputKey();

		return; // Do not print Escape
	}

	if (bIsStartTyping)
	{
		UE_LOG(LogTemp, Warning, TEXT(">> GetDisplayName %s"), *Key.GetDisplayName().ToString());

		TerminalHistory = TerminalHistory.Append(Key.GetDisplayName().ToString());

		CanvasRenderTarget2D->UpdateResource();
	}

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

