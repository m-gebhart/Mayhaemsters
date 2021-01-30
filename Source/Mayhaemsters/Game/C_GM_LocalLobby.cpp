// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#include "Mayhaemsters/Game/C_GM_LocalLobby.h"
#include "Mayhaemsters/Characters/C_Ch_LocalLobbyPlayer.h"
#include "Mayhaemsters/Game/C_GI_Mayhaemsters.h"
#include "Mayhaemsters/Characters/C_PC_LocalLobbyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Engine/LocalPlayer.h" 


AC_GM_LocalLobby::AC_GM_LocalLobby() :
	LobbySlots(4)
{
	LocalLobbyPlayerControllers.Init(nullptr, 4);
	DefaultPawnClass = AC_Ch_LocalLobbyPlayer::StaticClass();
	PlayerControllerClass = AC_PC_LocalLobbyPlayer::StaticClass();
}

void AC_GM_LocalLobby::BeginPlay()
{
	AssignPlayer0();
	AssignLocalPlayers(1);
}

void AC_GM_LocalLobby::AssignPlayer0()
{
	LocalLobbyPlayerControllers[0] = UGameplayStatics::GetPlayerController(this->GetWorld(), 0);
}

void AC_GM_LocalLobby::AssignLocalPlayers(int32 Player1Index)
{
	for (int32 PlayerId = Player1Index; PlayerId < LobbySlots; PlayerId++)
	{
		APlayerController* NewController = UGameplayStatics::CreatePlayer(this->GetWorld(), PlayerId, true);
		LocalLobbyPlayerControllers[PlayerId] = NewController;
	}
}

void AC_GM_LocalLobby::StartGame(int32 int_Players)
{
	UC_GI_Mayhaemsters* m_GameInstance = (UC_GI_Mayhaemsters*)UGameplayStatics::GetGameInstance(GetWorld());
	m_GameInstance->PlayerCount = int_Players;
	
	m_GameInstance->bIsInLobby = false;

	UGameplayStatics::OpenLevel(GetWorld(), "IslandOfBenuts");
}
