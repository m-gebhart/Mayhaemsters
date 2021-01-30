// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "Mayhaemsters/Actors/C_Bounceable.h"
#include "C_WaterActor.generated.h"

class UStaticMesh;
class UParticleSystem;

UENUM(BlueprintType)
enum class EWaterObjectType : uint8
{
	Boat  UMETA(DisplayName = "Boat"),
	Buoy  UMETA(DisplayName = "Buoy"),
	Barrel  UMETA(DisplayName = "Barrel")
};

UCLASS()
class MAYHAEMSTERS_API AC_WaterActor : public AC_Bounceable
{
	GENERATED_BODY()
	
public:	
	AC_WaterActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnHitKill(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void ZPosTimelineProgress(float Value);	
	UFUNCTION()
		void RollRotTimelineProgress(float Value);	
	UFUNCTION()
		void PitchRotTimelineProgress(float Value);

	FTimeline zPosCurveTimeline;	
	FTimeline rollRotCurveTimeline;	
	FTimeline pitchRotCurveTimeline;


public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Water Object Properties")
		UParticleSystem* Explosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Object Properties")
		EWaterObjectType WaterObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Object Properties")
		float zPosOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Object Properties")
		float rollRotOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Object Properties")
		float pitchRotOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Water Object Properties|Timelines|Curves")
		UCurveFloat* ZPosCurveFloat;													   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Water Object Properties|Timelines|Curves")
		UCurveFloat* RollRotCurveFloat;													   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Water Object Properties|Timelines|Curves")
		UCurveFloat* PitchRotCurveFloat;													   														   
																						   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Water Object Properties|Timelines")
		float ZTimelineSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Water Object Properties|Timelines")
		float RollTimelineSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Water Object Properties|Timelines")
		float PitchTimelineSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Object Properties|Static Meshes")
		UStaticMesh* BoatMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Object Properties|Static Meshes")
		UStaticMesh* BuoyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Object Properties|Static Meshes")
		UStaticMesh* BarrelMesh;

private:
	FVector OriginLoc;
	FVector TargetLoc;
};
