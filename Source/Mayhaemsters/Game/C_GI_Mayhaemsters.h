// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "C_GI_Mayhaemsters.generated.h"

class UC_SaveGame;
class AC_Ch_Player;

UCLASS()
class MAYHAEMSTERS_API UC_GI_Mayhaemsters : public UGameInstance
{
	GENERATED_BODY()

public:
	UC_GI_Mayhaemsters();

private:
	UC_SaveGame* m_SaveGameInstance;
	UC_SaveGame* m_LoadedGame;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 PlayerCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 RoundNumber;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		AC_Ch_Player* LatestWinner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		int32 Score_P1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		int32 Score_P2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		int32 Score_P3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		int32 Score_P4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		int32 MaxNumberOfRounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		FString Name_P1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		FString Name_P2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		FString Name_P3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		FString Name_P4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		int32 LobbyPlayersReady;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		bool bIsInLobby;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Options")
		int32 CurrentScreenResolutionIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Options")
		FString FullScreenState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Options")
		float MusicVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Options")
		float SFXVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Options")
		FString ResolutionConsoleCommand; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Options")
		int32 AAType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Options")
		int32 AAQuality;

public:
	UFUNCTION(BlueprintCallable, Category = "Saving and loading")
		void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "Saving and loading")
		bool LoadGame();
	UFUNCTION(BlueprintCallable, Category = "Saving and loading")
		void DeleteSave();
	UFUNCTION(BlueprintCallable, Category = "Saving and loading")
		void ResetValues();
};
