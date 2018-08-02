// Fill out your copyright notice in the Description page of Project Settings.

#include "KillFeedMessage.h"

#include "TimerManager.h"

void UKillFeedMessage::NativeConstruct()
{
	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UKillFeedMessage::RemoveKillFeedMessage, HideTime, false);
}

void UKillFeedMessage::RemoveKillFeedMessage()
{
	RemoveFromParent();
	RemoveFromViewport();
	
	// Ensure the timer is cleared by using the timer handle
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}
