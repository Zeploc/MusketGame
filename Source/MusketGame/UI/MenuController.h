// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuController.generated.h"

/**
 * 
 */
UCLASS()
class MUSKETGAME_API AMenuController : public APlayerController
{
	// ##################--DEFAULT--##################
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ##################--CUSTOM--##################

public:

	// Widget template class
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UUserWidget> MainWidgetClass;

	// Widget Instance
	UPROPERTY(BlueprintReadOnly, Category = UI)
		class UUserWidget* MainWidgetInstance;

	// Widget template class
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UUserWidget> HostWidgetClass;

	// Widget Instance
	UPROPERTY(BlueprintReadOnly, Category = UI)
		class UUserWidget* HostWidgetInstance;

	// Widget template class
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UUserWidget> JoinWidgetClass;

	// Widget Instance
	UPROPERTY(BlueprintReadOnly, Category = UI)
		class UUserWidget* JoinWidgetInstance;
	
	UFUNCTION(BlueprintCallable)
		void ShowHostGame();

	UFUNCTION(BlueprintCallable)
		void ShowJoinGame();

	UFUNCTION(BlueprintCallable)
		void EndGame();

	UFUNCTION(BlueprintCallable)
		void ShowMain();

	UFUNCTION(BlueprintCallable)
		void StartHost();

	UFUNCTION(BlueprintCallable)
		void JoinServer(const FString IPAddress);

private:
	void SetAllInvisible();
	void ShowWidget(TSubclassOf<class UUserWidget> WidgetClass, UUserWidget* WidgetToShow);


	FInputModeUIOnly InputType;
	
};
