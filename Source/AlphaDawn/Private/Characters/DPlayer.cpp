// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Framework/DPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/DAbilitySystemComponent.h"

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
	//Cast<UDAbilitySystemComponent>(DPS->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = DPS->GetAbilitySystemComponent();
	AttributeSet = DPS->GetAttributeSet();
	//OnASCRegistered.Broadcast(AbilitySystemComponent);
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
}
