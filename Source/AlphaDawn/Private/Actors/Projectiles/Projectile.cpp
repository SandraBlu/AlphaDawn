// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/Projectile.h"

#include "NiagaraComponent.h"
#include "AlphaDawn/AlphaDawn.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetCollisionObjectType(ECC_Projectile);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UNiagaraComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
}

void AProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// PostInitializeComponent is the preferred way of binding any events.
	SphereComp->OnComponentHit.AddDynamic(this, &AProjectile::OnActorHit);
	SetLifeSpan(Lifespan);
}

void AProjectile::Explode_Implementation()
{
	if (IsValid(this))
	{
		
	}
}

