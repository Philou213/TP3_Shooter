// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "TP3Shoot/TP3ShootCharacter.h"
#include "AICharacter.h"
#include <Kismet/GameplayStatics.h>

AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer)
{
	// Create BehaviorTree Component
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ATP3ShootCharacter* Shooter = Cast<ATP3ShootCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ATP3ShootCharacter::StaticClass()));
	AAICharacter* AI = Cast<AAICharacter>(InPawn);
	if (AI && AI->TreeAsset->BlackboardAsset)
	{
		UBlackboardComponent* BlackboardComponent;
		UseBlackboard(AI->TreeAsset->BlackboardAsset, BlackboardComponent);
		BlackboardComponent->SetValueAsObject("PlayerActor", Shooter);

		BehaviorTreeComponent->StartTree(*AI->TreeAsset);
	}
}
