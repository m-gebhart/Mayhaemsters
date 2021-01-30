// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "C_Bounceable.generated.h"

UCLASS()
class MAYHAEMSTERS_API AC_Bounceable : public AActor
{

	GENERATED_BODY()
	
public:	
	AC_Bounceable();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnHitBounce(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void PlayTimeline(struct FTimeline& Timeline, FName Function, UCurveFloat* Curve, bool PlayOnce);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounciness")
		float BounceFactor;
};
