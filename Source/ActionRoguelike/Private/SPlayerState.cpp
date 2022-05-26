// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
{
	TotalCredits = 0;
}

ASPlayerState* ASPlayerState::GetPlayerState(AActor* FromActor)
{
	if (!IsValid(FromActor) || !IsValid(FromActor->GetInstigatorController()))
	{
		return nullptr;
	}
	
	return Cast<ASPlayerState>(FromActor->GetInstigatorController()->PlayerState);
}

void ASPlayerState::AddCredits(const int32 Credits)
{
	if (HasAuthority())
	{
		TotalCredits += Credits;
		MulticastCreditsChanged(TotalCredits, Credits);
		LogOnScreen(GetWorld(), FString::Printf(TEXT("NewCreditsTotal: %d, Delta: %d, Player: %s"), TotalCredits, Credits, *GetNameSafe(GetOwner())));
	}
}

void ASPlayerState::RemoveCredits(const int32 Credits)
{
	if (HasAuthority())
	{
		TotalCredits -= Credits;
		TotalCredits = FMath::Max(0, TotalCredits);
		MulticastCreditsChanged(TotalCredits, -Credits);
		LogOnScreen(GetWorld(), FString::Printf(TEXT("NewCreditsTotal: %d, Delta: %d, Player: %s"), TotalCredits, -Credits, *GetNameSafe(GetOwner())));
	}
}

void ASPlayerState::MulticastCreditsChanged_Implementation(int32 NewCreditsTotal, int32 Delta)
{
	OnCreditsChanged.Broadcast(NewCreditsTotal, Delta);
}
