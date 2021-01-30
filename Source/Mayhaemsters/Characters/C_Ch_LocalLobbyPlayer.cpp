// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena


#include "Mayhaemsters/Characters/C_Ch_LocalLobbyPlayer.h"
#include "Mayhaemsters/Game/C_GI_Mayhaemsters.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h" 
#include "Kismet/GameplayStatics.h" 
#include "GameFramework/Actor.h" 
#include "GameFramework/Pawn.h" 
#include "GameFramework/PlayerController.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Containers/Array.h"

// Sets default values
AC_Ch_LocalLobbyPlayer::AC_Ch_LocalLobbyPlayer() :
	bHasJoined(false),
	bIsReady(false)
{
	PrimaryActorTick.bCanEverTick = true;
	UPrimitiveComponent* PhysicsComponent = (UPrimitiveComponent*)GetComponentByClass(UPrimitiveComponent::StaticClass());
	PhysicsComponent->SetSimulatePhysics(false);
	PhysicsComponent->SetEnableGravity(false);
	UCharacterMovementComponent* MovementComponent = (UCharacterMovementComponent*)GetComponentByClass(UCharacterMovementComponent::StaticClass());
	MovementComponent->GravityScale = 0.f;
}

void AC_Ch_LocalLobbyPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AC_Ch_LocalLobbyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AC_Ch_LocalLobbyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ConfirmAction", IE_Pressed, this, &AC_Ch_LocalLobbyPlayer::Confirm);
}

void AC_Ch_LocalLobbyPlayer::Confirm() 
{
	UC_GI_Mayhaemsters* GameInstance = (UC_GI_Mayhaemsters*)UGameplayStatics::GetGameInstance(GetWorld());

	if (GameInstance->bIsInLobby)
	{
		if (!bIsReady)
		{
			if (!bHasJoined) 
			{
				bHasJoined = true;
			}
			else
			{
				bIsReady = true;
			}
		}
	}
}
