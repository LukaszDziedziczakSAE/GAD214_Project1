// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "SlidingDoors.generated.h"

UENUM()
enum State
{
	Open,
	Closed,
	Opening,
	Closing,
	Locking,
	Unlocking
};

UENUM()
enum Mode
{
	Offline,
	Automatic,
	Manual
};

UCLASS()
class GAD214_PROJECT1_API ASlidingDoors : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingDoors();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBoxComponent* DoorwayArea;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* LeftDoor;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* RightDoor;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* CenterLock1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* CenterLock2;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float OpenPosition;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MovementTime;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TEnumAsByte<State> DoorState;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<Mode> DoorMode;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool Locked;

	UFUNCTION()
	void ToggleDoor();

	UPROPERTY()
	float StartTime;

	UFUNCTION()
	float TimeSinceStart();

	UFUNCTION(BlueprintPure)
	bool IsLocked();

	UFUNCTION(BlueprintPure)
	bool IsOpenable();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float LockingTime;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAudioComponent* DoorAudio;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAudioComponent* LockAudio;

	UFUNCTION()
	void PlayDoorSound(USoundBase* Sound);

	UFUNCTION()
	void PlayLockSound(USoundBase* Sound);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USoundBase* OpenSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USoundBase* CloseSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USoundBase* DoorLock;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float AutoCloseTime;

	UFUNCTION()
	void OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int CurrentlyOverlapping;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float ZeroOverlapTime;

	bool OverlapTimeCloseTime();
};
