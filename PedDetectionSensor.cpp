#include "Carla.h"
#include "Carla/Sensor/PedDetectionSensor.h"
#include "Kismet/GameplayStatics.h"
#include "Landscape.h"
#include "Carla/Settings/CarlaSettings.h"
#include "Kismet/KismetMathLibrary.h"

#include "carla/geom/Math.h"
#include "Carla/Actor/ActorBlueprintFunctionLibrary.h"
#include "Carla/Game/CarlaEpisode.h"
#include "Carla/Util/BoundingBoxCalculator.h"
#include "Carla/Vehicle/CarlaWheeledVehicle.h"

#include "Runtime/Engine/Public/DrawDebugHelpers.h"


FActorDefinition APedDetectionSensor::GetSensorDefinition() {
    return UActorBlueprintFunctionLibrary::MakePedDetectorDefinition();
}

APedDetectionSensor::APedDetectionSensor(const FObjectInitializer &ObjectInitializer)
        : Super(ObjectInitializer) {

    PrimaryActorTick.bCanEverTick = true;

    RandomEngine = CreateDefaultSubobject<URandomEngine>(TEXT("RandomEngine"));

    TraceParams = FCollisionQueryParams(FName(TEXT("Laser_Trace")), true, this);
    TraceParams.bTraceComplex = true;
    TraceParams.bReturnPhysicalMaterial = false;
}

void APedDetectionSensor::SetOwner(AActor *Owner) {
    Super::SetOwner(Owner);
}

void APedDetectionSensor::Set(const FActorDescription &ActorDescription) {
    Super::Set(ActorDescription);
    UActorBlueprintFunctionLibrary::SetPedDetector(ActorDescription, this);
}

void APedDetectionSensor::SetHorizontalFOV(float NewHorizontalFOV) {
    HorizontalFOV = NewHorizontalFOV;
}

void APedDetectionSensor::SetVerticalFOV(float NewVerticalFOV) {
    VerticalFOV = NewVerticalFOV;
}

void APedDetectionSensor::SetRange(float NewRange) {
    Range = NewRange;
}

void APedDetectionSensor::SetPointsPerSecond(int NewPointsPerSecond) {
    PointsPerSecond = NewPointsPerSecond;
}

void APedDetectionSensor::SetUpperYDistance(int NewUpperYDistance) {
    UpperYDistance = NewUpperYDistance;
}

void APedDetectionSensor::SetLowerYDistance(int NewLowerYDistance) {
    LowerYDistance = NewLowerYDistance;
}

void APedDetectionSensor::PostPhysTick(UWorld *World, ELevelTick TickType, float DeltaSeconds) {

    TSet < AActor * > DetectedActors;
    DetectedActors.Reset();

    constexpr float TO_METERS = 1e-2;
    FHitResult OutHit(ForceInit);
    const FTransform &ActorTransform = GetActorTransform();
    const FRotator &TransformRotator = ActorTransform.Rotator();
    const FVector &RadarLocation = GetActorLocation();
    const FVector &ForwardVector = GetActorForwardVector();
    const FVector TransformXAxis = ActorTransform.GetUnitAxis(EAxis::X);
    const FVector TransformYAxis = ActorTransform.GetUnitAxis(EAxis::Y);
    const FVector TransformZAxis = ActorTransform.GetUnitAxis(EAxis::Z);

    // Maximum radar radius in horizontal and vertical direction
    const float MaxRx = FMath::Tan(FMath::DegreesToRadians(HorizontalFOV * 0.5f)) * Range; 

    const int NumPoints = (int) (PointsPerSecond * DeltaSeconds);

    for (int i = 0; i < NumPoints; i++) {
        const float Radius = RandomEngine->GetUniformFloat();
        const float Angle = RandomEngine->GetUniformFloatInRange(0.0f, carla::geom::Math::Pi<float>());

        const float y_up = RandomEngine->GetUniformFloatInRange(0, UpperYDistance);
        const float y_bottom = RandomEngine->GetUniformFloatInRange(-LowerYDistance, 0);

        float Sin, Cos;
        FMath::SinCos(&Sin, &Cos, Angle);
        FVector EndLocation;

        // instead of circular cone base, get rectangular cone base to optimally cover the scene
        if (i % 4 == 0) {


            EndLocation = RadarLocation + TransformRotator.RotateVector({
                                                                                Range,
                                                                                MaxRx * Cos,
                                                                                y_up
                                                                        });


        } else {

            EndLocation = RadarLocation + TransformRotator.RotateVector({
                                                                                Range,
                                                                                MaxRx * Cos,
                                                                                y_bottom
                                                                        });
        }        

        const bool Hitted = World->LineTraceSingleByChannel(
                OutHit,
                RadarLocation,
                EndLocation,
                ECC_MAX,
                TraceParams,
                FCollisionResponseParams::DefaultResponseParam
        );
        const TWeakObjectPtr <AActor> HittedActor = OutHit.Actor;

        if (Hitted && HittedActor.Get()) {
            if (HittedActor->GetClass()->IsChildOf(ACharacter::StaticClass())) {
                DetectedActors.Add(OutHit.GetActor());
            }

            if (OutHit.bBlockingHit) {

                DrawDebugPoint(
                        World,
                        OutHit.ImpactPoint,
                        10,  //size
                        FColor(255, 0, 255),
                        false,  //persistent (never goes away)
                        0.1  //point leaves a trail on moving object
                );
            }
        }
    }

    if (DetectedActors.Num() > 0) {
        auto Stream = GetDataStream(*this);
        Stream.Send(*this, GetEpisode(), DetectedActors);
    }
}
