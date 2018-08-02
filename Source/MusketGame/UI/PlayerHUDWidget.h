// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MUSKETGAME_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = KillFeed, EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* KillFeedBox;
	
	// Widget template class
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UUserWidget> KillFeedMessageUIClass;
	
	void AddToKillFeed(FText KillFeedText);

	void UpdateKillFeed();

	// Widget template class
	UPROPERTY(EditDefaultsOnly, Category = UI)
		int KillFeedMaxMessages = 5;
};
