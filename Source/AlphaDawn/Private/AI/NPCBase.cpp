// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPCBase.h"

#include "DGameplayTags.h"
#include "AI/DAIController.h"
#include "AlphaDawn/AlphaDawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Framework/BFLAbilitySystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/DAbilitySystemComponent.h"
#include "GAS/DAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "UI/GAS/DUserWidget.h"

ANPCBase::ANPCBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDAbilitySystemComponent>("ASC");
	AttributeSet = CreateDefaultSubobject<UDAttributeSet>("Attributes");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("Health");
	HealthBar->SetupAttachment(GetRootComponent());
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Combat");
	Weapon->SetupAttachment(GetMesh(), FName("weapon"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UBFLAbilitySystem::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	
	if (UDUserWidget* EnemyHealthUI = Cast<UDUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		EnemyHealthUI->SetWidgetController(this);
	}

	if (const UDAttributeSet* AS = Cast<UDAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChange.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChange.Broadcast(Data.NewValue);
			}
		);
		//Call Hit React/Stunned function
		AbilitySystemComponent->RegisterGameplayTagEvent(FDGameplayTags::Get().ability_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ANPCBase::HitReactTagChanged);
		//Broadcast Initial Values
		OnHealthChange.Broadcast(AS->GetHealth());
		OnMaxHealthChange.Broadcast(AS->GetMaxHealth());
	}
}

void ANPCBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!HasAuthority()) return;
	AIC = Cast<ADAIController>(NewController);
	AIC->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AIC->RunBehaviorTree(BehaviorTree);
	AIC->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AIC->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), false);
	AIC->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

int32 ANPCBase::GetPlayerLevel_Implementation()
{
	return Level;
}

FVector ANPCBase::GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag)
{
	const FDGameplayTags& GameplayTags = FDGameplayTags::Get();
	if (CombatSocketTag.MatchesTagExact(GameplayTags.combatSocket_weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponDamageSocket);
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

void ANPCBase::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	MulticastHandleDeath(DeathImpulse);
	if (AIC) AIC->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
	DissolveMesh();
}

void ANPCBase::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ANPCBase::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void ANPCBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse);
	
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

void ANPCBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AIC && AIC->GetBlackboardComponent())
	{
		AIC->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void ANPCBase::SetToStunned(bool bIsStunned) const
{
	if (bStunned)
	{
		bIsStunned = true;
		if (AIC && AIC->GetBlackboardComponent())
		{
			AIC->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bStunned);
		}
	}
	bIsStunned = false;
}

void ANPCBase::SetToEndStun()
{
	if (AIC && AIC->GetBlackboardComponent())
	{
		AIC->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), false);
	}
}

void ANPCBase::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UDAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	if (HasAuthority())
	{
		InitializeAttributes();
	}
	OnASCRegistered.Broadcast(AbilitySystemComponent);
}

void ANPCBase::InitializeAttributes() const
{
	UBFLAbilitySystem::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
