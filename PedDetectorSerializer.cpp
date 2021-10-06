//
// Created by sujan on 19.01.21.
//


#include "carla/sensor/s11n/PedDetectorSerializer.h"

#include "carla/sensor/data/PedDetectEvent.h"

namespace carla {
    namespace sensor {
        namespace s11n {

            SharedPtr<SensorData> PedDetectorSerializer::Deserialize(RawData &&data) {
                return SharedPtr<SensorData>(new data::PedDetectEvent(std::move(data)));
            }

        } // namespace s11n
    } // namespace sensor
} // namespace carla