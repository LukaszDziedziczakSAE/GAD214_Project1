// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingDoors.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASlidingDoors::ASlidingDoors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door"));
	LeftDoor->SetupAttachment(RootComp);

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door"));
	RightDoor->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ASlidingDoors::BeginPlay()
{
	Super::BeginPlay();

	DoorState = State::Closed;
	
}

// Called every frame
void ASlidingDoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (DoorMode == Mode::Manual)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s ticking"), *GetName());
	}*/

	if (DoorState == State::Opening)
	{
		if (TimeSinceStart() > MovementTime)
		{
			LeftDoor->SetRelativeLocation(FVector(OpenPosition, 0, 0));
			RightDoor->SetRelativeLocation(FVector(-OpenPosition, 0, 0));
			DoorState = State::Open;
			UE_LOG(LogTemp, Warning, TEXT("Opened %s"), *GetName());
		}
		else
		{
			float CurrentPosition = FMath::Lerp(0, OpenPosition, TimeSinceStart()/MovementTime);
			LeftDoor->SetRelativeLocation(FVector(CurrentPosition, 0, 0));
			RightDoor->SetRelativeLocation(FVector(-CurrentPosition, 0, 0));
		}
	}

	else if (DoorState == State::Closing)
	{
		if (TimeSinceStart() > MovementTime)
		{
			LeftDoor->SetRelativeLocation(FVector(0, 0, 0));
			RightDoor->SetRelativeLocation(FVector(0, 0, 0));
			DoorState = State::Closed;
			UE_LOG(LogTemp, Warning, TEXT("Closed %s"), *GetName());
		}
		else
		{
			float CurrentPosition = FMath::Lerp(OpenPosition, 0, TimeSinceStart() / MovementTime);
			LeftDoor->SetRelativeLocation(FVector(CurrentPosition, 0, 0));
			RightDoor->SetRelativeLocation(FVector(-CurrentPosition, 0, 0));
		}
	}
}

void ASlidingDoors::ToggleDoor()
{
	if (DoorMode == Mode::Manual)
	{
		if (DoorState == State::Closed)
		{
			UE_LOG(LogTemp, Warning, TEXT("Opening %s"), *GetName());
			StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			DoorState = State::Opening;
		}

		else if (DoorState == State::Open)
		{
			UE_LOG(LogTemp, Warning, TEXT("Closing %s"), *GetName());
			StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			DoorState = State::Closing;
		}
	}
	

}

float ASlidingDoors::TimeSinceStart()
{
	return UGameplayStatics::GetRealTimeSeconds(GetWorld()) - StartTime;
}

bool ASlidingDoors::IsLocked()
{
	return DoorMode == Mode::Offline || Locked;
}

bool ASlidingDoors::IsOpenable()
{
	return DoorMode != Mode::Offline && !Locked;
}

