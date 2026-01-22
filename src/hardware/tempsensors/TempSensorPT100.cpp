/**
 * @file TempSensorPT100.cpp
 *
 * @brief Handler for PTT100 sensor with MAX 31865 board
 */

#include "TempSensorPT100.h"
#include "Logger.h"

#define INITIAL_CHANGERATE 200
#define RUNTIME_CHANGERATE 5

float RREF = 430.0;

TempSensorPT100::TempSensorPT100(const int GPIOPin) {
    // Initialize MAX31865
    LOGF(WARNING, "GPIOPin %0i", GPIOPin);
    PT100Sensor_ = new Adafruit_MAX31865(GPIOPin, 3, 4, 5);
    // Start sampling the PT100 sensor (3 wire)
    PT100Sensor_->begin(MAX31865_3WIRE);
}

bool TempSensorPT100::sample_temperature(double& temperature) const {

    float temp = PT100Sensor_->temperature(100, RREF);

    // First check for faults w/ MAX 31865
    uint8_t fault = PT100Sensor_->readFault();
    
    if (fault) {
        Serial.print("Fault 0x"); Serial.println(fault, HEX);
        if (fault & MAX31865_FAULT_HIGHTHRESH) {
            LOGF(WARNING, "RTD High Threshold");
            return false;
        }
        if (fault & MAX31865_FAULT_LOWTHRESH) {
            LOGF(WARNING, "RTD Low Threshold");
            return false;
        }
        if (fault & MAX31865_FAULT_REFINLOW) {
            LOGF(WARNING, "REFIN- > 0.85 x Bias"); 
            return false;
        }
        if (fault & MAX31865_FAULT_REFINHIGH) {
            LOGF(WARNING, "REFIN- < 0.85 x Bias - FORCE- open");
            return false;
        }
        if (fault & MAX31865_FAULT_RTDINLOW) {
            LOGF(WARNING, "RTDIN- < 0.85 x Bias - FORCE- open");
            return false; 
        }
        if (fault & MAX31865_FAULT_OVUV) {
            LOGF(WARNING, "Under/Over voltage"); 
            return false;
        }
        PT100Sensor_->clearFault();
    }
    if (temp < 0.0 || temp > 180.0)  {
        LOGF(WARNING, "Temperature reading not within 0 - 180°C range: %0.01f°C", temp);
        return false;
    }
    // All fine, return temp:
    temperature = temp;

    return true;
}
