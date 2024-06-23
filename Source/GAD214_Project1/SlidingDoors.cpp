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

	DoorwayArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Doorway Area"));
	DoorwayArea->SetupAttachment(RootComp);
	DoorwayArea->OnComponentBeginOverlap.AddDynamic(this, &ASlidingDoors::OnOverlapStart);
	DoorwayArea->OnComponentEndOverlap.AddDynamic(this, &ASlidingDoors::OnOverlapEnd);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door"));
	LeftDoor->SetupAttachment(RootComp);

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door"));
	RightDoor->SetupAttachment(RootComp);

	CenterLock1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Center Lock 1"));
	CenterLock1->SetupAttachment(LeftDoor);

	CenterLock2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Center Lock 2"));
	CenterLock2->SetupAttachment(RightDoor);

	DoorAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Door Audio"));
	DoorAudio->SetupAttachment(RootComp);

	LockAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Lock Audio"));
	LockAudio->SetupAttachment(RootComp);

	ZeroOverlapTime = 0;
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
		if (TimeSinceStart() >= MovementTime)
		{
			LeftDoor->SetRelativeLocation(FVector(OpenPosition, 0, 0));
			RightDoor->SetRelativeLocation(FVector(-OpenPosition, 0, 0));
			CenterLock1->SetRelativeLocation(FVector(OpenPosition, 13, 112.5f));
			CenterLock2->SetRelativeLocation(FVector(-OpenPosition, -3, 112.5f));
			DoorState = State::Open;
			UE_LOG(LogTemp, Warning, TEXT("Opened %s"), *GetName());
		}
		else
		{
			float CurrentPosition = FMath::Lerp(0, OpenPosition, TimeSinceStart()/MovementTime);
			LeftDoor->SetRelativeLocation(FVector(CurrentPosition, 0, 0));
			RightDoor->SetRelativeLocation(FVector(-CurrentPosition, 0, 0));
			CenterLock1->SetRelativeLocation(FVector(CurrentPosition, 13, 112.5f));
			CenterLock2->SetRelativeLocation(FVector(-CurrentPosition, -3, 112.5f));
		}
	}

	else if (DoorState == State::Closing)
	{
		if (TimeSinceStart() >= MovementTime)
		{
			LeftDoor->SetRelativeLocation(FVector(0, 0, 0));
			RightDoor->SetRelativeLocation(FVector(0, 0, 0));
			CenterLock1->SetRelativeLocation(FVector(0, 13, 112.5f));
			CenterLock2->SetRelativeLocation(FVector(0, -3, 112.5f));
			StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			DoorState = State::Locking;
			PlayLockSound(DoorLock);
		}
		else
		{
			float CurrentPosition = FMath::Lerp(OpenPosition, 0, TimeSinceStart() / MovementTime);
			LeftDoor->SetRelativeLocation(FVector(CurrentPosition, 0, 0));
			RightDoor->SetRelativeLocation(FVector(-CurrentPosition, 0, 0));
			CenterLock1->SetRelativeLocation(FVector(CurrentPosition, 13, 112.5f));
			CenterLock2->SetRelativeLocation(FVector(-CurrentPosition, -3, 112.5f));
		}
	}

	else if (DoorState == State::Unlocking)
	{
		if (TimeSinceStart() >= LockingTime)
		{
			CenterLock1->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 180, 180)));
			CenterLock2->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 180, 0)));
			StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			DoorState = State::Opening;
			PlayDoorSound(OpenSound);
		}

		else
		{
			float CurrentRotation = FMath::Lerp(0, 180, TimeSinceStart() / LockingTime);
			CenterLock1->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, CurrentRotation, 180)));
			CenterLock2->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, CurrentRotation, 0)));
		}
	}

	else if (DoorState == State::Locking)
	{
		if (TimeSinceStart() >= LockingTime)
		{
			CenterLock1->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, 180)));
			CenterLock2->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, 0)));
			DoorState = State::Closed;
			UE_LOG(LogTemp, Warning, TEXT("Closed %s"), *GetName());
		}

		else
		{
			float CurrentRotation = FMath::Lerp(180, 0, TimeSinceStart() / LockingTime);
			CenterLock1->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, CurrentRotation, 180)));
			CenterLock2->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, CurrentRotation, 0)));
		}
	}

	// Autoclose
	if (AutoCloseTime > 0)
	{
		if (CurrentlyOverlapping > 0)
		{
			ZeroOverlapTime = 0;
		}
		else if (ZeroOverlapTime <= AutoCloseTime)
		{
			ZeroOverlapTime += DeltaTime;
		}

		if (DoorState == State::Open && OverlapTimeCloseTime())
		{
			ToggleDoor();
		}
	}

	if (DoorMode == Mode::Automatic)
	{
		if (DoorState == State::Closed && CurrentlyOverlapping > 0)
		{
			ToggleDoor();
		}
	}
	
}

void ASlidingDoors::ToggleDoor()
{
	if (DoorMode != Mode::Offline)
	{
		if (DoorState == State::Closed)
		{
			UE_LOG(LogTemp, Warning, TEXT("Opening %s"), *GetName());
			StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			//DoorState = State::Opening;
			DoorState = State::Unlocking;
			PlayLockSound(DoorLock);
		}

		else if (DoorState == State::Open)
		{
			UE_LOG(LogTemp, Warning, TEXT("Closing %s"), *GetName());
			StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			DoorState = State::Closing;
			//DoorState = State::Locking;
			PlayDoorSound(CloseSound);
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

void ASlidingDoors::PlayDoorSound(USoundBase* Sound)
{
	if (DoorAudio->IsPlaying()) DoorAudio->Stop();
	DoorAudio->SetSound(Sound);
	DoorAudio->Play();
}

void ASlidingDoors::PlayLockSound(USoundBase* Sound)
{
	if (LockAudio->IsPlaying()) LockAudio->Stop();
	LockAudio->SetSound(Sound);
	LockAudio->Play();
}

void ASlidingDoors::OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	ACharacter* Character = (ACharacter*)OtherActor;
	if (Character != nullptr)
	{
		CurrentlyOverlapping++;
		UE_LOG(LogTemp, Warning, TEXT("Added Overlap %s"), *GetName());
	}
}

void ASlidingDoors::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Character = (ACharacter*)OtherActor;
	if (Character != nullptr)
	{
		CurrentlyOverlapping--;
		UE_LOG(LogTemp, Warning, TEXT("Removed Overlap %s"), *GetName());

		if (CurrentlyOverlapping < 0) CurrentlyOverlapping = 0;
	}
}

bool ASlidingDoors::OverlapTimeCloseTime()
{
	return (ZeroOverlapTime >= AutoCloseTime);
}

