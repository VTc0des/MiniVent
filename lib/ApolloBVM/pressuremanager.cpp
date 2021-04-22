#include "pressuremanager.h"

PressureManager::PressureManager(int reading, uint8_t pressure_pin) : _pressure_pin(pressure_pin) {};

void PressureManager::calibrate() {

}

void PressureManager::poll(){
    //if sufficient time has passed, then poll the pressure sensor
    //save the value into the sensor buffer
    analogRead(_pressure_pin);
}

void PressureManager::peakDetection() {
    
}