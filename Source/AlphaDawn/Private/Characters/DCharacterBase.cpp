// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "DGameplayTags.h"
#include "GameplayEffectTypes.h"
#include "GAS/DAbilitySystemComponent.h"
#include "GAS/DebuffNiagaraComponent.h"

// Sets default values
ADCharacterBase::ADCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	EffectDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("EffectDebuffComponent");
	EffectDebuffComponent->SetupAttachment(GetRootComponent());
	EffectDebuffComponent->DebuffTag = FDGameplayTags::Get().Debuff_Stun;
	EffectAttachComp = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	EffectAttachComp->SetupAttachment(GetRootComponent());

	//PassiveCloak = CreateDefaultSubobject<UPassiveNiagaraComponent>("CloakComponent");
	//PassiveCloak->SetupAttachment(EffectAttachComp);

}

void ADCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	EffectAttachComp->SetWorldRotation((FRotator::ZeroRotator));
}

UAbilitySystemComponent* ADCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ADCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageTaken =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

UAnimMontage* ADCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

UAnimMontage* ADCharacterBase::GetStunnedMontage_Implementation()
{
	return StunnedMontage;
}

bool ADCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ADCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ADCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* ADCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

ECharacterClass ADCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered ADCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDeath ADCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

void ADCharacterBase::Die(const FVector& DeathImpulse)
{
}

FOnDamageSignature& ADCharacterBase::GetOnDamageSignature()
{
	return OnDamageDelegate;
}

void ADCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADCharacterBase::InitAbilityActorInfo()
{
}

 void ADCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
 {
 	check(IsValid(GetAbilitySystemComponent()));
 	check(GameplayEffectClass);
 	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
 	ContextHandle.AddSourceObject(this);
 	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
 	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
 }

void ADCharacterBase::InitializeAttributes() const
{
	ApplyEffectToSelf(PrimaryAttributes, 1.f);
	ApplyEffectToSelf(SecondaryAttributes, 1.f);
	ApplyEffectToSelf(ResistanceAttributes, 1.f);
	ApplyEffectToSelf(VitalAttributes, 1.f);
}

void ADCharacterBase::GrantAbilities()
{
	UDAbilitySystemComponent* DASC = CastChecked<UDAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	DASC->AddGrantedAbilities(GrantedAbilities);
	DASC->AddPassiveAbilities(GrantedPassiveAbilities);
}


