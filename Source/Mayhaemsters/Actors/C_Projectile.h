// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "C_Projectile.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMorphing); // creating a new type "FEventType" that allows to broadcast functions to blueprints

class UStaticMesh;
class UProjectileMovementComponent;
class AC_Ch_Player;
class UC_GI_Mayhaemsters;
class AC_GS_Mayhaemsters;
class AC_GM_Mayhaemsters;
class AC_Morphable;

UCLASS()
class MAYHAEMSTERS_API AC_Projectile : public AActor
{
	GENERATED_BODY()
	
private:
	float m_ShootingMoment;
	UC_GI_Mayhaemsters* m_GameInstance;
	AC_GS_Mayhaemsters* m_GameState;
	TArray<FHitResult> m_OutHits;
	APlayerController* m_PlayerController;

	bool SearchForMorphables();
	void Explode();

public:	
	// Sets default values for this actor's properties
	AC_Projectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UProjectileMovementComponent* ProjectileComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* StMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
		float ExplosionRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
		float FlightDuration;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY (VisibleAnywhere, BlueprintReadWrite, Category = "Player")
		AC_Ch_Player* Player;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		int32 ControllerIndex;


	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
