// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHUDWidget.h"
#include "MusketGame/UI/KillFeedMessage.h"
#include "Engine.h"

void UPlayerHUDWidget::AddToKillFeed(FText KillFeedText)
{
	if (KillFeedMessageUIClass)
	{
			UKillFeedMessage* NewKillFeedMessage = CreateWidget<UKillFeedMessage>(GetWorld(), KillFeedMessageUIClass);
			if (NewKillFeedMessage)
			{
				UTextBlock* KillfeedText = NewKillFeedMessage->KillTextMessage;// Cast<UKillFeedMessage>(NewKillFeedMessage)->KillFeedTextObject;
				if (KillfeedText) KillfeedText->Text = KillFeedText;
				else GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("Can't find kill feed text ref in new instance!"));

				KillFeedBox->AddChild(NewKillFeedMessage);
				UpdateKillFeed();
			}
			else GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("Can't add child, New killfeed Text Widget not valid!"));
		}
}

void UPlayerHUDWidget::UpdateKillFeed()
{
	int32 ChildrenCount = KillFeedBox->GetChildrenCount();
	if (ChildrenCount > KillFeedMaxMessages)
	{
		KillFeedBox->RemoveChildAt(0);
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("Removing killfeed message, too many"));
	}
}
