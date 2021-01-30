// Fill out your copyright notice in the Description page of Project Settings

#include "Mayhaemsters/Characters/C_Ch_Player.h"
#include "Mayhaemsters/Actors/C_Projectile.h"
#include "Mayhaemsters/Game/C_GS_Mayhaemsters.h"
#include "Mayhaemsters/Game/C_GI_Mayhaemsters.h"
#include "Mayhaemsters/Game/C_GM_Mayhaemsters.h"
#include "Mayhaemsters/Actors/C_PickupItem.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Containers/Array.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Animation/WidgetAnimation.h"

AC_Ch_Player::AC_Ch_Player() :
	CharacterName(""),
	PlayerIndex(-1),
	AccelerationValue(.5f),
	DecelerationValue(.88f),
	YawRotationIntensity(2.f),
	RollRotationIntensity(1.f),
	RollRotationLimit(90.f),
	RollRotationBackIntensity(1.f),
	PitchRotationIntensity(1.f),
	PitchRotationLimit(15.f),
	PitchRotationAccelerationLimit(35.f),
	PitchRotationBackIntensity(1.f),
	BounceStunDuration(.35f),
	PlayerBounciness(1000.f),
	DashBounciness(1600.f),
	BasicObjectBounciness(600.f),
	AttackCooldown(2.f),
	DashBoolDuration(.6f),
	bIsDead(false),
	bCanShoot(true),
	bCanDash(true),
	bIsDashing(false),
	bInAcceleration(false),
	Score(0),
	bIsBoosted(false),
	bIsSlowed(false),
	bIsShieldActive(false),
	bGraveGrounded(false),
	PosSelfDestruct(0),
	PosDash(1),
	PosAttack(2),
	PosItem(3),
	NegativeInputOn(-1),
	DefaultSpeed(600.f),
	BoostedSpeed(1200.f),
	ReducedSpeed(400.f),
	SpeedBoostDuration(5.f),
	SpeedReductionDuration(5.f),
	m_SpeedBoostMoment(0.f),
	m_SpeedReductionMoment(0.f),
	ShieldDuration(6.f),
	m_currentYawRotation(0.f),
	bUnifierActive(false),
	bJammerActive(false),
	bIsBeingPushed(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	ABXYAbilities = { "Attack", "Dash", "UseItem", "SelfDestruct" };
	AttackSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackSoundComponent"));
	DashSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DashSoundComponent"));
	CollectSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectSoundComponent"));
	BallSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BallSoundComponent"));
	RemapSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("RemapSoundComponent"));
	DeathSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathSoundComponent"));
	SpeedUpSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SpeedUpSoundComponent"));
	SpeedDownSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SpeedDownSoundComponent"));

	m_SkelMesh = (USkeletalMeshComponent*)(GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	m_ChosenJammedButtons.Init(0, 2);
}

void AC_Ch_Player::BeginPlay()
{
	Super::BeginPlay();
	CharacterName = GetCharacterName(),

	this->Tags.AddUnique("PlayerCharacter");
	m_Movement = GetCharacterMovement();
	m_Movement->MaxWalkSpeed = DefaultSpeed;
	m_Movement->MaxAcceleration *= AccelerationValue;
	m_Movement->bUseSeparateBrakingFriction = true;
	m_Movement->BrakingFriction *= DecelerationValue;
	m_Movement->RotationRate.Yaw *= YawRotationIntensity;
	m_GameState = (AC_GS_Mayhaemsters*)UGameplayStatics::GetGameState(GetWorld());
	m_GameState->AllPlayers.AddUnique(this);

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AC_Ch_Player::OnHit);

	if (AttackSoundComponent && AttackSoundCue)
		AttackSoundComponent->SetSound(AttackSoundCue);
	if (DashSoundComponent && DashSoundCue)
		DashSoundComponent->SetSound(DashSoundCue);
	if (CollectSoundComponent && CollectSoundCue)
		CollectSoundComponent->SetSound(CollectSoundCue);
	if (BallSoundComponent && BallSoundCue)
		BallSoundComponent->SetSound(BallSoundCue);
	if (RemapSoundComponent && RemapSoundCue)
		RemapSoundComponent->SetSound(RemapSoundCue);
	if (ResetMapSoundComponent && ResetMapSoundCue)
		ResetMapSoundComponent->SetSound(ResetMapSoundCue);
	if (DeathSoundComponent && DeathSoundCue)
		DeathSoundComponent->SetSound(DeathSoundCue);
	if (SpeedUpSoundComponent && SpeedUpSoundCue)
		SpeedUpSoundComponent->SetSound(SpeedUpSoundCue);
	if (SpeedDownSoundComponent && SpeedDownSoundCue)
		SpeedDownSoundComponent->SetSound(SpeedDownSoundCue);

	FTimerHandle RelativeLocTimer;
	GetWorld()->GetTimerManager().SetTimer(RelativeLocTimer, this, &AC_Ch_Player::OnCountDownEnd, ((AC_GM_Mayhaemsters*)UGameplayStatics::GetGameMode(GetWorld()))->CountdownTime, false);
}

void AC_Ch_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDead) {
		if (m_GameState->bGameRunning)
		{
			if (((GetWorld()->GetTimeSeconds() - m_ShootingMoment) > AttackCooldown) && !bCanShoot)
				bCanShoot = true;

			if (((GetWorld()->GetTimeSeconds() - m_DashingMoment) > DashCooldown) && !bCanDash)
				bCanDash = true;

			if (((GetWorld()->GetTimeSeconds() - m_ShieldMoment) > ShieldDuration) && bIsShieldActive)
				bIsShieldActive = false;

			if (((GetWorld()->GetTimeSeconds() - m_SpeedBoostMoment) > SpeedBoostDuration) && bIsBoosted)
			{
				GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
				bIsBoosted = false;
			}

			if (((GetWorld()->GetTimeSeconds() - m_SpeedReductionMoment) > SpeedReductionDuration) && bIsSlowed)
			{
				GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
				bIsSlowed = false;
			}
		}

		if (PlayerIndex == -1)
		{
			SetCharacterIndex();
		}
	}
}

void AC_Ch_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &AC_Ch_Player::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &AC_Ch_Player::MoveY);
	PlayerInputComponent->BindAction("AbilityA", IE_Pressed, this, &AC_Ch_Player::AbilityA);
	PlayerInputComponent->BindAction("AbilityB", IE_Pressed, this, &AC_Ch_Player::AbilityB);
	PlayerInputComponent->BindAction("AbilityY", IE_Pressed, this, &AC_Ch_Player::AbilityY);
	PlayerInputComponent->BindAction("AbilityX", IE_Pressed, this, &AC_Ch_Player::AbilityX);
}

void AC_Ch_Player::OnCountDownEnd()
{
	MeshRelativeLoc = m_SkelMesh->GetRelativeLocation();
}

FString AC_Ch_Player::GetCharacterName()
{
	FString PlayerName = GetName();
	UC_GI_Mayhaemsters* m_GameInstance = (UC_GI_Mayhaemsters*)UGameplayStatics::GetGameInstance(GetWorld());

	if (m_GameInstance)
	{
		if (PlayerName == "BP_Ch_Player_C_0")
			return m_GameInstance->Name_P1;
		else if (PlayerName == "BP_Ch_Player_C_1")
			return m_GameInstance->Name_P2;
		else if (PlayerName == "BP_Ch_Player_C_2")
			return m_GameInstance->Name_P3;
		else if (PlayerName == "BP_Ch_Player_C_3")
			return m_GameInstance->Name_P4;
	}
	return GetName();
}

void AC_Ch_Player::MoveX(float Value)
{
	if (!bIsDead && m_GameState->bGameRunning)
	{
		if (!bIsBeingPushed)
		{
			m_MoveXValue = Value;

			if (Value != 0)
			{
				RollRotation();
				PitchRotation();
				AddMovementInput(FVector(m_MoveXValue, .0f, .0f));
			}
			if (m_MoveYValue == 0 && m_MoveXValue == 0 && GetActorRotation().Roll == GetActorRotation().Pitch == 0.f)
			{
				RollBackRotation();
				PitchBackRotation();
			}
		}
	}
}

void AC_Ch_Player::MoveY(float Value)
{
	if (!bIsDead && m_GameState->bGameRunning)
	{
		if (!bIsBeingPushed)
		{
			m_MoveYValue = Value;

			if (Value != 0)
			{
				RollRotation();
				PitchRotation();
				AddMovementInput(FVector(.0f, m_MoveYValue, .0f));
			}
			if (m_MoveYValue == 0 && m_MoveXValue == 0 && GetActorRotation().Roll == GetActorRotation().Pitch == 0.f)
			{
				RollBackRotation();
				PitchBackRotation();
			}
		}
	}
}

void AC_Ch_Player::RollRotation()
{

	//Rotation around Roll-/X-Axis
	m_previousYawRotation = m_currentYawRotation;
	m_currentYawRotation = GetCharacterMovement()->GetLastUpdateRotation().Yaw;
	if (FMath::RoundHalfToEven(m_previousYawRotation) == FMath::RoundHalfToEven(m_currentYawRotation))
	{
		RollBackRotation();
	}
	//Getting Roll Direction
	else if (-179.9f < m_currentYawRotation && m_currentYawRotation < 179.9f)
	{
		if (m_previousYawRotation > m_currentYawRotation && GetCharacterMovement()->GetLastUpdateRotation().Roll > -RollRotationLimit)
			//Counter-Clockwise
			AddActorLocalRotation(FQuat(FRotator(0.f, 0.f, -RollRotationIntensity)), false, 0, ETeleportType::None);
		else if (m_previousYawRotation < m_currentYawRotation && GetCharacterMovement()->GetLastUpdateRotation().Roll < RollRotationLimit)
			//Clockwise
			AddActorLocalRotation(FQuat(FRotator(0.f, 0.f, RollRotationIntensity)), false, 0, ETeleportType::None);
	}

	m_SkelMesh->SetRelativeLocation(MeshRelativeLoc);
}

void AC_Ch_Player::RollBackRotation()
{
	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, FMath::FInterpTo(GetActorRotation().Roll, 0.f, GetWorld()->GetDeltaSeconds(), RollRotationBackIntensity)));
}

void AC_Ch_Player::PitchRotation()
{
	if (!bInAcceleration)
	{
		SetActorRotation(FRotator(FMath::FInterpTo(GetActorRotation().Pitch, PitchRotationAccelerationLimit, GetWorld()->GetDeltaSeconds(), PitchRotationIntensity), GetActorRotation().Yaw, GetActorRotation().Roll));
		if (GetActorRotation().Pitch > PitchRotationAccelerationLimit - PitchRotationIntensity)
			bInAcceleration = true;
	}
	else
	{
		SetActorRotation(FRotator(FMath::FInterpTo(GetActorRotation().Pitch, PitchRotationLimit, GetWorld()->GetDeltaSeconds(), PitchRotationBackIntensity * 1.5f), GetActorRotation().Yaw, GetActorRotation().Roll));
	}
}

void AC_Ch_Player::PitchBackRotation()
{
	if (bInAcceleration) {
		bInAcceleration = false;
	}
	SetActorRotation(FRotator(FMath::FInterpTo(GetActorRotation().Pitch, 0.f, GetWorld()->GetDeltaSeconds(), PitchRotationBackIntensity), GetActorRotation().Yaw, GetActorRotation().Roll));
}

void AC_Ch_Player::AbilityA()
{
	if (!bIsDead && m_GameState->bGameRunning)
	{
		if (bUnifierActive)
		{
			UnifierAction();
		}
		else
		{
			if (!bJammerActive || (bJammerActive && !m_ChosenJammedButtons.Contains(0)))
			{
				switch (InputStates)
				{
				case EInputStates::S0:
					Attack();
					break;
				case EInputStates::S1:
					Dash();
					break;
				case EInputStates::S2:
					SelfDestruct();
					break;
				case EInputStates::S3:
					UseItem();
					break;
				default:
					break;
				}
			}
		}
	}
}

void AC_Ch_Player::AbilityB()
{
	if (!bIsDead && m_GameState->bGameRunning)
	{
		if (bUnifierActive)
		{
			UnifierAction();
		}
		else
		{
			if (!bJammerActive || (bJammerActive && !m_ChosenJammedButtons.Contains(1)))
			{
				switch (InputStates)
				{
				case EInputStates::S0:
					Dash();
					break;
				case EInputStates::S1:
					Attack();
					break;
				case EInputStates::S2:
					UseItem();
					break;
				case EInputStates::S3:
					SelfDestruct();
					break;
				default:
					break;
				}
			}
		}
	}
}

void AC_Ch_Player::AbilityY()
{
	if (!bIsDead && m_GameState->bGameRunning)
	{
		if (bUnifierActive)
		{
			UnifierAction();
		}
		else
		{
			if (!bJammerActive || (bJammerActive && !m_ChosenJammedButtons.Contains(2)))
			{
				switch (InputStates)
				{
				case EInputStates::S0:
					SelfDestruct();
					break;
				case EInputStates::S1:
					UseItem();
					break;
				case EInputStates::S2:
					Attack();
					break;
				case EInputStates::S3:
					Dash();
					break;
				default:
					break;
				}
			}
		}
	}
}

void AC_Ch_Player::AbilityX()
{
	if (!bIsDead && m_GameState->bGameRunning)
	{
		if (bUnifierActive)
		{
			UnifierAction();
		}
		else
		{
			if (!bJammerActive || (bJammerActive && !m_ChosenJammedButtons.Contains(3)))
			{
				switch (InputStates)
				{
				case EInputStates::S0:
					UseItem();
					break;
				case EInputStates::S1:
					SelfDestruct();
					break;
				case EInputStates::S2:
					Dash();
					break;
				case EInputStates::S3:
					Attack();
					break;
				default:
					break;
				}
			}
		}
	}
}

void AC_Ch_Player::SetCharacterIndex()
{
	if (GetController())
	{
		APlayerController* m_PlayerController = (APlayerController*)(GetController());
		PlayerIndex = UGameplayStatics::GetPlayerControllerID(m_PlayerController);
	}
}

void AC_Ch_Player::RandomizeButtons()
{
	if (!bIsDead && m_GameState->bGameRunning)
	{
		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AC_Ch_Player::StaticClass(), "PlayerCharacter", Players);

		for (AActor* Player : Players)
		{
			if (Player && Player != this)
			{
				m_CurrentPlayer = (AC_Ch_Player*)Player;
				m_CurrentPlayer->InputStates = (EInputStates)FMath::RandRange(0, 3);
				m_CurrentPlayer->CheckAbilities();
			}
		}
		if (RemapSoundComponent && RemapSoundCue)
			RemapSoundComponent->Play(0.f);
	}
}

void AC_Ch_Player::Attack()
{
	SetNegativeInputFeedbackOn(-1);
	if (Projectile && bCanShoot)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		SpawnParams.Owner = this;
		FVector ProjectileInitLoc = (m_SkelMesh->GetSocketLocation("ProjSocket"));
		FRotator ProjectileInitRot = GetCapsuleComponent()->GetForwardVector().Rotation();
		ProjectileInitRot.Pitch = 0.f;
		AC_Projectile* NewProjectile = GetWorld()->SpawnActor<AC_Projectile>(Projectile, ProjectileInitLoc, ProjectileInitRot, SpawnParams);
		NewProjectile->SetActorScale3D(FVector(2.5f, 2.5f, 2.5f));

		MoveIgnoreActorAdd(NewProjectile);

		if (AttackSoundComponent && AttackSoundCue)
		{
			AttackSoundComponent->Play(0.f);
		}

		m_ShootingMoment = GetWorld()->GetTimeSeconds();
		bCanShoot = false;
	}
	else
	{
		SetNegativeInputFeedbackOn(PosAttack);
	}
}

void AC_Ch_Player::Dash()
{
	SetNegativeInputFeedbackOn(-1);
	if (bCanDash)
	{
		bIsDashing = true;

		ShowDashGhost(PlayerIndex);

		GetWorld()->GetTimerManager().SetTimer(DashTimeHandle, this, &AC_Ch_Player::ResetDashBool, DashBoolDuration, false);
		if ((m_MoveXValue == 0) && (m_MoveYValue == 0))
		{
			FVector DashVelocity = ((GetCapsuleComponent()->GetForwardVector()) * FVector(DashDistance * 10.f));
			LaunchCharacter(DashVelocity, false, false);
		}
		else
		{
			FVector DashVelocity = (FVector(m_MoveXValue, m_MoveYValue, 0) * FVector(DashDistance * 10.f));
			LaunchCharacter(DashVelocity, false, false);
		}
		if (DashSoundComponent && DashSoundCue)
			DashSoundComponent->Play(0.f);
		m_DashingMoment = GetWorld()->GetTimeSeconds();
		bCanDash = false;
	}
	else
	{
		SetNegativeInputFeedbackOn(PosDash);
	}
}

void AC_Ch_Player::ResetDashBool()
{
	bIsDashing = false;
	GetWorldTimerManager().ClearTimer(DashTimeHandle);
}

void AC_Ch_Player::SelfDestruct()
{
	Die();
	m_GameState->RoundOver();
}

void AC_Ch_Player::MystifyButtons()
{
	if (!bIsDead && m_GameState->bGameRunning)
	{
		bUnifierActive = false;
		bJammerActive = false;

		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AC_Ch_Player::StaticClass(), "PlayerCharacter", Players);

		for (AActor* Player : Players)
		{
			if (Player && Player != this)
			{
				m_CurrentPlayer = (AC_Ch_Player*)Player;
				
				APlayerController* m_PlayerController = (APlayerController*)(m_CurrentPlayer->GetController());
				int32 PlayerID = UGameplayStatics::GetPlayerControllerID(m_PlayerController);

				m_CurrentPlayer->bMysteryActive = true;

				ActivateMystery(PlayerID);

				m_CurrentPlayer->CheckAbilities();
			}
		}
	}
}

void AC_Ch_Player::UnifyButtons()
{
	if (!bIsDead && m_GameState->bGameRunning)
	{
		bMysteryActive = false;
		bJammerActive = false;

		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AC_Ch_Player::StaticClass(), "PlayerCharacter", Players);

		for (AActor* Player : Players)
		{
			if (Player && Player != this)
			{
				m_CurrentPlayer = (AC_Ch_Player*)Player;

				APlayerController* m_PlayerController = (APlayerController*)(m_CurrentPlayer->GetController());
				int32 PlayerID = UGameplayStatics::GetPlayerControllerID(m_PlayerController);

				m_CurrentPlayer->m_ChosenUnifiedButton = FMath::RandRange(0, 3);
				m_CurrentPlayer->bUnifierActive = true;

				UpdateUnifierUI(PlayerID, m_CurrentPlayer->m_ChosenUnifiedButton);

				m_CurrentPlayer->CheckAbilities();
			}
		}
	}
}

void AC_Ch_Player::UnifierAction()
{
	switch (m_ChosenUnifiedButton)
	{
	case 0:
		Attack();
		break;
	case 1:
		Dash();
		break;
	case 2:
		SelfDestruct();
		break;
	case 3:
		UseItem();
		break;
	}
}

void AC_Ch_Player::JamButtons()
{
	if (!bIsDead && m_GameState->bGameRunning)
	{
		bMysteryActive = false;
		bUnifierActive = false;

		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AC_Ch_Player::StaticClass(), "PlayerCharacter", Players);

		for (AActor* Player : Players)
		{
			if (Player && Player != this)
			{
				m_CurrentPlayer = (AC_Ch_Player*)Player;

				APlayerController* m_PlayerController = (APlayerController*)(m_CurrentPlayer->GetController());
				int32 PlayerID = UGameplayStatics::GetPlayerControllerID(m_PlayerController);

				m_CurrentPlayer->m_ChosenJammedButtons[0] = FMath::RandRange(0, 3);
				m_CurrentPlayer->m_ChosenJammedButtons[1] = FMath::RandRange(0, 3);

				if ((m_CurrentPlayer->m_ChosenJammedButtons[0] == m_CurrentPlayer->m_ChosenJammedButtons[1]))
				{
					if ((m_CurrentPlayer->m_ChosenJammedButtons[1] == 3))
					{
						m_CurrentPlayer->m_ChosenJammedButtons[1] = FMath::RandRange(0, 2);
					}
					else
					{
						m_CurrentPlayer->m_ChosenJammedButtons[1] = m_CurrentPlayer->m_ChosenJammedButtons[1] + 1;
					}
				}

				m_CurrentPlayer->bJammerActive = true;

				UpdateJammerUI(PlayerID, m_CurrentPlayer->m_ChosenJammedButtons);
			}
		}
	}
}

void AC_Ch_Player::DeactivateMystery()
{
	bMysteryActive = false;
}

void AC_Ch_Player::DeactivateUnifier()
{
	bUnifierActive = false;
}

void AC_Ch_Player::DeactivateJammer()
{
	bJammerActive = false;
}

void AC_Ch_Player::UseItem()
{
	SetNegativeInputFeedbackOn(-1);
	if (Inventory.Num() > 0)
	{
		int32 ItemIndex = Inventory.Num() - 1;
		AC_PickupItem* ItemToUse = Inventory[ItemIndex];

		if (Inventory.Num() > 0)
		{
			switch (ItemToUse->ItemType)
			{
			case EPickupItemType::MapResetter:
				if (!(m_GameState->bIsResettingMap))
				{
					m_GameState->bIsResettingMap = true;
					ItemToUse->ResetMap();
				}
				else
				{
					return;
				}
				break;
			case EPickupItemType::Remapper:
				ItemToUse->RemapControls();
				if (RemapSoundComponent && RemapSoundCue)
					RemapSoundComponent->Play(0.f);
				break;
			case EPickupItemType::Shield:
				ItemToUse->ActivateShield();
				if (BallSoundComponent && BallSoundCue)
					BallSoundComponent->Play(0.f);
				break;
			case EPickupItemType::SpeedDown:
				ItemToUse->DecreaseSpeed();
				break;
			case EPickupItemType::SpeedUp:
				ItemToUse->IncreaseSpeed();
				break;
			case EPickupItemType::Mystery:
				MystifyButtons();
				break;
			case EPickupItemType::Unifier:
				UnifyButtons();
				break;
			case EPickupItemType::Jammer:
				JamButtons();
				break;
			}
		}

		Inventory.RemoveAt(ItemIndex);
		ItemToUse->Destroy();
	}
	else
	{
		SetNegativeInputFeedbackOn(PosItem);
	}
}

void AC_Ch_Player::PickItemUp(AC_PickupItem* PickedItem)
{
	if (Inventory.Num() > 0)
	{
		Inventory[0]->Destroy();
		Inventory.RemoveAt(0);
	}

	Inventory.Insert(PickedItem, 0);

	if (CollectSoundComponent && CollectSoundCue)
		CollectSoundComponent->Play(0.f);
}

void AC_Ch_Player::CalculateBoostTime()
{
	m_SpeedBoostMoment = GetWorld()->GetTimeSeconds();
	if (SpeedUpSoundComponent && SpeedUpSoundCue)
		SpeedUpSoundComponent->Play(0.f);
}

void AC_Ch_Player::CalculateReductionTime()
{
	m_SpeedReductionMoment = GetWorld()->GetTimeSeconds();
	if (SpeedDownSoundComponent && SpeedDownSoundCue)
		SpeedDownSoundComponent->Play(0.f);
}

void AC_Ch_Player::CalculateShieldTime()
{
	m_ShieldMoment = GetWorld()->GetTimeSeconds();
}

void AC_Ch_Player::CheckAbilities()
{
	//storing position of ability icons in clockwise Y(0), B(1), A(2), X(3)
	switch (InputStates)
	{
	case EInputStates::S0:
		ABXYAbilities = { "Attack", "Dash", "UseItem", "SelfDestruct" };
		PosSelfDestruct = 0;
		PosDash = 1;
		PosAttack = 2;
		PosItem = 3;
		break;
	case EInputStates::S1:
		ABXYAbilities = { "Dash", "Attack", "SelfDestruct", "UseItem" };
		PosItem = 0;
		PosAttack = 1;
		PosDash = 2;
		PosSelfDestruct = 3;
		break;
	case EInputStates::S2:
		ABXYAbilities = { "SelfDestruct", "UseItem", "Dash", "Attack" };
		PosAttack = 0;
		PosItem = 1;
		PosSelfDestruct = 2;
		PosDash = 3;
		break;
	case EInputStates::S3:
		ABXYAbilities = { "UseItem", "SelfDestruct", "Attack", "Dash" };
		PosDash = 0;
		PosSelfDestruct = 1;
		PosItem = 2;
		PosAttack = 3;
		break;
	default:
		break;
	}
}

void AC_Ch_Player::SetNegativeInputFeedbackOn(int32 PosAbility)
{
	NegativeInputOn = PosAbility;
}

void AC_Ch_Player::Die()
{
	bIsDead = true;
	m_GameState->DeadPlayers++;
	if (DeathSoundComponent && DeathSoundCue)
	{
		DeathSoundComponent->Play(0.f);
	}
	this->Tags.Add("Grave");
}

void AC_Ch_Player::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsBeingPushed)
	{
		if (OtherActor->ActorHasTag("PlayerCharacter"))
		{
			bIsBeingPushed = true;
			SetBounceTimer(BounceStunDuration);

			FVector ForceVectorOther = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), OtherActor->GetActorLocation());
			ForceVectorOther.Z = 0;
			((ACharacter*)OtherActor)->LaunchCharacter(ForceVectorOther * (PlayerBounciness / 2), false, false);

			FVector ForceVectorSelf = UKismetMathLibrary::GetDirectionUnitVector(OtherActor->GetActorLocation(), GetActorLocation());
			ForceVectorSelf.Z = 0;
			if (/*((AC_Ch_Player*)OtherActor)->*/bIsDashing)
			{
				LaunchCharacter((ForceVectorSelf * DashBounciness), true, false);
			}
			else
			{
				LaunchCharacter((ForceVectorSelf * PlayerBounciness / 2), true, false);
			}
		}
		else if (OtherActor && !OtherActor->ActorHasTag("Ground") && GetVelocity().Size() > 700.f)
		{
			bIsBeingPushed = true;
			SetBounceTimer(BounceStunDuration / 2);

			FVector BounceDirection = UKismetMathLibrary::GetDirectionUnitVector(Hit.Location, GetActorLocation());
			BounceDirection.Z = 0;

			if (bIsDashing)
			{
				LaunchCharacter(BounceDirection * BasicObjectBounciness * 2, true, false);
			}
			else
			{
				LaunchCharacter(BounceDirection * BasicObjectBounciness, true, false);
			}

			SwayWorldMesh(OtherActor);
		}
	}
}

void AC_Ch_Player::SetBounceTimer(float Duration)
{
	//UE_LOG(LogTemp, Warning, TEXT("sEt"))
	GetWorld()->GetTimerManager().SetTimer(MovementLockTimeHandle, this, &AC_Ch_Player::ResetBounceBool, Duration, false);
}

void AC_Ch_Player::ResetBounceBool()
{
	//UE_LOG(LogTemp, Warning, TEXT("rEsEt"))
	bIsBeingPushed = false;
	GetWorldTimerManager().ClearTimer(MovementLockTimeHandle);
}
