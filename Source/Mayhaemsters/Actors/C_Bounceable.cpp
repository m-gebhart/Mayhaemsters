// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena


#include "Mayhaemsters/Actors/C_Bounceable.h"
#include "Mayhaemsters/Characters/C_Ch_Player.h"
#include "Components/SkeletalMeshComponent.h" 
#include "Components/StaticMeshComponent.h" 
#include "Kismet/KismetMathLibrary.h"

AC_Bounceable::AC_Bounceable() : BounceFactor(1.f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AC_Bounceable::BeginPlay()
{
	Super::BeginPlay();
	if (GetComponentByClass(USkeletalMeshComponent::StaticClass()))
		((USkeletalMeshComponent*)(GetComponentByClass(USkeletalMeshComponent::StaticClass())))->OnComponentHit.AddDynamic(this, &AC_Bounceable::OnHitBounce);
	if (GetComponentByClass(UStaticMeshComponent::StaticClass()))
		((UStaticMeshComponent*)(GetComponentByClass(UStaticMeshComponent::StaticClass())))->OnComponentHit.AddDynamic(this, &AC_Bounceable::OnHitBounce);
}

void AC_Bounceable::Tick(float DeltaTime)
{}

void AC_Bounceable::OnHitBounce(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("PlayerCharacter"))
	{
		((AC_Ch_Player*)OtherActor)->SetBounceTimer(((AC_Ch_Player*)OtherActor)->BounceStunDuration);

		FVector ForceVectorPlayer = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), OtherActor->GetActorLocation());
		ForceVectorPlayer.Z = 0.f;
		((ACharacter*)OtherActor)->LaunchCharacter(ForceVectorPlayer * ((AC_Ch_Player*)OtherActor)->BasicObjectBounciness*BounceFactor, false, false);
	}
}

void AC_Bounceable::PlayTimeline(struct FTimeline& Timeline, FName Function, UCurveFloat* Curve, bool PlayOnce)
{
	FOnTimelineFloat TimelineBind;
	TimelineBind.BindUFunction(this, Function);
	Timeline.AddInterpFloat(Curve, TimelineBind);
	if (PlayOnce)
	{
		Timeline.SetLooping(false);
	}
	else
	{
		Timeline.SetLooping(true);
	}

	Timeline.PlayFromStart();
}
