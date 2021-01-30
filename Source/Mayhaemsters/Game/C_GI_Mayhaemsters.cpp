// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena


#include "Mayhaemsters/Game/C_GI_Mayhaemsters.h"
#include "Mayhaemsters/Game/C_SaveGame.h"
#include "Styling/SlateColor.h" 
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UC_GI_Mayhaemsters::UC_GI_Mayhaemsters() :
	PlayerCount(4),
	Score_P1(0),
	Score_P2(0),
	Score_P3(0),
	Score_P4(0),
	RoundNumber(0),
	CurrentScreenResolutionIndex(0),
	FullScreenState("f"),
	MusicVolume(1.f),
	SFXVolume(1.f),
	ResolutionConsoleCommand(""),
	AAType(1),
	AAQuality(2),
	MaxNumberOfRounds(3),
	Name_P1(""),
	Name_P2(""),
	Name_P3(""),
	Name_P4(""),
	LobbyPlayersReady(0),
	bIsInLobby(false)
{
}

void UC_GI_Mayhaemsters::SaveGame()
{
	m_SaveGameInstance = (UC_SaveGame*)(UGameplayStatics::CreateSaveGameObject(UC_SaveGame::StaticClass()));

	if (m_SaveGameInstance)
	{
		m_SaveGameInstance->Score_P1 = Score_P1;
		m_SaveGameInstance->Score_P2 = Score_P2;
		m_SaveGameInstance->Score_P3 = Score_P3;
		m_SaveGameInstance->Score_P4 = Score_P4;
		m_SaveGameInstance->RoundNumber = RoundNumber;
		m_SaveGameInstance->CurrentScreenResolutionIndex = CurrentScreenResolutionIndex;
		m_SaveGameInstance->FullScreenState = FullScreenState;
		m_SaveGameInstance->MusicVolume = MusicVolume;
		m_SaveGameInstance->SFXVolume = SFXVolume;
		m_SaveGameInstance->ResolutionConsoleCommand = ResolutionConsoleCommand;
		m_SaveGameInstance->AAType = AAType;
		m_SaveGameInstance->AAQuality = AAQuality;
		m_SaveGameInstance->MaxNumberOfRounds = MaxNumberOfRounds;
		m_SaveGameInstance->Name_P1 = Name_P1;
		m_SaveGameInstance->Name_P2 = Name_P2;
		m_SaveGameInstance->Name_P3 = Name_P3;
		m_SaveGameInstance->Name_P4 = Name_P4;
	}

	UGameplayStatics::SaveGameToSlot(m_SaveGameInstance, "SaveGameSlot", 0);
}

bool UC_GI_Mayhaemsters::LoadGame()
{
	if (m_SaveGameInstance)
	{
		m_LoadedGame = (UC_SaveGame*)(UGameplayStatics::LoadGameFromSlot("SaveGameSlot", 0));

		if (m_LoadedGame)
		{
			Score_P1 = m_LoadedGame->Score_P1;
			Score_P2 = m_LoadedGame->Score_P2;
			Score_P3 = m_LoadedGame->Score_P3;
			Score_P4 = m_LoadedGame->Score_P4;
			RoundNumber = m_LoadedGame->RoundNumber;
			CurrentScreenResolutionIndex = m_LoadedGame->CurrentScreenResolutionIndex;
			FullScreenState = m_LoadedGame->FullScreenState;
			MusicVolume = m_LoadedGame->MusicVolume;
			SFXVolume = m_LoadedGame->SFXVolume;
			ResolutionConsoleCommand = m_LoadedGame->ResolutionConsoleCommand;
			AAType = m_LoadedGame->AAType;
			AAQuality = m_LoadedGame->AAQuality;
			MaxNumberOfRounds = m_LoadedGame->MaxNumberOfRounds;
			Name_P1 = m_LoadedGame->Name_P1;
			Name_P2 = m_LoadedGame->Name_P2;
			Name_P3 = m_LoadedGame->Name_P3;
			Name_P4 = m_LoadedGame->Name_P4;

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void UC_GI_Mayhaemsters::DeleteSave()
{
	UGameplayStatics::DeleteGameInSlot("SaveGameSlot", 0);
}

void UC_GI_Mayhaemsters::ResetValues()
{
	Score_P1 = 0;
	Score_P2 = 0;
	Score_P3 = 0;
	Score_P4 = 0;
	RoundNumber = 0;
}
