// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/BFLAbilitySystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "DGameplayTags.h"
#include "Engine/OverlapResult.h"
#include "Framework/DGameModeBase.h"
#include "Framework/DPlayerState.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GAS/AlphaHUD.h"
#include "UI/GAS/Controllers/WidgetController.h"

bool UBFLAbilitySystem::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAlphaHUD*& OutAlphaHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAlphaHUD = Cast<AAlphaHUD>(PC->GetHUD());
		if (OutAlphaHUD)
		{
			ADPlayerState* PS = PC->GetPlayerState<ADPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController= PC;
			FWidgetControllerParams(PC, PS, ASC, AS);
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UBFLAbilitySystem::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAlphaHUD* AlphaHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AlphaHUD))
	{
		return AlphaHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuController* UBFLAbilitySystem::GetAttributeMenuController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAlphaHUD* AlphaHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AlphaHUD))
	{
		return AlphaHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

UAbilityMenuController* UBFLAbilitySystem::GetAbilityMenuController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAlphaHUD* AlphaHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AlphaHUD))
	{
		return AlphaHUD->GetAbilityMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UBFLAbilitySystem::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharClassInfo ClassDefaultInfo = CharacterClassInfo->GetClassInfo(CharacterClass);

	//Add Source Objects(ASC) and Effect Context and Outgoing Spec for AI Attributes Data Asset: CharacterClassInfo
	FGameplayEffectContextHandle PrimaryAttributeContextHandle = ASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributeContextHandle = ASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttSpecHandle.Data.Get());

	FGameplayEffectContextHandle ResistanceAttributeContextHandle = ASC->MakeEffectContext();
	ResistanceAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle ResistanceAttSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->ResistanceAttributes, Level, ResistanceAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*ResistanceAttSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributeContextHandle = ASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttSpecHandle.Data.Get());
}

void UBFLAbilitySystem::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->SharedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharClassInfo& DefaultInfo = CharacterClassInfo->GetClassInfo(CharacterClass);
	for (auto AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UBFLAbilitySystem::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ADGameModeBase* GameMode = Cast<ADGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr) return nullptr;
	return GameMode->CharacterClassInfo;
}

UAbilityInfo* UBFLAbilitySystem::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ADGameModeBase* GameMode = Cast<ADGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr) return nullptr;
	return GameMode->AbilityInfo;
}

bool UBFLAbilitySystem::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsBlockedHit();
	}
	return false;
}

bool UBFLAbilitySystem::IsDodgedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsDodgedHit();
	}
	return false;
}

bool UBFLAbilitySystem::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsCriticalHit();
	}
	return false;
}

bool UBFLAbilitySystem::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UBFLAbilitySystem::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UBFLAbilitySystem::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UBFLAbilitySystem::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UBFLAbilitySystem::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (REffectContext->GetDamageType().IsValid())
		{
			return *REffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UBFLAbilitySystem::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UBFLAbilitySystem::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UBFLAbilitySystem::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsRadialDamage();
	}
	return false;
}

float UBFLAbilitySystem::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UBFLAbilitySystem::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UBFLAbilitySystem::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDGameplayEffectContext* REffectContext = static_cast<const FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UBFLAbilitySystem::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UBFLAbilitySystem::SetIsDodgedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDodgedHit)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsDodgedHit(bInIsDodgedHit);
	}
}

void UBFLAbilitySystem::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UBFLAbilitySystem::SetIsSucessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UBFLAbilitySystem::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UBFLAbilitySystem::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UBFLAbilitySystem::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UBFLAbilitySystem::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		REffectContext->SetDamageType(DamageType);
	}
}

void UBFLAbilitySystem::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetDeathImpulse(InImpulse);
	}
}

void UBFLAbilitySystem::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetKnockbackForce(InForce);
	}
}

void UBFLAbilitySystem::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UBFLAbilitySystem::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UBFLAbilitySystem::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UBFLAbilitySystem::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin)
{
	if (FDGameplayEffectContext* REffectContext = static_cast<FDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetRadialDamageOrigin(InOrigin);
	}
}

void UBFLAbilitySystem::GetTargetsWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
	const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UBFLAbilitySystem::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bPlayerTeam = FirstActor->ActorHasTag(FName("player")) && SecondActor->ActorHasTag(FName("player"));
	const bool bEnemyTeam = FirstActor->ActorHasTag(FName("enemy")) && SecondActor->ActorHasTag(FName("enemy"));
	const bool bFriend = bPlayerTeam || bEnemyTeam;
	return !bFriend;
}

int32 UBFLAbilitySystem::GetXPRewardForEnemySlay(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;
	const FCharClassInfo& Info = CharacterClassInfo->GetClassInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

FGameplayEffectContextHandle UBFLAbilitySystem::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FDGameplayTags& GameplayTags = FDGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);
	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

TArray<FRotator> UBFLAbilitySystem::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}	
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UBFLAbilitySystem::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}	
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}

void UBFLAbilitySystem::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UBFLAbilitySystem::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
	}
}

void UBFLAbilitySystem::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
	}
}

void UBFLAbilitySystem::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}
