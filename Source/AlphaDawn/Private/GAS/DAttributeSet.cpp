// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DGameplayTags.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Framework/BFLAbilitySystem.h"
#include "Framework/DPlayerController.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

UDAttributeSet::UDAttributeSet()
{
	{
		const FDGameplayTags& GameplayTags = FDGameplayTags::Get();
	
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Agility, GetAgilityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Perception, GetPerceptionAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	//Secondary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCritHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCritHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCritHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_DodgeChance, GetDodgeChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Stealth, GetStealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegen, GetHealthRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_StaminaRegen, GetStaminaRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_EnergyRegen, GetEnergyRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxStamina, GetMaxStaminaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxEnergy, GetMaxEnergyAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Dark, GetDarkResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Electric, GetElectricResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Ice, GetIceResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Poison, GetPoisonResistanceAttribute);
	}
}

void UDAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Health, COND_None, REPNOTIFY_Always)
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Stamina, COND_None, REPNOTIFY_Always)
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always)
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Energy, COND_None, REPNOTIFY_Always)
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always)

// Primary Attributes
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Agility, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Perception, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Strength, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

// Secondary Attributes

DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Armor, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, DodgeChance, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, CritHitChance, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, CritHitDamage, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, CritHitResistance, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);	
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, StaminaRegen, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, EnergyRegen, COND_None, REPNOTIFY_Always);	
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);	
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, Stealth, COND_None, REPNOTIFY_Always);

// Resistance Attributes
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, PoisonResistance, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, DarkResistance, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, ElectricResistance, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
DOREPLIFETIME_CONDITION_NOTIFY(UDAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);
}

void UDAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
}

void UDAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(GetEnergy(), 0.f, GetMaxEnergy()));
	}
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UDAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bRestoreFullHealth)
	{
		SetHealth(GetMaxHealth());
		bRestoreFullHealth = false;
	}
	if (Attribute ==GetMaxStaminaAttribute() && bRestoreFullStamina)
	{
		SetStamina(GetMaxStamina());
		bRestoreFullStamina = false;
	}
	if (Attribute ==GetMaxEnergyAttribute() && bRestoreFullEnergy)
	{
		SetEnergy(GetMaxEnergy());
		bRestoreFullEnergy = false;
	}
}

void UDAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UDAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalDamage = GetDamage();
	SetDamage(0.f);
	if (LocalDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
				CombatInterface->Die(UBFLAbilitySystem::GetDeathImpulse(Props.EffectContextHandle));
			}
			SendXPEvent(Props);
		}
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FDGameplayTags::Get().ability_HitReact);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			const FVector& KnockbackForce = UBFLAbilitySystem::GetKnockbackForce(Props.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
		}
		const bool bBlock = UBFLAbilitySystem::IsBlockedHit(Props.EffectContextHandle);
		const bool bDodge = UBFLAbilitySystem::IsDodgedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UBFLAbilitySystem::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalDamage,bBlock, bDodge, bCriticalHit);
		if (UBFLAbilitySystem::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			Debuff(Props);
		}
	}
}

void UDAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalXP = GetXP();
	SetXP(0.f);
	//Add to XP
	const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
	const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
	const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalXP);
	const int32 NumLevelUps = NewLevel - CurrentLevel;
	if (NumLevelUps > 0)
	{
		IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
		
		int32 AttributePtsReward = 0;
		int32 AbilityPtsReward = 0;

		for (int32 i=0; i < NumLevelUps; ++i)
		{
			AttributePtsReward += IPlayerInterface::Execute_GetAttributePtsReward(Props.SourceCharacter, CurrentLevel + i);
			AbilityPtsReward += IPlayerInterface::Execute_GetAbilityPtsReward(Props.SourceCharacter, CurrentLevel + i);	
		}
		
		IPlayerInterface::Execute_AddToAttributePts(Props.SourceCharacter, AttributePtsReward);
		IPlayerInterface::Execute_AddToAbilityPts(Props.SourceCharacter, AbilityPtsReward);

		bRestoreFullHealth = true;
		bRestoreFullStamina = true;
		bRestoreFullEnergy = true;
			
		IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
	}
	IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalXP);
}

void UDAttributeSet::Debuff(const FEffectProperties& Props)
{
	//SetEffectProperties----------------------------------------------------------------------
	const FDGameplayTags& Tag = FDGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);
	
	const FGameplayTag DamageType = UBFLAbilitySystem::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UBFLAbilitySystem::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UBFLAbilitySystem::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UBFLAbilitySystem::GetDebuffFrequency(Props.EffectContextHandle);
	
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(Tag.DamageTypeToDebuff[DamageType]);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModInfo = Effect->Modifiers[Index];

	ModInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModInfo.ModifierOp = EGameplayModOp::Additive;
	ModInfo.Attribute = UDAttributeSet::GetDamageAttribute();
	
	if  (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FDGameplayEffectContext* DContext = static_cast<FDGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		DContext->SetDamageType(DebuffDamageType);
		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UDAttributeSet::ShowFloatingText(const FEffectProperties& Props, float DamageAmount, bool bBlockedHit, bool bDodgedHit, bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if(ADPlayerController* PC = Cast<ADPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(DamageAmount, Props.TargetCharacter, bBlockedHit, bDodgedHit, bCriticalHit);
			return;
		}
		if(ADPlayerController* PC = Cast<ADPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(DamageAmount, Props.TargetCharacter, bBlockedHit, bDodgedHit, bCriticalHit);
		}
	}
}

void UDAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UBFLAbilitySystem::GetXPRewardForEnemySlay(Props.TargetCharacter, TargetClass, TargetLevel);
		const FDGameplayTags& GameplayTags = FDGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_meta_xp;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_meta_xp, Payload);
	}
}

void UDAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Health, OldHealth);
}

void UDAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, MaxHealth, OldMaxHealth);
}

void UDAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Stamina, OldStamina);
}

void UDAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Agility, OldAgility);
}

void UDAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Intelligence, OldIntelligence);
}

void UDAttributeSet::OnRep_Perception(const FGameplayAttributeData& OldPerception) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Perception, OldPerception);
}

void UDAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Resilience, OldResilience);
}

void UDAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Strength, OldStrength);
}

void UDAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Vigor, OldVigor);
}

void UDAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, MaxStamina, OldMaxStamina);
}

void UDAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Energy, OldEnergy);
}

void UDAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, MaxEnergy, OldMaxEnergy);
}

void UDAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Armor, OldArmor);
}

void UDAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UDAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, BlockChance, OldBlockChance);
}

void UDAttributeSet::OnRep_DodgeChance(const FGameplayAttributeData& OldDodgeChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, DodgeChance, OldDodgeChance);
}

void UDAttributeSet::OnRep_CritHitChance(const FGameplayAttributeData& OldCritHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, CritHitChance, OldCritHitChance);
}

void UDAttributeSet::OnRep_CritHitDamage(const FGameplayAttributeData& OldCritHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, CritHitDamage, OldCritHitDamage);
}

void UDAttributeSet::OnRep_CritHitResistance(const FGameplayAttributeData& OldCritHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, CritHitResistance, OldCritHitResistance);
}

void UDAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, HealthRegen, OldHealthRegen);
}

void UDAttributeSet::OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, StaminaRegen, OldStaminaRegen);
}

void UDAttributeSet::OnRep_EnergyRegen(const FGameplayAttributeData& OldEnergyRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, EnergyRegen, OldEnergyRegen);
}

void UDAttributeSet::OnRep_Stealth(const FGameplayAttributeData& OldStealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, Stealth, OldStealth);
}

void UDAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UDAttributeSet::OnRep_PoisonResistance(const FGameplayAttributeData& OldPoisonResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, PoisonResistance, OldPoisonResistance);
}

void UDAttributeSet::OnRep_DarkResistance(const FGameplayAttributeData& OldDarkResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, DarkResistance, OldDarkResistance);
}

void UDAttributeSet::OnRep_ElectricResistance(const FGameplayAttributeData& OldElectricResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, ElectricResistance, OldElectricResistance);
}

void UDAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, FireResistance, OldFireResistance);
}

void UDAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDAttributeSet, IceResistance, OldIceResistance);
}
