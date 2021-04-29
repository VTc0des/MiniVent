#include "pressuremanager.h"
#include "sensorbuffer.h"

SensorBuffer sb(BUFFER_LENGTH);

PressureManager::PressureManager(uint8_t pressure_pin) : _pressure_pin(pressure_pin) {};

void PressureManager::calibrate() {
    //intialize sensor buffer with buffer length
    _EMA_S_low = analogRead(_pressure_pin);
    _baseline = _EMA_S_low;
    _previousRead = millis();
}

void PressureManager::poll() {
    //if sufficient time has passed, then poll the pressure sensor
    //save the value into the sensor buffer
    _currentRead = millis();
    if (_currentRead - _previousRead > _sample_rate) {
        _pressureReading = analogRead(_pressure_pin);

        //filter raw data
        _EMA_S_low = ((_EMA_a_low * _pressureReading) + ((1 - _EMA_a_low) * _EMA_S_low)); //run the EMA

        //put it into the array
        sb.putBack(_EMA_S_low);
        _counter++;
    }

    if (_counter == _bufferLength) {
        //reset buffer length
        _counter = 0;
    }
}

float PressureManager::ipProcess() {
    //compare current value to previous value
    //find the maximum value. when it is the global maximum, convert it to other units
    if (_counter == 0) {
        _maxVal = sb.get(_counter);
    }
    else if (_counter < _bufferLength) {
        if (sb.get(_counter) > _maxVal) {
            _maxVal = sb.get(_counter);
        }
    }
    else if (_counter == _bufferLength) {
        _pip = ((_maxVal-_baseline)*100*_mBar_cmH2O)/1023;
        return _pip;
    }
}

float PressureManager::epProcess() {
    return _peep;
}

float PressureManager::ppProcess() {
    return _pp;
}