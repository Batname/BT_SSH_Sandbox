// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BG_SSHController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BG_SSH_API UBG_SSHController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBG_SSHController();

	~UBG_SSHController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Connection to SSH */
	UFUNCTION(BlueprintCallable, exec, Category="BG_SSH")
	void ConnectSSH();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BG_SSH")
	FString Hostname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BG_SSH")
	FString Username;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BG_SSH")
	FString Password;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BG_SSH")
	int ConnectedCode;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//  Create new Implemntation of SSH
	void InitSSH();
private:


	class FBG_SSHImpl* BG_SSHImpl;
};
