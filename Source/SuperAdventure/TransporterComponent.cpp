// Fill out your copyright notice in the Description page of Project Settings.


#include "TransporterComponent.h"
#include "CollectableKey.h"
#include "PressurePlate.h"

// Sets default values for this component's properties
UTransporterComponent::UTransporterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	MoveTime = 3.0f;
	ActivatedTriggerCount = 0;
	ArePointsSet = false;
	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
}


// Called when the game starts
void UTransporterComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerIsTriggerActor)
	{
		TriggerActors.Add(GetOwner());
	}
	
	for(AActor* Trigger : TriggerActors)
	{
		APressurePlate *PressurePlateActor = Cast<APressurePlate>(Trigger);
		if (PressurePlateActor)
		{
			PressurePlateActor->OnActivated.AddDynamic(this, &UTransporterComponent::OnTriggerActorActivated);
			PressurePlateActor->OnDeactivated.AddDynamic(this, &UTransporterComponent::OnTriggerActorDeactivated);
			continue;
		}

		ACollectableKey* KeyActor = Cast<ACollectableKey>(Trigger);
		if (KeyActor)
		{
			KeyActor->OnCollected.AddDynamic(this, &UTransporterComponent::OnTriggerActorActivated);
		}
	}
}


// Called every frame
void UTransporterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TriggerActors.Num() > 0)
	{
		AllTriggerActorsTriggered = (ActivatedTriggerCount >= TriggerActors.Num());
	}

	AActor* Owner = GetOwner();
	if (Owner && Owner->HasAuthority() && ArePointsSet)
	{
		FVector CurrentLocation = Owner->GetActorLocation();
		float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;

		FVector TargetLocation = AllTriggerActorsTriggered ? EndPoint : StartPoint;
		if (!CurrentLocation.Equals(TargetLocation))
		{
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			Owner->SetActorLocation(NewLocation);
		}
	}
}

void UTransporterComponent::SetPoints(FVector Point1, FVector Point2)
{
	if (Point1.Equals(Point2)) return;

	StartPoint = Point1;
	EndPoint = Point2;
	ArePointsSet = true;
}

void UTransporterComponent::OnTriggerActorActivated()
{
	ActivatedTriggerCount++;
}

void UTransporterComponent::OnTriggerActorDeactivated()
{
	ActivatedTriggerCount--;
}

