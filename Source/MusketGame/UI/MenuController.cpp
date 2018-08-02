// Fill out your copyright notice in the Description page of Project Settings.

// This Includes //
#include "MenuController.h"

// Engine Includes //
#include "Blueprint/UserWidget.h"
#include "Engine.h"

// Local Includes //
#include "NetworkSession/NWGameInstance.h"

// Called when the game starts or when spawned
void AMenuController::BeginPlay()
{
	ShowWidget(MainWidgetClass, MainWidgetInstance);
	bShowMouseCursor = true;

	/** Bind function for NETWORK ERROR handling */
	GEngine->OnNetworkFailure().AddUObject(Cast<UNWGameInstance>(GetGameInstance()), &UNWGameInstance::HandleNetworkFailure);
}

void AMenuController::ShowHostGame()
{
	ShowWidget(HostWidgetClass, HostWidgetInstance);	
}

void AMenuController::ShowJoinGame()
{
	ShowWidget(JoinWidgetClass, JoinWidgetInstance);
}

void AMenuController::EndGame()
{
	ConsoleCommand("quit");
}

void AMenuController::ShowMain()
{
	ShowWidget(MainWidgetClass, MainWidgetInstance);
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, "Back to main menu");
}

void AMenuController::StartHost()
{
	UNWGameInstance* NetworkGameInstance = Cast<UNWGameInstance>(GetGameInstance());
	if (NetworkGameInstance->HostSession(GetLocalPlayer()->GetPreferredUniqueNetId(), "ServerName",  true, true, 2))
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, "Creating Game Session...");
	}
	/*const FString& MapName = "TestLevel";
	const FString URL = "/Game/Maps/" + MapName + "?Listen";
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, "Hosting Listen server on map " + MapName);
	GetWorld()->ServerTravel(URL);*/
	FInputModeGameOnly GameInputType;
	SetInputMode(GameInputType);
}

void AMenuController::JoinServer(const FString IPAddress)
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, "Attempting to connect to " + IPAddress);
	ClientTravel(IPAddress, TRAVEL_Absolute);
	FInputModeGameOnly GameInputType;
	SetInputMode(GameInputType);
}

void AMenuController::SearchForServers()
{
	UNWGameInstance* NetworkGameInstance = Cast<UNWGameInstance>(GetGameInstance());
	NetworkGameInstance->FindSessions(GetLocalPlayer()->GetPreferredUniqueNetId(), true, true);
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, "Looking for available Game Sessions...");
	FInputModeGameOnly GameInputType;
	SetInputMode(GameInputType);
}

void AMenuController::SetAllInvisible()
{
	if (MainWidgetInstance) MainWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	if (HostWidgetInstance) HostWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	if (JoinWidgetInstance) JoinWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
}

void AMenuController::ShowWidget(TSubclassOf<class UUserWidget> WidgetClass, UUserWidget * WidgetToShow)
{
	SetAllInvisible();
	if (WidgetClass)
	{
		if (!WidgetToShow)
		{
			WidgetToShow = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
			WidgetToShow->AddToViewport();
		}
		else
		{
			WidgetToShow->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, "Hud Class not found");
	InputType.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputType);
}
