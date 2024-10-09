// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectiles/OverlapMissile.h"
#include "Fireball.generated.h"

/**
 * 
 */
UCLASS()
class ALPHADAWN_API AFireball : public AOverlapMissile
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	UPROPERTY(BlueprintReadOnly)
	AActor* ReturnToActor;
	
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
	
protected:
	
	virtual void BeginPlay() override;
	
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void PostInitializeComponents() override;
	
};
