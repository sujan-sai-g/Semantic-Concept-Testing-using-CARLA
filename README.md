# Semantic-Concept-Testing-using-CARLA
Semantic Concept Testing in Autonomous Driving by Extraction of Object-Level Annotations from CARLA

# Steps for adding the Pedestrian detection sensor 

# Prerequisites
-Setup CARLA simulator v0.9.11 on linux

-Run make package and make PythonAPI.

-Launch server + manual_control.py to check the base version is running 

# Add the following files to provided folders

PedDetectionSensor.cpp, PedDetectionSensor.h - carla_base_dir/Unreal/CarlaUE4/Plugins/Carla/Source/Carla/Sensor

PedDetectorSerializer.cpp, PedDetectorSerializer.h - carla_base_dir/LibCarla/source/carla/sensor/s11n

PedDetectEvent.h - carla_base_dir/LibCarla/source/carla/sensor/data/

### (since only parts of these files are changed, you can alternatively just copy the modifications)
ActorBlueprintFunctionLibrary.cpp, ActorBlueprintFunctionLibrary.h - carla_base_dir/Unreal/CarlaUE4/Plugins/Carla/Source/Carla/Actor/ 

SensorRegistry.h - carla_base_dir/LibCarla/source/carla/sensor/

SensorData.cpp - carla_base_dir/PythonAPI/carla/source/libcarla/


# Python API usage example
```
blueprint_library = world.get_blueprint_library()
ped_detector_bp = blueprint_library.find('sensor.other.ped_detector')

ped_detector_bp.set_attribute('horizontal_fov', str(90))  
ped_detector_bp.set_attribute('vertical_fov', str(50))  
ped_detector_bp.set_attribute('points_per_second', str(200000))
ped_detector_bp.set_attribute('range', str(200))
ped_detector_bp.set_attribute('lower_y_distance', str(5000))
ped_detector_bp.set_attribute('upper_y_distance', str(400))

ped_detector_transform = # define transform
ped_detector_sensor = world.spawn_actor(ped_detector_bp, ped_detector_transform, attach_to=self.player)

```




# License
Copyright (c) 2021 Fraunhofer IAIS. All rights reserved.

This repository is licensed under the MIT license. See LICENSE.txt for the full license text.

This was developed as part of the [KI-Absicherung](https://www.ki-absicherung-projekt.de/) project by Sujan Sai Gannamaneni (Fraunhofer IAIS).
