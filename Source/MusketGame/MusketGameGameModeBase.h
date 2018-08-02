// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MusketGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MUSKETGAME_API AMusketGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = Game, EditDefaultsOnly)
		float GameTimer = 30.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	/* Handle to manage the timer */
	FTimerHandle GameTimerHandle;
	

	UPROPERTY(Category = KillFeed, EditAnywhere, BlueprintReadWrite)
		bool WinConditionTimer = true;

	UFUNCTION()
		void ServerGameComplete();
		
};
