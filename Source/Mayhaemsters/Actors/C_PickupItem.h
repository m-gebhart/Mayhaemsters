// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "C_PickupItem.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USceneComponent;
class AC_Ch_Player;
class UCharacterMovementComponent;
class AC_GS_Mayhaemsters;

UENUM(BlueprintType)
enum class EPickupItemType : uint8
{
	Remapper UMETA(DisplayName = "Control Remapper"),
	Shield UMETA(DisplayName = "Protective Shield"),
	SpeedUp UMETA(DisplayName = "Speed Booster"),
	SpeedDown UMETA(DisplayName = "Speed Reducer"),
	MapResetter UMETA(DisplayName = "Map Resetter"),
	Mystery UMETA(DisplayName = "Mystery"),
	Unifier UMETA(DisplayName = "Unifier"),
	Jammer UMETA(DisplayName = "Jammer")
};

UCLASS()
class MAYHAEMSTERS_API AC_PickupItem : public AActor
{
	GENERATED_BODY()
	
public:
	AC_PickupItem();

	UFUNCTION()
		void RotTimelineProgress(float Value);	
	UFUNCTION()
		void ZTimelineProgress(float Value);

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ResetMap();
	void RemapControls();
	void ActivateShield();
	void DecreaseSpeed();
	void IncreaseSpeed();
	void SetItemIndex(int Index);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void PlayTimeline(struct FTimeline& Timeline, FName Function, UCurveFloat* Curve, bool PlayOnce);

	FTimeline RotCurveTimeline;
	FTimeline ZCurveTimeline;

private:
	AC_Ch_Player* m_OverlappingPlayer;
	AC_Ch_Player* m_OtherPlayer;
	AC_GS_Mayhaemsters* m_GameState;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* StMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
		EPickupItemType ItemType;

	int ItemIndex;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		UCurveFloat* RotCurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		UCurveFloat* ZCurveFloat;

	UPROPERTY()
		FRotator StartRot;
	UPROPERTY()
		FRotator EndRot;
	UPROPERTY()
		FVector StartLoc;
	UPROPERTY()
		FVector EndLoc;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		float YawOffset;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		float ZOffset;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		float RotTimelineSpeed;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		float ZTimelineSpeed;
};
