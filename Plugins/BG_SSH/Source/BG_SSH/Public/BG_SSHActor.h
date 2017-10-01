// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/CanvasRenderTarget2D.h"
#include "Engine/Canvas.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BG_SSHActor.generated.h"

UCLASS()
class BG_SSH_API ABG_SSHActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABG_SSHActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BG_SSH")
	class UBG_SSHController* BG_SSHController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BG_SSH")
	class UCanvasRenderTarget2D* CanvasRenderTarget2D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BG_SSH")
	FString TerminalHistory;

	UFUNCTION()
	virtual void OnCanvasRenderTargetUpdate(class UCanvas* Canvas, int32 Width, int32 Height);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
