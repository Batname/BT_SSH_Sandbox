// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
