// Fill out your copyright notice in the Description page of Project Settings.

#include "MusketGameGameModeBase.h"


// Called when the game starts or when spawned
void AMusketGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, TEXT("Using Musket Game Mode"));
}

