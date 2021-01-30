// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena


#include "Mayhaemsters/Actors/C_PickupItem.h"
#include "Mayhaemsters/Characters/C_Ch_Player.h"
#include "Mayhaemsters/Game/C_GS_Mayhaemsters.h"
#include "Mayhaemsters/Game/C_GM_Mayhaemsters.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AC_PickupItem::AC_PickupItem() : YawOffset(40.f), ZOffset(100.f), RotTimelineSpeed(.2f), ZTimelineSpeed(.2f)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	StMesh = CreateDefaultSubobject<UStaticMeshComponent>("StMesh");
	StMesh->SetCollisionProfileName("OverlapAll");
	StMesh->SetupAttachment(SceneRoot);

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>("BoxTrigger");
	BoxTrigger->SetupAttachment(SceneRoot);
	BoxTrigger->SetGenerateOverlapEvents(true);
	BoxTrigger->SetCollisionProfileName("OverlapAllDynamic");
}

void AC_PickupItem::BeginPlay()
{
	Super::BeginPlay();

	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AC_PickupItem::OnBoxBeginOverlap);
	m_GameState = (AC_GS_Mayhaemsters*)UGameplayStatics::GetGameState(GetWorld());
	SetActorRelativeRotation(FRotator(0, 0, 90));

	if (RotCurveFloat)
	{
		StartRot = EndRot = GetActorRotation();
		StartRot.Yaw -= YawOffset;
		EndRot.Yaw += YawOffset;

		PlayTimeline(RotCurveTimeline, FName("RotTimelineProgress"), RotCurveFloat, false);
	}

	if (ZCurveFloat)
	{
		StartLoc = EndLoc = GetActorLocation();
		EndLoc.Z += ZOffset;

		PlayTimeline(ZCurveTimeline, FName("ZTimelineProgress"), ZCurveFloat, false);

	}
}

void AC_PickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AddActorWorldRotation(FRotator(0.f, RotationSpeed*DeltaTime, 0.f));
	if (RotCurveFloat) {
		RotCurveTimeline.TickTimeline(DeltaTime* RotTimelineSpeed);
	}
	if (ZCurveFloat) {
		ZCurveTimeline.TickTimeline(DeltaTime * ZTimelineSpeed);
	}
}

void AC_PickupItem::RotTimelineProgress(float Value)
{
	SetActorRotation(FMath::Lerp(StartRot, EndRot, Value));
}

void AC_PickupItem::ZTimelineProgress(float Value)
{
	SetActorLocation(FMath::Lerp(StartLoc, EndLoc, Value));
}

void AC_PickupItem::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor) && (OtherActor != this) && (OtherComp))
	{
		if (OtherActor->ActorHasTag("PlayerCharacter"))
		{
			RootComponent->SetVisibility(false, true);
			StMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			BoxTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			m_OverlappingPlayer = (AC_Ch_Player*)OtherActor;
			m_OverlappingPlayer->PickItemUp(this);
		}
	}
}

void AC_PickupItem::ResetMap()
{
	m_GameState->MapReset();
}

void AC_PickupItem::RemapControls()
{
	m_OverlappingPlayer->RandomizeButtons();
}

void AC_PickupItem::ActivateShield()
{
	m_OverlappingPlayer->bIsShieldActive = true;
	m_OverlappingPlayer->CalculateShieldTime();
}

void AC_PickupItem::DecreaseSpeed()
{
	TArray<AActor*> OtherPlayers;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AC_Ch_Player::StaticClass(), "PlayerCharacter", OtherPlayers);

	for (AActor* Player : OtherPlayers)
	{
		if (m_OverlappingPlayer && m_OverlappingPlayer != Player)
		{
			m_OtherPlayer = (AC_Ch_Player*)Player;
			m_OtherPlayer->GetCharacterMovement()->MaxWalkSpeed = m_OverlappingPlayer->ReducedSpeed;
			m_OtherPlayer->bIsSlowed = true;
			m_OtherPlayer->CalculateReductionTime();
		}
	}
}

void AC_PickupItem::IncreaseSpeed()
{
	if (m_OverlappingPlayer && !m_OverlappingPlayer->bIsBoosted)
	{
		m_OverlappingPlayer->GetCharacterMovement()->MaxWalkSpeed = m_OverlappingPlayer->BoostedSpeed;
		m_OverlappingPlayer->bIsBoosted = true;
		m_OverlappingPlayer->CalculateBoostTime();
	}
}

void AC_PickupItem::SetItemIndex(int Index)
{
	ItemIndex = Index;
}

void AC_PickupItem::PlayTimeline(struct FTimeline& Timeline, FName Function, UCurveFloat* Curve, bool PlayOnce)
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