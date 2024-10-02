// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DAIController.generated.h"

/**
 * 
 */
UCLASS()
class ALPHADAWN_API ADAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    	
    ADAIController();
    	
protected:
    	
    UPROPERTY()
    UBehaviorTree* BTree;
	
};
