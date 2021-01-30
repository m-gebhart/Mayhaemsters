// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena


#include "Mayhaemsters/Actors/C_WaterActor.h"
#include "Mayhaemsters/Characters/C_Ch_Player.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathVectorCommon.h" 
#include "Components/StaticMeshComponent.h" 
#include "Components/TimelineComponent.h"
#include "Engine/World.h"

AC_WaterActor::AC_WaterActor() : 
	zPosOffset(50.f),
	rollRotOffset(10.f),
	pitchRotOffset(20.f),
	ZTimelineSpeed(.4f),
	RollTimelineSpeed(.3f),
	PitchTimelineSpeed(.2f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AC_WaterActor::BeginPlay()
{
	Super::BeginPlay();

	TargetLoc = OriginLoc = GetActorLocation();
	TargetLoc.Z += zPosOffset;

	if (GetComponentByClass(UStaticMeshComponent::StaticClass()) && WaterObjectType == EWaterObjectType::Barrel) 
	{
		((UStaticMeshComponent*)(GetComponentByClass(UStaticMeshComponent::StaticClass())))->OnComponentHit.AddDynamic(this, &AC_WaterActor::OnHitKill);
		UE_LOG(LogTemp, Warning, TEXT("done"));
	}

	if (ZPosCurveFloat && zPosOffset != 0.f)
	{
		PlayTimeline(zPosCurveTimeline, FName("ZPosTimelineProgress"), ZPosCurveFloat, false);
	}

	if (RollRotCurveFloat && rollRotOffset != 0.f)
	{
		PlayTimeline(rollRotCurveTimeline, FName("RollRotTimelineProgress"), ZPosCurveFloat, false);
	}

	if (PitchRotCurveFloat && pitchRotOffset != 0.f)
	{
		PlayTimeline(pitchRotCurveTimeline, FName("PitchRotTimelineProgress"), ZPosCurveFloat, false);
	}
}

void AC_WaterActor::ZPosTimelineProgress(float Value)
{
	SetActorLocation(FMath::Lerp(OriginLoc, TargetLoc, Value));
}

void AC_WaterActor::RollRotTimelineProgress(float Value)
{
	SetActorRotation(FMath::Lerp(FRotator(GetActorRotation().Pitch,GetActorRotation().Yaw, 0-rollRotOffset), FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, 0+rollRotOffset), Value));
}

void AC_WaterActor::PitchRotTimelineProgress(float Value)
{
	SetActorRotation(FMath::Lerp(FRotator(0 - pitchRotOffset, GetActorRotation().Yaw, GetActorRotation().Roll), FRotator(0 + pitchRotOffset, GetActorRotation().Yaw, GetActorRotation().Roll), Value));
}

void AC_WaterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ZPosCurveFloat)
	{
		zPosCurveTimeline.TickTimeline(DeltaTime*ZTimelineSpeed);
	}

	if (RollRotCurveFloat)
	{
		rollRotCurveTimeline.TickTimeline(DeltaTime * RollTimelineSpeed);
	}

	if (PitchRotCurveFloat)
	{
		pitchRotCurveTimeline.TickTimeline(DeltaTime * PitchTimelineSpeed);
	}
}

void AC_WaterActor::OnHitKill(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("PlayerCharacter")) 
	{
		FTransform SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation(), FVector(3.f, 3.f, 3.f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, SpawnTransform, true, EPSCPoolMethod::None, true);

		if (!((AC_Ch_Player*)OtherActor)->bIsShieldActive)
		{
			((AC_Ch_Player*)OtherActor)->SelfDestruct();
		}

		Destroy();
	}
}
