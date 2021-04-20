#ifndef pressuremanager_h
#define pressuremanager_h

#include <Arduino.h>


class PressureManager {
public:
    PressureManager(double reading);

    void start(); //is calibrate for baseline 
    void poll();
    void smooth(); //try to replace with hardware filters, will save memory space and improve processing speed. 
    void peakDetection();

private:
    // variables to hold values that are outputted to the display
    //put these floats in pressure manager, then pass them to the update functions
    float _pip;
    float _peep;
    float _pp;
};

#endif