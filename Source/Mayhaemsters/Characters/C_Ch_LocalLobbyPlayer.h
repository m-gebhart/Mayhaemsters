// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Ch_LocalLobbyPlayer.generated.h"

class UC_GI_Mayhaemsters;

UCLASS()
class MAYHAEMSTERS_API AC_Ch_LocalLobbyPlayer : public ACharacter
{
	GENERATED_BODY()


public:
	AC_Ch_LocalLobbyPlayer();

protected:
	virtual void BeginPlay() override;

	void Confirm();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bHasJoined;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bIsReady;
};
