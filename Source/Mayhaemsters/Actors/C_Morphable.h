// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#pragma once

#include "CoreMinimal.h"
#include "Mayhaemsters/Actors/C_Bounceable.h"
#include "C_Morphable.generated.h"

class USkeletalMeshComponent;

UCLASS()
class MAYHAEMSTERS_API AC_Morphable : public AC_Bounceable
{
	GENERATED_BODY()

public:
	AC_Morphable();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Morphing")
		void MorphMesh();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* MorphableMesh;
};
