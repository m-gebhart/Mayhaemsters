// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Ch_Player.generated.h"

class AC_Projectile;
class AC_GS_Mayhaemsters;
class AC_GM_Mayhaemsters;
class AC_PickupItem;
class UAudioComponent;
class USkeletalMeshComponent;
class TimerManager;

UENUM(BlueprintType)
enum EInputStates
{	S0 UMETA(DisplayName = "State 0"),
	S1 UMETA(DisplayName = "State 1"),
	S2 UMETA(DisplayName = "State 2"),
	S3 UMETA(DisplayName = "State 3")
};

UCLASS()
class MAYHAEMSTERS_API AC_Ch_Player : public ACharacter
{
	GENERATED_BODY()

public:
	AC_Ch_Player();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void RandomizeButtons();
	void PickItemUp(AC_PickupItem* PickedItem);
	void CalculateBoostTime();
	void CalculateReductionTime();
	void CalculateShieldTime();
	void CheckAbilities();
	void Die();
	void SetBounceTimer(float Duration);

	UFUNCTION(BlueprintCallable, Category = "UI Management")
		void SetNegativeInputFeedbackOn(int32 PosAbility);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:
	virtual void BeginPlay() override;

private:
	FString GetCharacterName();
	void MoveX(float Value);
	void MoveY(float Value);
	void RollRotation();
	void RollBackRotation();
	void PitchRotation();
	void PitchBackRotation();
	void AbilityA();
	void AbilityB();
	void AbilityY();
	void AbilityX();

	void Attack();
	void Dash();
	void UseItem();

	void SetCharacterIndex();

	void ResetDashBool();
	void ResetBounceBool();

	void MystifyButtons();
	void UnifyButtons();
	void UnifierAction();
	void JamButtons();

	void OnCountDownEnd();

public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
		void SelfDestruct();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
		void ActivateMystery(int32 PlayerID);
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
		void UpdateUnifierUI(int32 PlayerID, int32 UnifiedButton);
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
		void UpdateJammerUI(int32 PlayerID, const TArray<int32>& JammedButtons);
	UFUNCTION(BlueprintCallable, Category = "Ability")
		void DeactivateMystery();
	UFUNCTION(BlueprintCallable, Category = "Ability")
		void DeactivateUnifier();
	UFUNCTION(BlueprintCallable, Category = "Ability")
		void DeactivateJammer();
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
		void DeactivateUIItems();
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
		void ShowDashGhost(int32 PlayerID);
	UFUNCTION(BlueprintImplementableEvent, Category = "Bouncing")
		void SwayWorldMesh(AActor* HitActor);

private:
	AC_Ch_Player* m_CurrentPlayer;
	float originSpeed;
	float m_ShootingMoment;
	float m_DashingMoment;
	float m_SpeedBoostMoment;
	float m_SpeedReductionMoment;
	float m_ShieldMoment;
	float m_MoveXValue, m_MoveYValue;
	AC_GS_Mayhaemsters* m_GameState;
	float m_previousYawRotation;
	float m_currentYawRotation;
	UCharacterMovementComponent* m_Movement;
	UAudioComponent* AttackSoundComponent;
	UAudioComponent* DashSoundComponent;
	UAudioComponent* CollectSoundComponent;
	UAudioComponent* BallSoundComponent;
	UAudioComponent* RemapSoundComponent;
	UAudioComponent* ResetMapSoundComponent;
	UAudioComponent* DeathSoundComponent;
	UAudioComponent* SpeedUpSoundComponent;
	UAudioComponent* SpeedDownSoundComponent;
	USkeletalMeshComponent* m_SkelMesh;
	FTimerHandle MovementLockTimeHandle;
	FTimerHandle DashTimeHandle;
	bool bIsBeingPushed;

	FVector MeshRelativeLoc;

	int32 m_ChosenUnifiedButton;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Identity")
		FString CharacterName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Identity")
		int32 PlayerIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enums")
		TEnumAsByte<EInputStates> InputStates;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Spawning")
		TSubclassOf<AC_Projectile> Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "Movement and Rotation")
		float AccelerationValue;
	UPROPERTY(EditDefaultsOnly, Category = "Movement and Rotation")
		float DecelerationValue;
	UPROPERTY(EditDefaultsOnly, Category = "Movement and Rotation")
		float YawRotationIntensity;
	UPROPERTY(EditDefaultsOnly, Category = "Movement and Rotation")
		float RollRotationIntensity;
	UPROPERTY(EditDefaultsOnly, Category = "Movement and Rotation")
		float RollRotationLimit;
	UPROPERTY(EditDefaultsOnly, Category = "Movement and Rotation")
		float RollRotationBackIntensity;
	UPROPERTY(EditDefaultsOnly, Category = "Movement and Rotation")
		float PitchRotationIntensity;
	UPROPERTY(EditDefaultsOnly, Category = "Movement and Rotation")
		float PitchRotationLimit;
	UPROPERTY(EditDefaultsOnly, Category = "Movement and Rotation")
		float PitchRotationAccelerationLimit;
	UPROPERTY(EditDefaultsOnly, Category = "Movement and Rotation")
		float PitchRotationBackIntensity;

	UPROPERTY(EditDefaultsOnly, Category = "Bounciness")
		float BounceStunDuration;
	UPROPERTY(EditDefaultsOnly, Category = "Bounciness")
		float PlayerBounciness;
	UPROPERTY(EditDefaultsOnly, Category = "Bounciness")
		float DashBounciness;
	UPROPERTY(EditDefaultsOnly, Category = "Bounciness")
		float BasicObjectBounciness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Management")
		float AttackCooldown;
	UPROPERTY(EditDefaultsOnly, Category = "Ability Management")
		float DashDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Management")
		float DashCooldown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Management")
		float DashBoolDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Management")
		TArray<FString> ABXYAbilities;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Management")
		int32 PosSelfDestruct;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Management")
		int32 PosDash;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Management")
		int32 PosAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Management")
		int32 PosItem;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability Management")
		int32 NegativeInputOn;

	UPROPERTY(EditDefaultsOnly, Category = "Item Management")
		float DefaultSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Item Management")
		float BoostedSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Item Management")
		float ReducedSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Management")
		float SpeedBoostDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Management")
		float SpeedReductionDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Management")
		float ShieldDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Management")
		bool bMysteryActive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Management")
		bool bUnifierActive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Management")
		bool bJammerActive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Management")
		TArray<int32> m_ChosenJammedButtons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bIsDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bCanShoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bCanDash;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bIsDashing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bInAcceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		TArray<AC_PickupItem*> Inventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bIsBoosted;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bIsSlowed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bIsShieldActive;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player State")
		bool bGraveGrounded;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		class USoundCue* AttackSoundCue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		class USoundCue* DashSoundCue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		class USoundCue* CollectSoundCue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		class USoundCue* BallSoundCue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		class USoundCue* RemapSoundCue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		class USoundCue* ResetMapSoundCue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		class USoundCue* DeathSoundCue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		class USoundCue* SpeedUpSoundCue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		class USoundCue* SpeedDownSoundCue;
};
