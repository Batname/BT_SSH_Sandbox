// Fill out your copyright notice in the Description page of Project Settings.

#include "BG_SSHTable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Engine/Canvas.h"

ABG_SSHTable::ABG_SSHTable()
{ 

	TableMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMeshComponent"));
	RootComponent = TableMeshComponent;

	TerminalMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TerminalMeshComponent"));
	TerminalMeshComponent->SetupAttachment(RootComponent);
	// Add Mesh and Texture
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TerminalWindowMesh(TEXT("StaticMesh'/BG_SSH/Assets/Meshes/Terminal_Window.Terminal_Window'"));
	if (TerminalWindowMesh.Succeeded())
	{
		TerminalMeshComponent->SetStaticMesh(TerminalWindowMesh.Object);
		TerminalMeshComponent->RelativeScale3D = FVector(1.f, 0.2f, 0.15f);
		static ConstructorHelpers::FObjectFinder<UMaterial> TerminalMainMat(TEXT("Material'/BG_SSH/Assets/Materials/TerminalMainMat.TerminalMainMat'"));
		if (TerminalMainMat.Succeeded())
		{
			TerminalMeshComponent->SetMaterial(0, TerminalMainMat.Object);
		}
	}

	BoxInteractionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxInteractionComponent"));
	BoxInteractionComponent->SetupAttachment(RootComponent);
}

void ABG_SSHTable::BeginPlay()
{
	//Get the first material of the static mesh and turn it into a material instance
	DynamicMatInstance = TerminalMeshComponent->CreateAndSetMaterialInstanceDynamic(0);

	Super::BeginPlay();

}

void ABG_SSHTable::OnCanvasRenderTargetUpdate(UCanvas * Canvas, int32 Width, int32 Height)
{
	Super::OnCanvasRenderTargetUpdate(Canvas, Width, Height);

	if (DynamicMatInstance)
	{
		DynamicMatInstance->SetTextureParameterValue(FName("TP2D"), CanvasRenderTarget2D);
	}
}
