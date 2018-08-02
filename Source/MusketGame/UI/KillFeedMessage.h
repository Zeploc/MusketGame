// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "KillFeedMessage.generated.h"

/**
 * 
 */
UCLASS()
class MUSKETGAME_API UKillFeedMessage : public UUserWidget
{
	GENERATED_BODY()

	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(Category = KillFeed, EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* KillTextMessage;
	
	UPROPERTY(Category = KillFeed, EditDefaultsOnly)
		float HideTime = 10.0f;

	/* Handle to manage the timer */
	FTimerHandle TimerHandle;

	UFUNCTION()
		void RemoveKillFeedMessage();
};
