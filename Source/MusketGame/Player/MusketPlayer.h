// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "MusketPlayer.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EWeaponEnum : uint8
{
	W_BOUNCYBALL 	UMETA(DisplayName = "Bouncy Ball"),
};

UCLASS()
class MUSKETGAME_API AMusketPlayer : public ACharacter
{
// ##################--DEFAULTS--##################
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMusketPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ##################--CUSTOM--##################

public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
	
	// Gun muzzle's offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;


	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ABaseProjectile> ProjectileClass;

	// Current Weapon Value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EWeaponEnum CurrentWeapon;
	
	// Widget template class
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UUserWidget> HUDClass;

	// Widget Instance
	UPROPERTY(BlueprintReadOnly, Category = UI)
		class UUserWidget* HUDInstance;


	// Widget template class
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UUserWidget> PauseMenuClass;

	// Widget Instance
	UPROPERTY(BlueprintReadOnly, Category = UI)
		class UUserWidget* PauseMenuInstance;


	// Current Weapon Value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, Replicated)
		float fCurrentHealth;

	// Current Weapon Value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		bool IsShoot;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);


	UFUNCTION(BlueprintCallable)
		void ToggleMenu();
	
protected:
	/** Gun mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;


	/** Handles player attempting to interact*/
	void Interact();

	/** Fires a virtual projectile. */
	void OnLeftFire();
	void OnRightFirePress();
	void OnRightFireRelease();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

private:

	/*UFUNCTION(BlueprintNativeEvent, Category = "")
		void doStuff();
	virtual void ShootProjectile_Implementation();*/

	// Shoots projectile (Take in type of projectile)
	UFUNCTION(Server, Reliable, WithValidation)
		void ShootProjectile();
		void ShootProjectile_Implementation();
		bool ShootProjectile_Validate();

	UFUNCTION(NetMulticast, Reliable)
		void KillPlayer();
		void KillPlayer_Implementation();


	UFUNCTION(Client, Reliable)
		void ResetPlayerView();
		void ResetPlayerView_Implementation();

	FVector SpawnPos;
	FRotator SpawnActorRotation;

	FInputModeUIOnly UIInputType;
	FInputModeGameOnly GameInputType;
};
