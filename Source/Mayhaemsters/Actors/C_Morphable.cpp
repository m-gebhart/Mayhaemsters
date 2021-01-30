// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena


#include "Mayhaemsters/Actors/C_Morphable.h"
#include "Components/SkeletalMeshComponent.h"

AC_Morphable::AC_Morphable()
{
	PrimaryActorTick.bCanEverTick = true;

	this->Tags.Add("Morphable");

	MorphableMesh = CreateDefaultSubobject<USkeletalMeshComponent>("MorphableMesh");
	RootComponent = MorphableMesh;
}

void AC_Morphable::BeginPlay()
{
	Super::BeginPlay();
}

void AC_Morphable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}