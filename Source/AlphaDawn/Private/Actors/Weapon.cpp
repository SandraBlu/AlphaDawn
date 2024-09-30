// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon.h"

#include "Characters/DPlayer.h"
#include "Components/BoxComponent.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AWeapon::AWeapon()
{
	SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SKMesh->bReceivesDecals = false;
	SKMesh->SetCollisionObjectType(ECC_WorldDynamic);
	SKMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SKMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = SKMesh;

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());

	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(GetRootComponent());

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(GetRootComponent());

	ActiveWeaponSocket = FName("weapon_r");

}

void AWeapon::OnEquip()
{
	AttachMeshToPawn(PawnOwner->GetMesh(), ActiveWeaponSocket);
	if (EquipSFX)
	{
		UpdateCombatType(CombatType);
		UGameplayStatics::PlaySoundAtLocation(this, EquipSFX, GetActorLocation());
	}
}

void AWeapon::OnUnEquip()
{
	AttachMeshToPawn(PawnOwner->GetMesh(), InactiveWeaponSocket);
	if (EquipSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DisarmSFX, GetActorLocation());
	}
}

USkeletalMeshComponent* AWeapon::GetWeaponMesh() const
{
	return SKMesh;
}

void AWeapon::UpdateCombatType(ECombatType)
{
	CombatType;
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GetWorld Location Of Start and End Scene Components
	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceEnd->GetComponentLocation();
	//Add actors to Ignore Array: this = weapon
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;

	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, FVector(5.f, 5.f, 5.f), TraceStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false,
		ActorsToIgnore, EDrawDebugTrace::ForDuration, BoxHit, true);

	if (BoxHit.GetActor())
	{
		if (ICombatInterface* iHit = Cast<ICombatInterface>(BoxHit.GetActor()))
		{
			iHit->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
			OnHit(BoxHit);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	PawnOwner = Cast<ADPlayer>(GetOwner());
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::PlayEquipMontage(const FName SectionName)
{
	UAnimInstance* AnimInstance = PawnOwner->GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipAnim)
	{
		AnimInstance->Montage_Play(EquipAnim);
		AnimInstance->Montage_JumpToSection(SectionName, EquipAnim);
	}
}

class ADPlayer* AWeapon::GetPawnOwner() const
{
	return PawnOwner;
}

void AWeapon::AttachMeshToPawn(USceneComponent* InParent, FName InSocketName)
{
	if (PawnOwner)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		USkeletalMeshComponent* PawnMesh = PawnOwner->GetMesh();
		AttachToComponent(PawnOwner->GetMesh(), TransformRules, InSocketName);
	}
}

