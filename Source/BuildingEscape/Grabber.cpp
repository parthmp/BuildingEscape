// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandler();
	FindInputComponent();
}

void UGrabber::Grab() {

	UE_LOG(LogTemp, Warning, TEXT("grabber called!"));

	FHitResult Hit = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		PhysicsHandler->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}

}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("release called!"));
	PhysicsHandler->ReleaseComponent();
}

void UGrabber::FindInputComponent() {

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("InputComponent not found for : %s"), *(GetOwner()->GetName()));
	}
}

FVector UGrabber::GetPlayerStart() {

	FVector Location;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Location, Rotation);

	return Location;
}

FVector UGrabber::GetPlayerEnd() {

	FVector Location;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Location, Rotation);

	return (Location + (Rotation.Vector() * Reach));
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {

	

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetPlayerStart(),
		GetPlayerEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* HittedActor = Hit.GetActor();

	if (HittedActor) {
		UE_LOG(LogTemp, Warning, TEXT("Hit at %s"), *(HittedActor->GetName()));
	}

	return Hit;
}

void UGrabber::FindPhysicsHandler() {
	PhysicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandler) {
		///physics component initialized.
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PhysicsComponent not found for : %s"), *(GetOwner()->GetName()));
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandler->GrabbedComponent) {

		PhysicsHandler->SetTargetLocation(GetPlayerEnd());

	}

	

}

