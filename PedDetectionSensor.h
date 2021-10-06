//
// Created by sujan on 19.01.21.
//

#pragma once

#include "Carla/Sensor/Sensor.h"

#include "Carla/Actor/ActorDefinition.h"
#include "Carla/Actor/ActorDescription.h"

#include "Components/BoxComponent.h"

#include "PedDetectionSensor.generated.h"

UCLASS()
class CARLA_API APedDetectionSensor : public ASensor
{
GENERATED_BODY()

public:

APedDetectionSensor(const FObjectInitializer &ObjectInitializer);

static FActorDefinition GetSensorDefinition();

void Set(const FActorDescription &ActorDescription) override;

void SetOwner(AActor *Owner) override;

//void Tick(float DeltaSeconds) override;
virtual void PostPhysTick(UWorld *World, ELevelTick TickType, float DeltaTime) override;


UFUNCTION(BlueprintCallable, Category = "PedDetect")
void SetHorizontalFOV(float NewHorizontalFOV);

UFUNCTION(BlueprintCallable, Category = "PedDetect")
void SetVerticalFOV(float NewVerticalFOV);

UFUNCTION(BlueprintCallable, Category = "PedDetect")
void SetRange(float NewRange);

UFUNCTION(BlueprintCallable, Category = "PedDetect")
void SetPointsPerSecond(int NewPointsPerSecond);

UFUNCTION(BlueprintCallable, Category = "PedDetect")
void SetUpperYDistance(int NewUpperYDistance);

UFUNCTION(BlueprintCallable, Category = "PedDetect")
void SetLowerYDistance(int NewLowerYDistance);

protected:
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Detection")
float Range;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Detection")
float HorizontalFOV;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Detection")
float VerticalFOV;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Detection")
int PointsPerSecond;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Detection")
int UpperYDistance;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Detection")
int LowerYDistance;

private:

FCollisionQueryParams TraceParams;
};