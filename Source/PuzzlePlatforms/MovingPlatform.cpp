// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(HasAuthority())
	{
		FVector Location = GetActorLocation();
		const FVector GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
		const FVector Direction = (GlobalTargetLocation - Location).GetSafeNormal();		
		Location += Speed * DeltaSeconds * Direction;
		SetActorLocation(Location);
	}
}
