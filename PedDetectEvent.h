//
// Created by sujan on 19.01.21.
//

#pragma once

#include "carla/rpc/ActorId.h"
#include "carla/sensor/data/Array.h"
#include "carla/sensor/s11n/PedDetectorSerializer.h"

namespace carla {
    namespace sensor {
        namespace data {

            class PedDetectEvent : public Array<rpc::ActorId> {
                using Super = Array<rpc::ActorId>; // modified

            public:
//                using Serializer = s11n::PedDetectorSerializer;

                explicit PedDetectEvent(RawData &&data)
                        : Super(0u, std::move(data)) {}
            };

        } // namespace data
    } // namespace sensor
} // namespace carla