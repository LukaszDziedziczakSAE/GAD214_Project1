// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "DoorSwitch.h"
#include "DoorActivatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAD214_PROJECT1_API UDoorActivatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorActivatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere)
	float RayLength;

	UPROPERTY()
	ADoorSwitch* DoorSwitch;

	UFUNCTION(BlueprintCallable)
	void MouseClicked();
};
