// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fire.h"
#include "AIController.h"
#include "TP3Shoot/TP3ShootCharacter.h"

UBTTask_Fire::UBTTask_Fire()
{
	NodeName = "Fire Weapon";
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller and controlled pawn
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) return EBTNodeResult::Failed;

	// Cast to your character class
	ATP3ShootCharacter* Character = Cast<ATP3ShootCharacter>(AIController->GetPawn());
	if (Character == nullptr) return EBTNodeResult::Failed;

	// Call the Fire function
	Character->Fire();

	// Return success
	return EBTNodeResult::Succeeded;
}