#ifndef pressuremanager_h
#define pressuremanager_h

#include <Arduino.h>
#include "constants.h"

class PressureManager {
public:
    PressureManager(uint8_t pressure_pin);
    
    void calibrate(); 
    void poll(); 
    float ipProcess();
    float epProcess();
    float ppProcess();

private:
    const uint8_t _sample_rate = 20;        //poll for a new pressure sample every 20 ms
    const float _mBar_cmH2O = 1.01972;     //conversion factor from mBar to cmH2O to display to the screen 
    const float _EMA_a_low = 0.7;          //initialization of EMA alpha
    const uint8_t _bufferLength = BUFFER_LENGTH;

    //pin location for the pressure sensor. 
    uint8_t _pressure_pin;
    uint8_t _counter = 0;
    
    //variables to hold baseline and current reading
    int _baseline = 0;
    int _pressureReading = 0;
    int _maxVal = 0;

    int _EMA_S_low = 0;     //initialization of EMA S

    //variables to track time
    unsigned long _currentRead;
    unsigned long _previousRead;

    float _pip = 0.0;
    float _peep = 0.0;
    float _pp = 0.0;
};

#endif