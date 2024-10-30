// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBeam.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBeam::AProjectileBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and set Niagara Effect as the root component
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	RootComponent = NiagaraEffect;

	// Create a Collider and attach it to the root component
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
	Collider->InitSphereRadius(10.0f);
	Collider->SetCollisionProfileName(TEXT("Projectile"));
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collider->SetCollisionObjectType(ECC_PhysicsBody);
	Collider->SetCollisionResponseToAllChannels(ECR_Overlap);

	//Collider->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBeam::OnOverlapBegin);

	// Create a Projectile Movement Component and attach it to the root component
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->UpdatedComponent = RootComponent;
	ProjectileComponent->InitialSpeed = 10000.0f;
	ProjectileComponent->MaxSpeed = 10000.0f;
	ProjectileComponent->ProjectileGravityScale = 0.0f;
	ProjectileComponent->bRotationFollowsVelocity = true;
	ProjectileComponent->bShouldBounce = false;
}

void AProjectileBeam::Activates(FVector start, FVector end)
{
	// Calculate distance and direction
	float Distance = FVector::Distance(start, end);

	// Set the scale of the Niagara effect
	FVector Scale = FVector(1.0f, 1.0f, Distance / 100.0f); // Adjust the Z scale based on distance

	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	NiagaraEffect->SetWorldScale3D(Scale);
	GetWorldTimerManager().SetTimer(BeamDurationTimer, this, &AProjectileBeam::Deactivates, BeamDuration, false);
}

// Called when the game starts or when spawned
void AProjectileBeam::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBeam::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("OnOverlapBegin"));
	Deactivates();
	GetWorldTimerManager().ClearTimer(BeamDurationTimer);
}


void AProjectileBeam::Deactivates()
{
	ProjectileComponent->StopMovementImmediately();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}



