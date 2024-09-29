// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Framework/DPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/DAbilitySystemComponent.h"
#include "Components/EquipmentComponent.h"
#include "Input/DInputComponent.h"

ADPlayer::ADPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCam = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCam"));
	FollowCam->SetupAttachment(CameraBoom);
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	Gear = CreateDefaultSubobject<UEquipmentComponent>("GearComp");
}

void ADPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	GrantAbilities();
}

void ADPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void ADPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

void ADPlayer::InitAbilityActorInfo()
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	DPS->GetAbilitySystemComponent()->InitAbilityActorInfo(DPS, this);
	Cast<UDAbilitySystemComponent>(DPS->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = DPS->GetAbilitySystemComponent();
	AttributeSet = DPS->GetAttributeSet();
	//OnASCRegistered.Broadcast(AbilitySystemComponent);

	InitializeAttributes();
}

void ADPlayer::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void ADPlayer::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ADPlayer::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//if (Gear->EquippedWeapon == nullptr) return;
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UDAbilitySystemComponent* ADPlayer::GetASC()
{
	if (DAbilitySystemComponent == nullptr)
	{
		DAbilitySystemComponent = CastChecked<UDAbilitySystemComponent>(AbilitySystemComponent);
	}
	return DAbilitySystemComponent;
}

void ADPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UDInputComponent* InputComp = CastChecked<UDInputComponent>(InputComponent))
	{
		InputComp->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}
