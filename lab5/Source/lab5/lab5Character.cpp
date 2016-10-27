// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "lab5.h"
#include "lab5Character.h"
#include "Weapon.h"

Alab5Character::Alab5Character()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	MyWeapon = nullptr;
}

void Alab5Character::BeginPlay()
{
	// Call base class BeginPlay 
	Super::BeginPlay(); 
	// Spawn the weapon, if one was specified 
	if (WeaponClass) 
	{ 
		UWorld* World = GetWorld(); 
		if (World) 
		{
			FActorSpawnParameters SpawnParams; 
			SpawnParams.Owner = this; 
			SpawnParams.Instigator = Instigator;
			// Need to set rotation like this because otherwise gun points down 
			FRotator Rotation(0.0f, 0.0f, -90.0f);

			// Spawn the Weapon 
			MyWeapon = World->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, Rotation, SpawnParams); 
				
			if (MyWeapon) 
			{ 
				// This is attached to "WeaponPoint" which is defined in the skeleton 
				MyWeapon->AttachToComponent(GetMesh(),
							FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponPoint")); 
				MyWeapon->SetMyOwner(this);
			}
		} 
	} 
}

void Alab5Character::OnStartFire()
{
	if (MyWeapon != nullptr)
	{
		MyWeapon->OnStartFire();
	}
}

void Alab5Character::OnStopFire()
{
	if (MyWeapon != nullptr)
	{
		MyWeapon->OnStopFire();
	}
}
