#include "MovableActor.h"

AMovableActor::AMovableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	Transporter = CreateDefaultSubobject<UTransporterComponent>(TEXT("Transporter"));

	Point1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point 1"));
	Point1->SetupAttachment(RootComp);
	Point1->SetRelativeLocation(FVector(0, 0, 0));

	Point2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point 2"));
	Point2->SetupAttachment(RootComp);
	Point2->SetRelativeLocation(FVector(0, 0, 300));
}

void AMovableActor::BeginPlay()
{
	Super::BeginPlay();
	
	FVector StartPoint = GetActorLocation() + Point1->GetRelativeLocation();
	FVector EndPoint = GetActorLocation() + Point2->GetRelativeLocation();

	Transporter->SetPoints(StartPoint, EndPoint);
}

void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

