// Fill out your copyright notice in the Description page of Project Settings.

#include "MusketGameGameModeBase.h"

#include "MusketGame/NetworkSession/NWGameInstance.h"
#include "MusketGame/Player/GamePlayerController.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void AMusketGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, TEXT("Using Musket Game Mode"));
	if (WinConditionTimer)
		GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &AMusketGameGameModeBase::ServerGameComplete, GameTimer, false);

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Game Starting from Server!"));
	/*TArray<AActor*> PlayersArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGamePlayerController::StaticClass(), PlayersArray);
	for (int i = 0; i < PlayersArray.Num(); i++)
	{
		Cast<AGamePlayerController>(PlayersArray[i])->StartGame(GameTimer);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Calling controller Game Starting on " + FString::FromInt(i)));
	}*/
	Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController())->StartGame(GameTimer);
}


void AMusketGameGameModeBase::ServerGameComplete()
{
	Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController())->GameComplete();
}

