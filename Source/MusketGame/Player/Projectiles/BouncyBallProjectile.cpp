// Fill out your copyright notice in the Description page of Project Settings.

// This Includes //
#include "BouncyBallProjectile.h"

// Engine Includes //
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABouncyBallProjectile::ABouncyBallProjectile()
{
	ProjectileMovementComponent->Bounciness = 2.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.6f;
}

