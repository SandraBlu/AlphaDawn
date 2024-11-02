// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DPlayer.h"

#include "DGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "Actors/Weapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Framework/DPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/DAbilitySystemComponent.h"
#include "Components/EquipmentComponent.h"
#include "Framework/DPlayerController.h"
#include "GAS/DataAssets/LevelUpInfo.h"
#include "Input/DInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GAS/AlphaHUD.h"

ADPlayer::ADPlayer()
{
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	Gear = CreateDefaultSubobject<UEquipmentComponent>("GearComp");

	CharacterClass = ECharacterClass::Elementalist;

	LevelUpFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFXComp"));
	LevelUpFX->SetupAttachment(GetRootComponent());
	LevelUpFX->bAutoActivate = false;
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

int32 ADPlayer::GetPlayerLevel_Implementation()
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	return DPS->GetPlayerLevel();
}

FVector ADPlayer::GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag)
{
	const FDGameplayTags& GameplayTags = FDGameplayTags::Get();
	if (CombatSocketTag.MatchesTagExact(GameplayTags.combatSocket_weapon) && IsValid(Gear->EquippedWeapon))
	{
		return Gear->EquippedWeapon->GetWeaponMesh()->GetSocketLocation(Gear->EquippedWeapon->FiringSocket);
	}
	if (CombatSocketTag.MatchesTagExact(GameplayTags.combatSocket_handL))
	{
		return GetMesh()->GetSocketLocation(HandRSocket);
	}
	if (CombatSocketTag.MatchesTagExact(GameplayTags.combatSocket_handR))
	{
		return GetMesh()->GetSocketLocation(HandLSocket);
	}
	return FVector();
}

void ADPlayer::Die(const FVector& DeathImpulse)
{
	MulticastHandleDeath(DeathImpulse);
}

void ADPlayer::AddToXP_Implementation(int32 InXP)
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	DPS->AddToXP(InXP);
}

void ADPlayer::LevelUp_Implementation()
{
	MulticastLevelUpVFX();
}

int32 ADPlayer::GetXP_Implementation() const
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	return DPS->GetXP();
}

int32 ADPlayer::FindLevelForXP_Implementation(int32 InXP) const
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	return DPS->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ADPlayer::GetAttributePtsReward_Implementation(int32 Level) const
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	return DPS->LevelUpInfo->LevelUpInfo[Level].AttributePointReward;
}

int32 ADPlayer::GetAbilityPtsReward_Implementation(int32 Level) const
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	return DPS->LevelUpInfo->LevelUpInfo[Level].AbilityPointReward;
}

void ADPlayer::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	DPS->AddToLevel(InPlayerLevel);
	if (UDAbilitySystemComponent* RASC = Cast<UDAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		RASC->UpdateAbilityStatus(DPS->GetPlayerLevel());
	}
}

void ADPlayer::AddToAttributePts_Implementation(int32 InAttributePoints)
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	DPS->AddToAttributePts(InAttributePoints);
}

void ADPlayer::AddToAbilityPts_Implementation(int32 InAbilityPoints)
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	DPS->AddToAbilityPts(InAbilityPoints);
}

int32 ADPlayer::GetAttributePoints_Implementation() const
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	return DPS->GetAttributePts();
}

int32 ADPlayer::GetAbilityPoints_Implementation() const
{
	ADPlayerState* DPS = GetPlayerState<ADPlayerState>();
	check(DPS);
	return DPS->GetAbilityPts();
}

AWeapon* ADPlayer::GetCurrentWeapon_Implementation()
{
	if (Gear->EquippedWeapon)
	{
		return Gear->EquippedWeapon;
	}
	return nullptr;
}

void ADPlayer::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	if (!Gear->EquippedWeapon) return;
	Gear->EquippedWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Gear->EquippedWeapon->GetWeaponMesh()->SetSimulatePhysics(true);
	Gear->EquippedWeapon->GetWeaponMesh()->SetEnableGravity(true);
	Gear->EquippedWeapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Gear->EquippedWeapon->GetWeaponMesh()->AddImpulse(DeathImpulse);

	UGameplayStatics::PlaySoundAtLocation(this, DeathCry, GetActorLocation(), GetActorRotation());
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	bDead = true;
	OnDeath.Broadcast(this);
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
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	
	if (ADPlayerController* DPC = Cast<ADPlayerController>(GetController()))
	{
		if (AAlphaHUD* DHUD = Cast<AAlphaHUD>(DPC->GetHUD()))
		{
			DHUD->InitOverlay(DPC, DPS, AbilitySystemComponent, AttributeSet);
		}
	}
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
	if (Gear->EquippedWeapon == nullptr) return;
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

void ADPlayer::MulticastLevelUpVFX_Implementation()
{
	if (IsValid(LevelUpFX))
	{
		LevelUpFX->Activate(true);
	}
}

void ADPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UDInputComponent* InputComp = CastChecked<UDInputComponent>(InputComponent))
	{
		InputComp->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}
