// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#include "Mayhaemsters/Game/C_GM_Mayhaemsters.h"
#include "Mayhaemsters/Game/C_GI_Mayhaemsters.h"
#include "Mayhaemsters/Game/C_GM_LocalLobby.h"
#include "Mayhaemsters/Game/C_GS_Mayhaemsters.h"
#include "Mayhaemsters/Characters/C_Ch_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h" 
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Containers/Array.h"
#include "TimerManager.h" 


AC_GM_Mayhaemsters::AC_GM_Mayhaemsters() :
	m_TimeDilationMoment(0.f),
	TimeDilationValue(.1f),
	TimeDilationDuration(1.f),
	CountdownTime(3.f),
	OnGoTime(.5f),
	CountdownText("3"),
	CountdownCounter(3),
	CountdownSoundPitch(1.f),
	CountdownSoundVol(1.f),
	Remap(13.f),
	Shield(13.f),
	SpeedUp(13.f),
	SpeedDown(9.f),
	MapReset(13.f),
	Mystery(13.f),
	Unifier(13.f),
	Jammer(13.f),
	CameraBorderDeathDuration(2.f)
{
	PrimaryActorTick.bCanEverTick = true;
	ItemSpawnPercentages.Init(0, 8);
	DefaultPawnClass = AC_Ch_Player::StaticClass();
	OnGoTexts.Add("Go!");
}

void AC_GM_Mayhaemsters::BeginPlay()
{
	Super::BeginPlay();

	SetTimeOfDay();

	SetupSpawnPercentageArray();
	UC_GI_Mayhaemsters* m_GameInstance = (UC_GI_Mayhaemsters*)UGameplayStatics::GetGameInstance(GetWorld());
	AC_GS_Mayhaemsters* m_GameState = (AC_GS_Mayhaemsters*)UGameplayStatics::GetGameState(GetWorld());
	m_GameState->DeadPlayersLimit = m_GameInstance->PlayerCount - 1;
	TArray<AActor*> StartPoints;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), StartPoints);


	if (StartPoints.Num() != 0 && !UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 1)) 
	{
		for (int32 ControllerIndex = 0; ControllerIndex < m_GameInstance->PlayerCount; ControllerIndex++)
		{
			UGameplayStatics::CreatePlayer(this->GetWorld(), ControllerIndex, true);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(CountdownTimeHandle, this, &AC_GM_Mayhaemsters::SetCountdownTimer, CountdownTime/3.f, true);
	if (CountdownSounds[0])
		UGameplayStatics::PlaySound2D(GetWorld(), (USoundBase*)CountdownSounds[0], CountdownSoundVol, CountdownSoundPitch, 0.f);
}

void AC_GM_Mayhaemsters::SetCountdownTimer()
{
	CountdownCounter--;
	switch (CountdownCounter)
	{
	case 2:
		if (CountdownSounds[1])
			UGameplayStatics::PlaySound2D(GetWorld(), (USoundBase*)CountdownSounds[1], CountdownSoundVol, CountdownSoundPitch, 0.f);
		CountdownText = "2";
		break;
	case 1:
		if (CountdownSounds[2])
			UGameplayStatics::PlaySound2D(GetWorld(), (USoundBase*)CountdownSounds[2], CountdownSoundVol, CountdownSoundPitch, 0.f);
		CountdownText = "1";
		break;
	case 0:
		CountdownText = GetOnGoText();
		if (CountdownSounds[3])
			UGameplayStatics::PlaySound2D(GetWorld(), (USoundBase*)CountdownSounds[3], CountdownSoundVol, CountdownSoundPitch, 0.f);
		((AC_GS_Mayhaemsters*)UGameplayStatics::GetGameState(GetWorld()))->bGameRunning = true;
		GetWorldTimerManager().ClearTimer(CountdownTimeHandle);
		GetWorld()->GetTimerManager().SetTimer(CountdownTimeHandle, this, &AC_GM_Mayhaemsters::SetOnGoTimer, OnGoTime, false);
		break;
	}
}

FString AC_GM_Mayhaemsters::GetOnGoText()
{
	if (OnGoTexts.Num() == 0)
		return "GO!";
	return OnGoTexts[FMath::RandRange(0, OnGoTexts.Num()-1)];
}

void AC_GM_Mayhaemsters::SetTimeOfDay()
{
	int32 DayTimeIndex = FMath::RandRange(0, 2);
	TimeOfDay = (ETimeOfDay)DayTimeIndex;

	SetDayTime(TimeOfDay);
}

void AC_GM_Mayhaemsters::SetOnGoTimer()
{
	CountdownCounter = -1;
	GetWorldTimerManager().ClearTimer(CountdownTimeHandle);
}


void AC_GM_Mayhaemsters::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if ((GetWorld()->GetTimeSeconds() - m_TimeDilationMoment) > (TimeDilationDuration / (1.f / TimeDilationValue)))
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	}
}

void AC_GM_Mayhaemsters::SetupSpawnPercentageArray()
{
	ItemSpawnPercentages[0] = Remap;
	ItemSpawnPercentages[1] = Shield;
	ItemSpawnPercentages[2] = SpeedUp;
	ItemSpawnPercentages[3] = SpeedDown;
	ItemSpawnPercentages[4] = MapReset;
	ItemSpawnPercentages[5] = Mystery;
	ItemSpawnPercentages[6] = Unifier;
	ItemSpawnPercentages[7] = Jammer;
}

int32 AC_GM_Mayhaemsters::GetItemTypeInt(int32 int_random)
{
	int32 int_ArrayPos = 0;
	for (int i = 0; i < ItemSpawnPercentages.Num(); i++)
	{
		for (int j = 0; j < ItemSpawnPercentages[i]; j++)
		{
			int_ArrayPos++;
			if (int_ArrayPos == int_random)
				return (int32)i;
		}
	}
	return 0;
}

void AC_GM_Mayhaemsters::DilateTime()
{
	m_TimeDilationMoment = GetWorld()->GetTimeSeconds();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilationValue);
}
