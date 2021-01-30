// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#include "Mayhaemsters/Game/C_GS_Mayhaemsters.h"
#include "Mayhaemsters/Game/C_GI_Mayhaemsters.h"
#include "Mayhaemsters/Characters/C_Ch_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AC_GS_Mayhaemsters::AC_GS_Mayhaemsters() :
	bGameRunning(false),
	DeadPlayers(0),
	DeadPlayersLimit(0),
	bIsResettingMap(false),
	bGameOver(false)
{
}

void AC_GS_Mayhaemsters::BeginPlay()
{
	Super::BeginPlay();
}

void AC_GS_Mayhaemsters::RoundOver()
{
	if (DeadPlayers >= DeadPlayersLimit)
	{
		UC_GI_Mayhaemsters* GameInstance = (UC_GI_Mayhaemsters*)UGameplayStatics::GetGameInstance(GetWorld());
		if (GameInstance && bGameRunning)
		{
			GameInstance->RoundNumber++;
		}

		GameInstance->LatestWinner = GetWinner();
		bGameRunning = false;

		for (AActor* Player : AllPlayers)
		{
			AC_Ch_Player* Character = (AC_Ch_Player*)Player;
			if (!Character->bIsDead)
			{
				if (Character == (AC_Ch_Player*)UGameplayStatics::GetPlayerCharacter(this, 0))
				{
					GameInstance->Score_P1++;
				}
				else if (Character == (AC_Ch_Player*)UGameplayStatics::GetPlayerCharacter(this, 1))
				{
					GameInstance->Score_P2++;
				}
				else if (Character == (AC_Ch_Player*)UGameplayStatics::GetPlayerCharacter(this, 2))
				{
					GameInstance->Score_P3++;
				}
				else if (Character == (AC_Ch_Player*)UGameplayStatics::GetPlayerCharacter(this, 3))
				{
					GameInstance->Score_P4++;
				}
			}
		}

		if ((GameInstance->Score_P1 == GameInstance->MaxNumberOfRounds) ||
			(GameInstance->Score_P2 == GameInstance->MaxNumberOfRounds) ||
			(GameInstance->Score_P3 == GameInstance->MaxNumberOfRounds) ||
			(GameInstance->Score_P4 == GameInstance->MaxNumberOfRounds))
		{
			bGameOver = true;
		}

		OnRoundEnded.Broadcast();
	}
}

AC_Ch_Player* AC_GS_Mayhaemsters::GetWinner()
{
	if (DeadPlayers >= DeadPlayersLimit)
	{
		for (AActor* Player : AllPlayers)
		{
			AC_Ch_Player* Character = (AC_Ch_Player*)Player;
			if (!Character->bIsDead)
			{
				return Character;
			}
		}
	}
	return NULL; 
}

void AC_GS_Mayhaemsters::MapReset()
{
	OnMapReset.Broadcast();
}
