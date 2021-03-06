// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPEV2_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

private:
	float Reach = 200.f;
    
	UPROPERTY()
	UPhysicsHandleComponent *PhysicsHandle = nullptr;
    
	UPROPERTY()
	UInputComponent *InputComponent = nullptr;

	void Grab();

	void Release();

	void FindPhysicsHandle();

	void SetupInputComponent();

	FHitResult GetFirstPhysicsBodyInReach() const;

	FVector GetPlayersReach() const;

	FVector GetPlayersWorldPos() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
