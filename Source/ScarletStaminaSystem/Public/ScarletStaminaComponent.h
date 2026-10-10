// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScarletStaminaComponent.generated.h"

// Delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaConsumed, float, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaRecovered, float, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotEnoughStamina, float, Missing);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaStartedRecovery);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaDepletionRecovered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaFullyRecovered);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCARLETSTAMINASYSTEM_API UScarletStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScarletStaminaComponent();

protected:

	// Current amount of stamina points
	float Stamina;

	// Timer value for stamina recovery
	float RecoveryTimer = 0.f;

	// Is stamina currently depleted
	bool Depleted = false;

public:

	// Maximum amount of stamina points
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	float MaxStamina = 100.0;

	// Time that has to pass since last stamina consumption for stamina to start recovering
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Recovery")
	float StaminaRecoveryTimeout = 2.f;

	// Time that has to pass since stamina depletion for stamina to start recovering
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Recovery")
	float StaminaDepletionRecoveryTimeout = 5.f;

	// Amount of stamina regenerated each second during stamina recovery
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Recovery")
	float StaminaRecoveryRate = 10.f;

	// Amount of stamina that has to recover after depletion, for depletion to go away (stamina can be consumed again)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Recovery")
	float DepletionRecoveryThreshold = 50.f;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Updates stamina recovery
	void UpdateRecovery(float DeltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	// Returns current amount of stamina points
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStaminaSystem")
	float GetStamina() { return Stamina; }

	// Returns maximum amount of stamina points
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStaminaSystem")
	float GetMaxStamina() { return MaxStamina; }


	// Whether stamina is currently recovering
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStaminaSystem")
	bool IsRecovering() { return RecoveryTimer <= 0.0f; }

	// Current value of recovery timer
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStaminaSystem")
	float GetRecoveryTimer() { return RecoveryTimer; }

	// Whether stamina is currently depleted
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStaminaSystem")
	bool IsDepleted() { return Depleted; }


	// Is there enough stamina for something? Always false if stamina is depleted
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStaminaSystem")
	bool IsEnoughStamina(float Required) { return (!Depleted) && (Stamina >= Required); }

	// Consumes the requested amount of stamina
	// Returns true if there was enough stamina - false otherwise
	// If there was not enough stamina, OnNotEnoughStamina is called
	UFUNCTION(BlueprintCallable, Category = "ScarletStaminaSystem")
	bool ConsumeStamina(float Amount);

	// Recovers the specified amount of stamina
	UFUNCTION(BlueprintCallable, Category = "ScarletStaminaSystem")
	void RecoverStamina(float Amount);

public:

	// Delegates

	// Fired when any amount of stamina is consumed
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnStaminaConsumed OnStaminaConsumed;

	// Fired when any amount of stamina is recovered
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnStaminaRecovered OnStaminaRecovered;

	// Fired when current amount of stamina is changed
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnStaminaChanged OnStaminaChanged;

	// Fired by ConsumeStamina method, when there is not enough stamina
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnNotEnoughStamina OnNotEnoughStamina;


	// Fired when stamina is depleted
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnStaminaDepleted OnStaminaDepleted;

	// Fired when stamina has started recovering
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnStaminaStartedRecovery OnStaminaStartedRecovery;

	// Fired when stamina passes the depletion recovery threshold after being depleted
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnStaminaDepletionRecovered OnStaminaDepletionRecovered;
		 
	// Fired when stamina has finished recovering
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnStaminaFullyRecovered OnStaminaFullyRecovered;
};
