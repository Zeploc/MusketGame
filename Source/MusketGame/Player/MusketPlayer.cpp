// Fill out your copyright notice in the Description page of Project Settings.

// This Includes //
#include "MusketPlayer.h"

// Engine Includes //
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine.h"
#include "Blueprint/UserWidget.h"
#include "UnrealNetwork.h"

// Local Includes //
#include "Projectiles/BaseProjectile.h"

// Sets default values
AMusketPlayer::AMusketPlayer()
{

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);				// Set so only owner can see mesh
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);	// Attach mesh to FirstPersonCameraComponent
	Mesh1P->bCastDynamicShadow = false;			// Disallow mesh to cast dynamic shadows
	Mesh1P->CastShadow = false;				// Disallow mesh to cast other shadows

	// The owning player doesn't see the regular (third-person) body mesh.
	GetMesh()->SetOwnerNoSee(true);

	CurrentWeapon = EWeaponEnum::W_BOUNCYBALL;
}

// Called when the game starts or when spawned
void AMusketPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (HUDClass)
	{
		HUDInstance = CreateWidget<UUserWidget>(GetWorld(), HUDClass);
		if (HUDInstance) HUDInstance->AddToViewport();
		else GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("HUD INSTANCE not set!"));
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("HUDClass not set!"));


	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, TEXT("Using Musket Player"));

	if (HasAuthority())
		fCurrentHealth = 100.0f;

	SpawnPos = GetActorLocation();
	SpawnActorRotation = GetActorRotation(); //GetController()
	IsShoot = false;
	UIInputType.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
}


// Called every frame
void AMusketPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMusketPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("LeftFire", IE_Pressed, this, &AMusketPlayer::OnLeftFire);
	PlayerInputComponent->BindAction("RightFire", IE_Pressed, this, &AMusketPlayer::OnRightFirePress);
	PlayerInputComponent->BindAction("RightFire", IE_Released, this, &AMusketPlayer::OnRightFireRelease);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMusketPlayer::Interact);
	
	PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AMusketPlayer::ToggleMenu);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMusketPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMusketPlayer::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMusketPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMusketPlayer::LookUpAtRate);
}

void AMusketPlayer::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Interact Called!"));
}

void AMusketPlayer::OnLeftFire()
{
	ShootProjectile();
	IsShoot = true;
}

void AMusketPlayer::OnRightFirePress()
{
	FirstPersonCameraComponent->FieldOfView = 50.0f;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Zoom In"));
}
void AMusketPlayer::OnRightFireRelease()
{
	FirstPersonCameraComponent->FieldOfView = 90.0f;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Zoom Out"));
}

void AMusketPlayer::ShootProjectile_Implementation()
{
	// Attempt to fire a projectile.
	if (ProjectileClass && Role == ROLE_Authority)
	{
		// Would check if can shoot (again)
		
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// Skew the aim to be slightly upwards.
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// Spawn the projectile at the muzzle.
			ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				Projectile->PlayerOwner = this;
			}
		}
	}
}

bool AMusketPlayer::ShootProjectile_Validate()
{
	return true;
}

void AMusketPlayer::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AMusketPlayer, fCurrentHealth);
}

float AMusketPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority()) return Damage;
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, TEXT("Server applying damage to " + FString::FromInt(GetNetOwningPlayer()->GetUniqueID()) + " from " + FString::FromInt(DamageCauser->GetNetOwningPlayer()->GetUniqueID())));
	//GetWorld()->GetAuthGameMode()->GameState->PlayerArray[0]->PlayerId
	if (fCurrentHealth > 0 ) fCurrentHealth -= Damage;
	if (fCurrentHealth <= 0)
	{
		KillPlayer();
		if (Role == ROLE_SimulatedProxy || Role == ROLE_AutonomousProxy) // If this player is controlled by the server (I think)
			GetController()->SetControlRotation(SpawnActorRotation); // Set current controller rotation
		else // this player is controlled by a client
			ResetPlayerView(); // send message to controlling client
		//GetWorld()->GetAuthGameMode()->FindPlayerStart()
	}
	return Damage;
}

void AMusketPlayer::ToggleMenu()
{
	if (PauseMenuClass)
	{
		if (!PauseMenuInstance)
		{
			PauseMenuInstance = CreateWidget<UUserWidget>(GetWorld(), PauseMenuClass);
			PauseMenuInstance->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetInputMode(UIInputType);
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
		else
		{
			if (PauseMenuInstance->GetVisibility() == ESlateVisibility::Visible)
			{
				PauseMenuInstance->SetVisibility(ESlateVisibility::Hidden);
				GetWorld()->GetFirstPlayerController()->SetInputMode(GameInputType);
				GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
			}
			else
			{
				PauseMenuInstance->SetVisibility(ESlateVisibility::Visible);
				GetWorld()->GetFirstPlayerController()->SetInputMode(UIInputType);
				GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
			}
		}
	}
}

void AMusketPlayer::KillPlayer_Implementation()
{
	SetActorLocation(SpawnPos);
	fCurrentHealth = 100.0f;
}

void AMusketPlayer::ResetPlayerView_Implementation()
{
	GetController()->SetControlRotation(SpawnActorRotation); // Tell controlling client to set current controller rotation
}


void AMusketPlayer::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void AMusketPlayer::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void AMusketPlayer::TurnAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMusketPlayer::LookUpAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}