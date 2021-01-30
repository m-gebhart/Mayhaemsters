// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "C_GM_LocalLobby.generated.h"

class AC_Ch_LocalLobbyPlayer;

UCLASS()
class MAYHAEMSTERS_API AC_GM_LocalLobby : public AGameModeBase
{
	GENERATED_BODY()
	

	AC_GM_LocalLobby();

	virtual void BeginPlay() override;

	void AssignPlayer0();

private:
	TArray<APlayerController*> LocalLobbyPlayerControllers;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Lobby Settings")
		int32 LobbySlots;

public:
	UFUNCTION(BlueprintCallable, Category = "Lobby Functions")
		void AssignLocalPlayers(int32 Player1Index);

	UFUNCTION(BlueprintCallable, Category = "Lobby Functions")
		void StartGame(int32 Players);
};
