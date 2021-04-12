// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers += 1;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if(ActiveTriggers > 0)
	{
		ActiveTriggers -= 1;
	}
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

	if(HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(ActiveTriggers > 0)
	{
		if(HasAuthority())
		{
			FVector Location = GetActorLocation();
			const FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();

			const FVector NextLocation = Speed * DeltaSeconds * Direction;		
			Location += NextLocation;

			const float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
			const float JourneyTravelled = (Location - GlobalStartLocation).Size();

			if(JourneyTravelled >= JourneyLength)
			{
				const FVector Swap = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = Swap;
			}
		
			SetActorLocation(Location);
		}
	}
}
