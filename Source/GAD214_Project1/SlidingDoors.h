// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "SlidingDoors.generated.h"

UENUM()
enum State
{
	Open,
	Closed,
	Opening,
	Closing
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
	UStaticMeshComponent* LeftDoor;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* RightDoor;

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
};
