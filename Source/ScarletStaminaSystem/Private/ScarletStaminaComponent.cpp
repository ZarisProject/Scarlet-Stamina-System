// Fill out your copyright notice in the Description page of Project Settings.


#include "ScarletStaminaComponent.h"

// Sets default values for this component's properties
UScarletStaminaComponent::UScarletStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bAutoActivate = true;
}


// Called when the game starts
void UScarletStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// Start with max stamina
	Stamina = MaxStamina;
	Depleted = false;
	
}


// Updates stamina recovery
void UScarletStaminaComponent::UpdateRecovery(float DeltaTime)
{
	if (Stamina >= MaxStamina) return;

	// Timer ticking
	if (RecoveryTimer > 0.f)
	{
		RecoveryTimer -= DeltaTime;

		// Timer completed
		if (RecoveryTimer <= 0.f)
			OnStaminaStartedRecovery.Broadcast();
	}

	// Recovery ticking
	else
	{
		RecoverStamina(StaminaRecoveryRate * DeltaTime);
	}
}


// Called every frame
void UScarletStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsActive())
		UpdateRecovery(DeltaTime);
}



// Consumes the requested amount of stamina
// Returns true if there was enough stamina - false otherwise
// If there was not enough stamina, OnNotEnoughStamina is called
bool UScarletStaminaComponent::ConsumeStamina(float Amount)
{
	if (!IsEnoughStamina(Amount))
	{
		OnNotEnoughStamina.Broadcast((Depleted) ? Amount : Amount - Stamina);
		return false;
	}

	// Timeout timer reset
	RecoveryTimer = StaminaRecoveryTimeout;

	// Cache old value for delta calcuation
	float OldStamina = Stamina;

	// Update stamina
	Stamina -= Amount;

	// Depletion Check
	if (Stamina <= 0.f)
	{
		Stamina = 0.0;
		Depleted = true;

		RecoveryTimer = StaminaDepletionRecoveryTimeout;
	}

	float NegDelta = OldStamina - Stamina;

	// Delegates
	OnStaminaConsumed.Broadcast(NegDelta);
	OnStaminaChanged.Broadcast(-1.f * NegDelta);
	if (Depleted)
		OnStaminaDepleted.Broadcast();

	return true;
}

// Recovers the specified amount of stamina
void UScarletStaminaComponent::RecoverStamina(float Amount)
{
	// Early exit if stamina is at max value
	if (Stamina == MaxStamina)
		return;

	// Cache old value for delta calcuation
	float OldStamina = Stamina;

	// Update stamina
	Stamina += Amount;
	if (Stamina > MaxStamina)
		Stamina = MaxStamina;

	if (Depleted && (Stamina >= DepletionRecoveryThreshold))
	{
		Depleted = false; // Remove depletion when stamina has passed the threshold
		OnStaminaDepletionRecovered.Broadcast();
	}

	float Delta = Stamina - OldStamina;

	// Delegates
	OnStaminaRecovered.Broadcast(Delta);
	OnStaminaChanged.Broadcast(Delta);
	if (Stamina == MaxStamina)
		OnStaminaFullyRecovered.Broadcast();
}

