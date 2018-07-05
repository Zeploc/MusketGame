// Fill out your copyright notice in the Description page of Project Settings.

// This Includes //
#include "BaseProjectile.h"

// Engine Includes //
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Engine.h"

// Local Includes //
#include "MusketGame/Player/MusketPlayer.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetSimulatePhysics(false);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
	// Set the sphere's collision radius.
	CollisionComponent->InitSphereRadius(15.0f);
	// Set the root component to be the collision component.
	RootComponent = CollisionComponent;

	// Use this component to drive this projectile's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(CollisionComponent);	// Attach mesh to FirstPersonCameraComponent

	SetReplicates(true);

	// Die after 3 seconds.
	InitialLifeSpan = 3.0f;	

	PlayerOwner = nullptr;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ABaseProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	AMusketPlayer* PlayerRef = Cast<AMusketPlayer>(OtherActor);
	if (PlayerRef)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("Projectile Hit Player"));
		TSubclassOf<UDamageType> DamageType;
		UGameplayStatics::ApplyPointDamage(OtherActor, 10.0f, GetActorLocation(), Hit, nullptr, PlayerOwner, DamageType);
		//PlayerRef->PlayerProjectileHit();
		Destroy();
		return;
	}
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 20.0f, Hit.ImpactPoint);
	}
}

