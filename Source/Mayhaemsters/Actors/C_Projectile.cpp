// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#include "Mayhaemsters/Actors/C_Projectile.h"
#include "Mayhaemsters/Characters/C_Ch_Player.h"
#include "Mayhaemsters/Game/C_GI_Mayhaemsters.h"
#include "Mayhaemsters/Game/C_GS_Mayhaemsters.h"
#include "Mayhaemsters/Game/C_GM_Mayhaemsters.h"
#include "Mayhaemsters/Actors/C_Morphable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AC_Projectile::AC_Projectile() :
	ExplosionRange(1500.f),
	ControllerIndex(0),
	m_ShootingMoment(0.f),
	FlightDuration(1.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StMesh = CreateDefaultSubobject<UStaticMeshComponent>("StMesh");
	RootComponent = StMesh;
	StMesh->SetGenerateOverlapEvents(true);
	StMesh->SetNotifyRigidBodyCollision(true);
	StMesh->BodyInstance.SetCollisionProfileName("OverlapAll");

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComp");
	ProjectileComp->ProjectileGravityScale = 0;
	ProjectileComp->InitialSpeed = 2000;
	ProjectileComp->MaxSpeed = 2000;
}

// Called when the game starts or when spawned
void AC_Projectile::BeginPlay()
{
	Super::BeginPlay();

	StMesh->IgnoreActorWhenMoving(GetInstigator(), true);
	
	StMesh->OnComponentHit.AddDynamic(this, &AC_Projectile::OnCompHit);

	m_PlayerController = (APlayerController*)(GetInstigator()->GetController());
	ControllerIndex = UGameplayStatics::GetPlayerControllerID(m_PlayerController);
	Player = (AC_Ch_Player*)UGameplayStatics::GetPlayerCharacter(GetWorld(), ControllerIndex);

	m_ShootingMoment = GetWorld()->GetTimeSeconds();
}

// Called every frame
void AC_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((GetWorld()->GetTimeSeconds() - m_ShootingMoment) > FlightDuration)
	{
		Explode();
	}
}

void AC_Projectile::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor) && (OtherActor != this) && (OtherComp))
	{
		if (OtherActor->ActorHasTag("PlayerCharacter") && (OtherActor != Player))
		{
			AC_Ch_Player* HitActor = (AC_Ch_Player*)OtherActor;
			m_GameState = (AC_GS_Mayhaemsters*)UGameplayStatics::GetGameState(GetWorld());
			m_GameInstance = (UC_GI_Mayhaemsters*)UGameplayStatics::GetGameInstance(GetWorld());

			for (int32 i = 0; i < 4; i++)
			{
				if (Player == (AC_Ch_Player*)UGameplayStatics::GetPlayerCharacter(this, i))
				{
					if (!HitActor->bIsDead && !HitActor->bIsShieldActive)
					{
						HitActor->Die();
					}
				}
			}
			m_GameState->RoundOver();
		}

		Explode();
	}
}

bool AC_Projectile::SearchForMorphables()
{
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation();
	float TraceRadius = ExplosionRange;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	IgnoredActors.Add(Player);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToHit;
	ObjectTypesToHit.Add(EObjectTypeQuery::ObjectTypeQuery2);

	bool HitFound = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			TraceStart,
			TraceEnd,
			TraceRadius,
			ObjectTypesToHit,
			false,
			IgnoredActors,
			EDrawDebugTrace::None,
			m_OutHits,
			true,
			FLinearColor::Red,
			FLinearColor::Yellow,
			5.f
		);

	if (HitFound)
	{
		return true;
	}

	return false;
}

void AC_Projectile::Explode()
{
	if (SearchForMorphables())
	{
		for (FHitResult Hit : m_OutHits)
		{
			if (Hit.Actor->ActorHasTag("Morphable"))
			{
				AC_Morphable* HitMorphable = (AC_Morphable*)Hit.Actor.Get(false);

				HitMorphable->MorphMesh();
			}
		}
	}

	Destroy();
}
