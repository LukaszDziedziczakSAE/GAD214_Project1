// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorSwitch.h"
#include "Kismet/GameplayStatics.h"

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

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Lock Audio"));
	Audio->SetupAttachment(Mesh);
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

	switch (ButtonState)
	{
	case PressingIn:
		if (TimeSinceStart() < MovementTime)
		{
			SetButtonPosition(FMath::Lerp(OutY, InY, TimeSinceStart() / MovementTime));
		}
		else
		{
			SetButtonPosition(InY);
			ButtonState = PressingOut;
			StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

			Doors->ToggleDoor();
		}
		break;

	case PressingOut:
		if (TimeSinceStart() < MovementTime)
		{
			SetButtonPosition(FMath::Lerp(InY, OutY, TimeSinceStart() / MovementTime));
		}
		else
		{
			SetButtonPosition(OutY);
			ButtonState = Standby;
		}
		break;
	}

}

void ADoorSwitch::Activate()
{
	if (Doors != nullptr && Doors->IsOpenable() && ButtonState == EButtonState::Standby)
	{

		PlaySound(ButtonPressSound);
		StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		ButtonState = EButtonState::PressingIn;
		//Doors->ToggleDoor();
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

void ADoorSwitch::PlaySound(USoundBase* Sound)
{
	if (Audio->IsPlaying()) Audio->Stop();
	Audio->SetSound(Sound);
	Audio->Play();
}

float ADoorSwitch::TimeSinceStart()
{
	return UGameplayStatics::GetRealTimeSeconds(GetWorld()) - StartTime;
}

void ADoorSwitch::SetButtonPosition(float Position)
{
	FVector ButtonPosition = Button->GetRelativeLocation();
	ButtonPosition.Y = Position;
	Button->SetRelativeLocation(ButtonPosition);
}

