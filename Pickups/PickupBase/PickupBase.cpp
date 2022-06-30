#include "Pickups/PickupBase/PickupBase.h"
#include "Components/BoxComponent.h"

// Sets default values
APickupBase::APickupBase() : BoxSize(FVector(14.F))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Pickup Root Component"));

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));

	PickupMesh->SetupAttachment(PickupRootComponent);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupMesh->SetEnableGravity(true);
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->SetCastShadow(false);

	PickupBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Pickup Sphere"));

	PickupBox->SetupAttachment(PickupMesh);
	PickupBox->SetBoxExtent(BoxSize);
	PickupBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupBox->SetCollisionObjectType(ECC_Visibility);

	SetRootComponent(PickupRootComponent);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupBase::InteractWithObject_Implementation()
{
	Destroy();
}

void APickupBase::InteractableFound_Implementation()
{
	
}

