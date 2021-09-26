// Fill out your copyright notice in the Description page of Project Settings.


#include "GunActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Sets default values
AGunActor::AGunActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGunActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunActor::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, SkeletalMesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, SkeletalMesh, TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDirection;

	bool bHitSomething = GunTrace(HitResult,ShotDirection);
	
	if(bHitSomething)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpact , HitResult.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConcreteSound, HitResult.Location);
		if(!(HitResult.GetActor())){return;}
		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
		AController* OwnerController = GetGunController();
		HitResult.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);

	}
}

bool AGunActor::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	AController* OwnerController = GetGunController();
	if(!OwnerController){return false;}

	FVector PlayerViewLocation; 
	FRotator PlayerViewRotation; 

	OwnerController->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
	ShotDirection = -PlayerViewRotation.Vector();

	FVector End = PlayerViewLocation + PlayerViewRotation.Vector() * MaxRange;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(HitResult, PlayerViewLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGunActor::GetGunController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn){return nullptr;}

	return OwnerPawn->GetController();
}
