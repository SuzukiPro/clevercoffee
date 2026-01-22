/**
 * @file TempSensorPT100.h
 *
 * @brief Handler for PT100 temperature sensor
 */

#pragma once

#include "TempSensor.h"
#include <Adafruit_MAX31865.h>

class TempSensorPT100 final : public TempSensor {
    public:
        explicit TempSensorPT100(int GPIOPin);

    protected:
        bool sample_temperature(double& temperature) const override;

    private:
        Adafruit_MAX31865* PT100Sensor_;
};