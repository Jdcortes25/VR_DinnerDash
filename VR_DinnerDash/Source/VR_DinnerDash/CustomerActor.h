// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomerActor.generated.h"

UCLASS()
class VR_DINNERDASH_API ACustomerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomerActor();
	//~ACustomerActor();

	enum Customer_State
	{
		UNSEATED,
		LOOKING_AT_MENU,
		READY_TO_ORDER,
		WAITING_ON_FOOD,
		EATING,
		READY_TO_LEAVE
	};

	FTimerHandle timeHandler;
	Customer_State currCustomerState;

	UFUNCTION(BlueprintImplementableEvent, Category = "Customer_Events")
	void OnCustomerSeated();
	UFUNCTION(BlueprintImplementableEvent, Category = "Customer_Events")
	void OnCustomerReadyToOrder();
	UFUNCTION(BlueprintImplementableEvent, Category = "Customer_Events")
	void OnCustomerOrderReceived();
	UFUNCTION(BlueprintImplementableEvent, Category = "Customer_Events")
	void OnCustomerFoodReceived();
	UFUNCTION(BlueprintImplementableEvent, Category = "Customer_Events")
	void OnCustomerReadyToLeave();
	UFUNCTION(BlueprintImplementableEvent, Category = "Customer_Events")
	void OnCustomerLeaveHappy();
	UFUNCTION(BlueprintImplementableEvent, Category = "Customer_Events")
	void OnCustomerLeaveSad();
	UFUNCTION(BlueprintImplementableEvent, Category = "Customer_Events")
	void OnCustomerBeingSeated(bool inProcess);
	UFUNCTION(BlueprintImplementableEvent, Category = "Customer_Events")
	void OnHeartRemoved();

	UPROPERTY(BlueprintReadWrite, Category = "Customer_Stats")
	int customerHearts{ 10 };
	UPROPERTY(BlueprintReadWrite, Category = "Customer_Stats")
	float customerPatience{ 10 };
	UPROPERTY(BlueprintReadWrite, Category = "Customer_Stats")
	float lookAtMenuTime{ 10 };
	UPROPERTY(BlueprintReadWrite, Category = "Customer_Stats")
	float eatingTime{ 10 };
	UPROPERTY(BlueprintReadWrite, Category = "Customer_References")
	TSet<AActor*> listOfCustomers;

	UPROPERTY(BlueprintReadWrite, Category = "CustomerFigure_Specs")
	int spacing{ 105 };
	UPROPERTY(BlueprintReadWrite, Category = "CustomerFigure_Specs")
	TSubclassOf<class AActor> customerFigureReference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void CreateCustomer();
	void CustomerWaitingLogic();
	void ReadyToOrder();
	void ReadyToLeave();
	void RemoveHeart();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CustomerBeingSeated(bool inProcess);
	UFUNCTION(BlueprintCallable, Category = "Customer_Transitions")
	void SeatCustomer();
	UFUNCTION(BlueprintCallable, Category = "Customer_Transitions")
	void OrderReceived();
	UFUNCTION(BlueprintCallable, Category = "Customer_Transitions")
	void FoodReceived();
	UFUNCTION(BlueprintCallable, Category = "Customer_Transitions")
	void CustomerLeave(bool happy);
};
