// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Team.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TP3SHOOT_API UTeam : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeam();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess=true))
	int teamId;

	bool IsSameTeam(int otherTeamId);


		
};
