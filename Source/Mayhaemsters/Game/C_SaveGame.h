// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "C_SaveGame.generated.h"

UCLASS()
class MAYHAEMSTERS_API UC_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UC_SaveGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 DeadPlayers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 Score_P1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 Score_P2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 Score_P3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 Score_P4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 RoundNumber;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 CurrentScreenResolutionIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		FString FullScreenState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		FString ResolutionConsoleCommand;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 AAType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 AAQuality;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 UserIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		float MusicVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		float SFXVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		int32 MaxNumberOfRounds;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		FString Name_P1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		FString Name_P2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		FString Name_P3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
		FString Name_P4;
};
