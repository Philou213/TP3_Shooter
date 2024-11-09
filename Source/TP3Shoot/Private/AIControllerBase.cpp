// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "TP3Shoot/TP3ShootCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "AICharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Team.h>

AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer)
{
	// Create BehaviorTree Component
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));


	// Create the perception component
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SetPerceptionComponent(*PerceptionComponent);

	// Create the sight config
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SightRadius = 3000.0f;
	SightConfig->SetMaxAge(8.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 200.0f;
	SightConfig->LoseSightRadius = 3500;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


	AAIController::GetPerceptionComponent()->ConfigureSense(*SightConfig);

	//AAIController::GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnTargetPerceptionUpdated);

}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ATP3ShootCharacter* Shooter = Cast<ATP3ShootCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ATP3ShootCharacter::StaticClass()));
	ATP3ShootCharacter* AI = Cast<ATP3ShootCharacter>(InPawn);
	if (AI && AI->TreeAsset->BlackboardAsset)
	{
		UseBlackboard(AI->TreeAsset->BlackboardAsset, BlackboardComponent);
		BlackboardComponent->SetValueAsObject("PlayerActor", Shooter);

		BehaviorTreeComponent->StartTree(*AI->TreeAsset);
	}
}

void AAIControllerBase::OnTargetPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) const
{
	// Check if the perceived actor is a TP3ShootCharacter
	ATP3ShootCharacter* ShootCharacter = Cast<ATP3ShootCharacter>(Actor);
	if (!ShootCharacter) return;

	// Get the AI's team ID
	UTeam* AITeamComponent = Cast<UTeam>(GetPawn()->FindComponentByClass<UTeam>());
	if (!AITeamComponent) return;

	int AITeamId = AITeamComponent->GetTeamId();

	// Check if the target character is on a different team
	if (!ShootCharacter->Team || !ShootCharacter->Team->IsSameTeam(AITeamId))
	{
		// Check if the stimulus type is sight
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			// If stimulus is sensed successfully
			if (Stimulus.WasSuccessfullySensed())
			{
				// Update blackboard values
				BlackboardComponent->SetValueAsBool("CanSeePlayer", true);
				BlackboardComponent->SetValueAsObject("Target", Actor);
			}
			else
			{
				BlackboardComponent->SetValueAsBool("CanSeePlayer", false);
				BlackboardComponent->SetValueAsObject("Target", nullptr);
			}
		}
	}
}
