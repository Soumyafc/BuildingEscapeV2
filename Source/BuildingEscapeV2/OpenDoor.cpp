// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	FindpressurePlate();
	FindAudioComponent();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindpressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set!"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() >= MassToOpenDoors)
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator OpenDoor(0.f, TargetYaw, 0.f);
	OpenDoor.Yaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, 60);
	GetOwner()->SetActorRotation(OpenDoor);

	CloseDoorSound = false;
	if (!AudioComponent)
	{
		return;
	}
	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator OpenDoor(0.f, TargetYaw, 0.f);
	OpenDoor.Yaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, 80);
	GetOwner()->SetActorRotation(OpenDoor);

	OpenDoorSound = false;
	if (!AudioComponent)
	{
		return;
	}
	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		CloseDoorSound = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find All Overlapping Actors.
	TArray<AActor *> OverlappingActors;
	if (!PressurePlate)
	{
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Add Up Their Masses.

	for (AActor *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		
	}

	return TotalMass;
}