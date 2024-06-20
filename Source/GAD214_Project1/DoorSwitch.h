// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "SlidingDoors.h"
#include "DoorSwitch.generated.h"

UCLASS()
class GAD214_PROJECT1_API ADoorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorSwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Button;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UPointLightComponent* LockedLight;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UPointLightComponent* OpenLight;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ASlidingDoors* Doors;

	UFUNCTION(BlueprintCallable)
	void Activate();

	UFUNCTION(BlueprintPure)
	bool IsLocked();

	UFUNCTION(BlueprintPure)
	bool IsOpenable();
};
