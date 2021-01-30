// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "C_GS_Mayhaemsters.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventType); // creating a new type "FEventType" that allows to broadcast functions to blueprints

class UC_GI_Mayhaemsters;
class AC_Ch_Player;

UCLASS()
class MAYHAEMSTERS_API AC_GS_Mayhaemsters : public AGameStateBase
{
	GENERATED_BODY()

public:
	AC_GS_Mayhaemsters();
	virtual void BeginPlay() override;

private:

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 DeadPlayers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 DeadPlayersLimit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		TArray<AActor*> AllPlayers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		bool bGameOver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		bool bIsResettingMap;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
		FEventType OnRoundEnded;
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
		FEventType OnMapReset;


	bool bGameRunning;

	void RoundOver();
	void MapReset();
	AC_Ch_Player* GetWinner();
};
