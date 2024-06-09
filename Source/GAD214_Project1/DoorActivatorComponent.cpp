// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActivatorComponent.h"

// Sets default values for this component's properties
UDoorActivatorComponent::UDoorActivatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorActivatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	CameraComp = GetOwner()->FindComponentByClass<UCameraComponent>();
	//UE_LOG(LogTemp, Warning, TEXT("Found camera"));
}


// Called every frame
void UDoorActivatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (CameraComp != nullptr)
	{
		FHitResult Hit;

		FVector StartLocation = CameraComp->GetComponentLocation();
		FVector EndLocation = StartLocation + (CameraComp->GetForwardVector() * RayLength);

		FCollisionQueryParams CollisionParameters;
		CollisionParameters.AddIgnoredActor(GetOwner());

		if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, CollisionParameters))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Hit.GetActor()->GetName());
			ADoorSwitch* Switch = Cast<ADoorSwitch>(Hit.GetActor());
			if (Switch != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Door Switch confirmed"));
				DoorSwitch = Switch;
			}
			else if (DoorSwitch != nullptr)
			{
				DoorSwitch = nullptr;
			}
		}
		else if (DoorSwitch != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("No hit"));
			DoorSwitch = nullptr;
		}
	}
}

void UDoorActivatorComponent::MouseClicked()
{	
	if (DoorSwitch != nullptr)
	{
		DoorSwitch->Activate();
		//UE_LOG(LogTemp, Warning, TEXT("Door Switch Activated"));
	}
}

