# esp32_mpu
ESP32 hardware abstraction layer (hal) and example (main/main.c) for TKD InvenSense MPU chips.

![C/C++ CI](https://github.com/sebastianPsm/esp32_mpu/workflows/C/C++%20CI/badge.svg)

There are several MPU libraries for microcontroller (like the ESP32) avaiable (like [this](https://github.com/rupin/SparkFun_MPU-9250-DMP_Arduino_Library) or [that](https://github.com/natanaeljr/esp32-MPU-driver)), but this version here uses the official motion driver from TKD InvenSense [eMD 6.12](https://invensense.tdk.com/developers/software-downloads) and adds only a tiny hardware abstraction layer. Further, everything is nicely organized in components/mpu and main/main.c is just the example.

## Requirements

- [eMD 6.12](https://invensense.tdk.com/developers/software-downloads)
- espressiv idf

## How to use

1. Copy the components/mpu directory into your project directory.
2. Download the motion driver [eMD 6.12](https://invensense.tdk.com/developers/software-downloads) and extract the content into components/mpu
3. Apply components/mpu/patch.diff (e.g. with: `git apply -p1 --reject --whitespace=fix components/mpu/patch.diff`)
  - I discovered, that this step will not always work straightforward. However, the patch file isn't that big and you can apply the changes manually.
4. Select the right MPU chip (idf.py mencuconfig --> Component config --> motion_driver --> MPU chip model)

See the following Sketch as wiring example

## Sketch

![Sketch](https://github.com/sebastianPsm/esp32_mpu/raw/master/img/sketch.png)

## Demo video

[Application video](https://github.com/sebastianPsm/esp32_mpu/raw/master/video/demo.mp4)
