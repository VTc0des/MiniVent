#ifndef pressuremanager_h
#define pressuremanager_h

#include <Arduino.h>


class PressureManager {
public:
    PressureManager(int reading, uint8_t pressure_pin);

    void calibrate(); //is calibrate for baseline 
    void poll();
    void smooth(); //try to replace with hardware filters, will save memory space and improve processing speed. 
    void peakDetection();

private:

    //poll for a new pressure sample every 20 ms
    const uint8_t _sample_rate = 20;
    
    //pin location for the pressure sensor. 
    uint8_t _pressure_pin;

    //will it a problem if these variables are the same as alarmdisplay.h
    float _pip;
    float _peep;
    float _pp;
};

#endif