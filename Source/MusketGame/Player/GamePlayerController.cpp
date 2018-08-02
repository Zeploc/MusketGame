// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerController.h"

// Local Includes //
#include "MusketGame/UI/PlayerHUDWidget.h"
#include "MusketGame/MusketGameGameModeBase.h"

// Engine Includes //
#include "Engine.h"
#include "Blueprint/UserWidget.h"

void AGamePlayerController::AddKillFeed(AActor * Killer)
{
	FString KillMessage = "[" + FString::FromInt(Killer->GetUniqueID());
	KillMessage += "] killed [" + FString::FromInt(GetUniqueID()) + "]";
	Cast<UPlayerHUDWidget>(HUDInstance)->AddToKillFeed(FText::FromString(KillMessage));
}

// Called when the game starts or when spawned
void AGamePlayerController::BeginPlay()
{
	if (HUDClass)
	{
		HUDInstance = CreateWidget<UUserWidget>(GetWorld(), HUDClass);
		if (HUDInstance) HUDInstance->AddToViewport();
		else GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("HUD INSTANCE not set!"));
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("HUDClass not set!"));

	UIInputType.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
}

void AGamePlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AGamePlayerController::ToggleMenu);
}

// Called every frame
void AGamePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTimer -= DeltaTime;
	AMusketGameGameModeBase* IsAuthGamemode = Cast<AMusketGameGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsAuthGamemode)
		IsAuthGamemode->GameTimer = GameTimer;
}

void AGamePlayerController::ToggleMenu()
{
	if (PauseMenuClass)
	{
		if (!PauseMenuInstance)
		{
			PauseMenuInstance = CreateWidget<UUserWidget>(GetWorld(), PauseMenuClass);
			PauseMenuInstance->AddToViewport();
			SetInputMode(UIInputType);
			bShowMouseCursor = true;
		}
		else
		{
			if (PauseMenuInstance->GetVisibility() == ESlateVisibility::Visible)
			{
				PauseMenuInstance->SetVisibility(ESlateVisibility::Hidden);
				SetInputMode(GameInputType);
				bShowMouseCursor = false;
			}
			else
			{
				PauseMenuInstance->SetVisibility(ESlateVisibility::Visible);
				SetInputMode(UIInputType);
				bShowMouseCursor = true;
			}
		}
	}
}

void AGamePlayerController::StartGame_Implementation(float _GameTimer)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Game Starting!"));
	GameTimer = _GameTimer;
}

void AGamePlayerController::GameComplete_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Game Complete!"));
	SetPause(true);
	EndScreenInstance = CreateWidget<UUserWidget>(GetWorld(), EndScreenClass);
	EndScreenInstance->AddToViewport();
	SetInputMode(UIInputType);
	bShowMouseCursor = true;
}
