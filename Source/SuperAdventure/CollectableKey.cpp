#include "CollectableKey.h"
#include "Net/UnrealNetwork.h"
#include "SuperAdventureCharacter.h"

ACollectableKey::ACollectableKey()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);
	Mesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComp);
	Capsule->SetIsReplicated(true);
	Capsule->SetCollisionProfileName(FName("OverlapAllDynamic"));
	Capsule->SetCapsuleHalfHeight(150.0f);
	Capsule->SetCapsuleRadius(100.0f);

	RotationSpeed = 100.0f;

	CollectAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectAudio"));
	CollectAudio->SetupAttachment(RootComp);
	CollectAudio->SetAutoActivate(false);
}

void ACollectableKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollectableKey, IsCollected);
}

void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		// Rotates the static mesh
		Mesh->AddRelativeRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

		TArray<AActor*> OverlappingActors;
		Capsule->GetOverlappingActors(OverlappingActors, ASuperAdventureCharacter::StaticClass());
		if (!OverlappingActors.IsEmpty())
		{
			if (!IsCollected)
			{
				IsCollected = true;
				OnRep_IsCollected();
			}
		}
	}
}

void ACollectableKey::OnRep_IsCollected()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Display, TEXT("OnRep_IsCollected called from Server"));
		if (IsCollected)
		{
			OnCollected.Broadcast();
		}
	}
	else 
	{
		UE_LOG(LogTemp, Display, TEXT("OnRep_IsCollected called from Client"));
	}

	Mesh->SetVisibility(!IsCollected);
	CollectAudio->Play();

	if (IsCollected)
	{
		if (KeyHolderRef)
		{
			KeyHolderRef->ActivateKeyMesh();
		}
	}
}

