// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IBG_SSH.h"
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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//  Create new Implemntation of SSH
	void InitSSH();

	/** Connection to SSH */
	void RunSSHCommand(const TArray<FString>& Args);
private:

	void OnSHHConnectionSuccess(int Code);
	void OnSHHConnectionFalue(int Code);
	void OnSHHCommandResponseFalue(int Code);
	void OnSHHCommandResponseString(const FString& Response);


	class FBG_SSHImpl* BG_SSHImpl;


	// Add Delegates to class
	FSHHConnectionSuccess SHHConnectionSuccess;
	FSHHConnectionFalue SHHConnectionFalue;
	FSHHCommandResponseFalue SHHCommandResponseFalue;
	FSHHCommandResponseString SHHCommandResponseString;

public:
	// Declare console command, it is possible to move constructor to head and register as well
	FAutoConsoleCommand RunSSHCommandFromConsole = FAutoConsoleCommand(
		TEXT("ssh.RunSSHCommand"),
		*NSLOCTEXT("BG_SSH", "CommandText_RunSSHCommand", "Run SSH command from game console").ToString(),
		FConsoleCommandWithArgsDelegate::CreateUObject(this, &UBG_SSHController::RunSSHCommand)
	);
};
