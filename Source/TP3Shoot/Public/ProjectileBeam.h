// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileBeam.generated.h"


UCLASS()
class TP3SHOOT_API AProjectileBeam : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UNiagaraComponent* NiagaraEffect;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Collider;

	UPROPERTY(EditAnywhere, Category = "Timer")
	float BeamDuration = 5.0f;
	
public:	
	// Sets default values for this actor's properties
	AProjectileBeam();
	void Activates(FVector start, FVector end);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle BeamDurationTimer;
	void Deactivates();
	
};
