// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorSwitch.h"

// Sets default values
ADoorSwitch::ADoorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	Button->SetupAttachment(Mesh);

	LockedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LockedLight"));
	LockedLight->SetupAttachment(Mesh);

	OpenLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("OpenLight"));
	OpenLight->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ADoorSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocked())
	{
		OpenLight->SetVisibility(false);
		LockedLight->SetVisibility(true);
	}

	if (IsOpenable())
	{
		LockedLight->SetVisibility(false);
		OpenLight->SetVisibility(true);
	}
}

// Called every frame
void ADoorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorSwitch::Activate()
{
	if (Doors != nullptr && Doors->IsOpenable())
	{
		Doors->ToggleDoor();
	}
}

bool ADoorSwitch::IsLocked()
{
	if (Doors != nullptr)
	{
		return Doors->IsLocked();
	}
	return false;
}

bool ADoorSwitch::IsOpenable()
{
	if (Doors != nullptr)
	{
		return Doors->IsOpenable();
	}
	return false;
}

