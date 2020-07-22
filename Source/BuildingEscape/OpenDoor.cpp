// Fill out your copyright notice in the Description page of Project Settings.



#include "OpenDoor.h"
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
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor() {

	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor() {

	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if (GetTotalMass() > 40.0f) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay) {
		CloseDoor();
	}
	

}

float UOpenDoor::GetTotalMass() {
	
	float mass = 0.0f;

	if (PressurePlate != nullptr) {

		TArray<AActor*> OverlappingActors;

		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		for (const auto* Actor : OverlappingActors) {
			mass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}

	return mass;

}

