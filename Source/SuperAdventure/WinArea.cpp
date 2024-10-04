// Fill out your copyright notice in the Description page of Project Settings.


#include "WinArea.h"
#include "SuperAdventureCharacter.h"


// Sets default values
AWinArea::AWinArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	WinAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Win Area"));
	SetRootComponent(WinAreaBox);

	WinCondition = false;
}

// Called when the game starts or when spawned
void AWinArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (!WinCondition)
		{
			TArray<AActor*> OverlappingActors;
			WinAreaBox->GetOverlappingActors(OverlappingActors, ASuperAdventureCharacter::StaticClass());

			WinCondition = (OverlappingActors.Num() == GetWorld()->GetNumPlayerControllers());
			if (WinCondition)
			{
				UE_LOG(LogTemp, Display, TEXT("Win!"));

				MulticastRPCWin();
			}
		}		
	}
}

void AWinArea::MulticastRPCWin_Implementation()
{
	OnWinCondition.Broadcast();
}

