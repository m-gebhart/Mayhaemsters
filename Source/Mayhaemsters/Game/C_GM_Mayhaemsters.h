// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "C_GM_Mayhaemsters.generated.h"

class AC_Ch_Player;
class APlayerStart;
class TimerManager;
class USoundCue;

UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
	Day UMETA(DisplayName = "Day"),
	Dusk UMETA(DisplayName = "Dusk"),
	Night UMETA(DisplayName = "Night")
};

UCLASS()
class MAYHAEMSTERS_API AC_GM_Mayhaemsters : public AGameModeBase
{
	GENERATED_BODY()

public:
	AC_GM_Mayhaemsters();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Item Management")
		int32 GetItemTypeInt(int32 randomInt);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Of Day")
		ETimeOfDay TimeOfDay;

	UFUNCTION(BlueprintCallable)
		void DilateTime();

private:
	void SetupSpawnPercentageArray();
	void SetCountdownTimer();
	void SetOnGoTimer();
	FString GetOnGoText();
	void SetTimeOfDay();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Time Of Day")
		void SetDayTime(ETimeOfDay DayTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
		float TimeDilationValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
		float TimeDilationDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Border Death")
		float CameraBorderDeathDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Countdown", meta = (ToolTip = "Time for '3, 2, 1'"))
		float CountdownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Countdown", meta = (ToolTip = "Time for 'GO!'"))
		float OnGoTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Countdown")
		TArray<FString> OnGoTexts;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Countdown")
		FString CountdownText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Countdown")
		int32 CountdownCounter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Countdown")
		float CountdownSoundPitch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Countdown")
		float CountdownSoundVol;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Countdown")
		TArray<USoundCue*> CountdownSounds;

	FTimerHandle CountdownTimeHandle;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Feed Announcements")
		TArray<FString> WinFeedAnnouncements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawn Percentage")
		int32 Remap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawn Percentage")
		int32 Shield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawn Percentage")
		int32 SpeedUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawn Percentage")
		int32 SpeedDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawn Percentage")
		int32 MapReset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawn Percentage")
		int32 Mystery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawn Percentage")
		int32 Unifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawn Percentage")
		int32 Jammer;

private:
	APlayerStart* m_PlayerStart;
	float m_TimeDilationMoment;
	TArray<int32> ItemSpawnPercentages;
};
