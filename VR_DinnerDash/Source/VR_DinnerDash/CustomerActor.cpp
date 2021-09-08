// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerActor.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ACustomerActor::ACustomerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

//ACustomerActor::~ACustomerActor()
//{
//	delete customerFigure;
//}

// Called when the game starts or when spawned
void ACustomerActor::BeginPlay()
{
	Super::BeginPlay();
	CreateCustomer();
	currCustomerState = Customer_State::UNSEATED;
	GetWorldTimerManager().SetTimer(timeHandler, customerPatience*2, true);
}

// Called every frame
void ACustomerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CustomerWaitingLogic();

}

void ACustomerActor::CreateCustomer()
{
	int numOfCustomerFigures = FMath::RandRange(1, 4);
	int xSpace = 0;
	int ySpace = 0;
	for (int i = 0; i < numOfCustomerFigures; i++)
	{
		if (i != 0)
		{
			if (i % 2 == 0)
			{
				xSpace += spacing;
				ySpace = 0;
			}
			else
			{
				ySpace += spacing / 2;
			}
		}
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		FRotator rotation;
		if (customerFigureReference)
		{
			listOfCustomers.Add(GetWorld()->SpawnActor<AActor>(customerFigureReference, FVector(xSpace, ySpace, 0), rotation, spawnParams));
			listOfCustomers.Array()[listOfCustomers.Num() - 1]->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative,true));
		}		
	}
}

void ACustomerActor::CustomerWaitingLogic()
{
	float timePassed = GetWorldTimerManager().GetTimerElapsed(timeHandler);
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Time passed %f"),timePassed));*/
	switch (currCustomerState)
	{
	case ACustomerActor::UNSEATED:
		if (timePassed >= customerPatience)
		{
			//if (GEngine)
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Remove heart!"));
			RemoveHeart();
		}
		break;
	case ACustomerActor::LOOKING_AT_MENU:
		if (timePassed >= lookAtMenuTime)
		{
			ReadyToOrder();
		}
		break;
	case ACustomerActor::READY_TO_ORDER:
		if (timePassed >= customerPatience)
		{
			RemoveHeart();
		}
		break;
	case ACustomerActor::WAITING_ON_FOOD:
		if (timePassed >= customerPatience)
		{
			RemoveHeart();
		}
		break;
	case ACustomerActor::EATING:
		if (timePassed >= eatingTime)
		{
			ReadyToLeave();
		}
		break;
	case ACustomerActor::READY_TO_LEAVE:
		if (timePassed >= customerPatience)
		{
			RemoveHeart();
		}
		break;
	default:
		break;
	}
}

void ACustomerActor::RemoveHeart()
{
	customerHearts--;
	OnHeartRemoved();
	if (customerHearts <= 0)
	{
		CustomerLeave(false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(timeHandler, customerPatience * 2, true);
	}
}

void ACustomerActor::CustomerLeave(bool happy)
{
	if (happy)
	{
		OnCustomerLeaveHappy();
	}
	else
	{
		OnCustomerLeaveSad();
	}
}

void ACustomerActor::CustomerBeingSeated(bool inProcess)
{
	OnCustomerBeingSeated(inProcess);
	if (inProcess)
	{
		GetWorldTimerManager().PauseTimer(timeHandler);
	}
	else
	{
		GetWorldTimerManager().UnPauseTimer(timeHandler);
	}
}

void ACustomerActor::SeatCustomer()
{
	OnCustomerSeated();
	currCustomerState = LOOKING_AT_MENU;
	GetWorldTimerManager().SetTimer(timeHandler, customerPatience * 2, true);
}

void ACustomerActor::ReadyToOrder()
{
	OnCustomerReadyToOrder();
	currCustomerState = READY_TO_ORDER;
	GetWorldTimerManager().SetTimer(timeHandler, customerPatience * 2, true);
}

void ACustomerActor::OrderReceived()
{
	OnCustomerOrderReceived();
	currCustomerState = WAITING_ON_FOOD;
	GetWorldTimerManager().SetTimer(timeHandler, customerPatience * 2, true);
}

void ACustomerActor::FoodReceived()
{
	OnCustomerFoodReceived();
	currCustomerState = EATING;
	GetWorldTimerManager().SetTimer(timeHandler, customerPatience * 2, true);
}

void ACustomerActor::ReadyToLeave()
{
	OnCustomerReadyToLeave();
	currCustomerState = READY_TO_LEAVE;
	GetWorldTimerManager().SetTimer(timeHandler, customerPatience * 2, true);
}

