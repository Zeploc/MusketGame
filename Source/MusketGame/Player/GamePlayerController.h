// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MUSKETGAME_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


		UFUNCTION(BlueprintCallable)
	virtual void AddKillFeed(AActor* Killer);
	
	// Widget template class
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UUserWidget> HUDClass;

	// Widget Instance
	UPROPERTY(BlueprintReadOnly, Category = UI)
		class UUserWidget* HUDInstance;

	// Widget template class
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UUserWidget> EndScreenClass;

	// Widget Instance
	UPROPERTY(BlueprintReadOnly, Category = UI)
		class UUserWidget* EndScreenInstance;


	// Widget template class
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UUserWidget> PauseMenuClass;

	// Widget Instance
	UPROPERTY(BlueprintReadOnly, Category = UI)
		class UUserWidget* PauseMenuInstance;

	UFUNCTION(BlueprintCallable)
		void ToggleMenu();
	
	UFUNCTION(NetMulticast, Reliable)
		void GameComplete();
		void GameComplete_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void StartGame(float _GameTimer);
		void StartGame_Implementation(float _GameTimer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);


	UPROPERTY(Category = Game, EditDefaultsOnly, BlueprintReadWrite)
		float GameTimer = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	FInputModeUIOnly UIInputType;
	FInputModeGameOnly GameInputType;
};
