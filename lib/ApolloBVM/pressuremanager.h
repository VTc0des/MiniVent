#ifndef pressuremanager_h
#define pressuremanager_h

#include <Arduino.h>
#include "constants.h"

class PressureManager {
public:
    PressureManager(uint8_t pressure_pin);
    
    void calibrate(); 
    void poll(); 
    void iholdPoll();
    float ipProcess();
    float epProcess();
    float ppProcess();

private:
    const uint8_t _sample_rate = 20;        //poll for a new pressure sample every 20 ms
    const uint8_t _ihold_sample_rate = 10;  //ihold poll is faster to try to grab more data points. 
    const uint8_t _bufferLength = BUFFER_LENGTH;
    const uint8_t _ihold_bufferLength = IHOLD_BUFFER_LENGTH;
    const float _mBar_cmH2O = 1.02; //conversion factor from mBar to cmH2O to display to the screen with 2 significant decimal places.  actual value = 1.01972
    const float _EMA_a_low = 0.7;   //initialization of EMA alpha

    //pin location for the pressure sensor.
    uint8_t _pressure_pin;

    uint8_t _counter;
    uint8_t _iholdCounter;
    uint8_t _finalidx;  //stores the final index of the pressure array. 
    bool _processFlag;  //flag variable to indicate the array is full and the maximum/minimum can be found. 
    
    //dynamic allocation of pressure readings that are overwritten every 100 samples OR however long buffer_length is defined to be. 
    int _pressureArray[BUFFER_LENGTH];
    int _plateauArray[IHOLD_BUFFER_LENGTH];              //array variable to store the pressure readings during the inspiratory hold ONLY. 
    int _EMA_S_low;     
    int _ihold_EMA_S_low;

    //variables to store analog values of process functions
    int _maxInspVal;
    int _platVal;
    int _numPlat;

    //variables to track time
    unsigned long _currentRead;
    unsigned long _previousRead;

    float _pip = 0.0;
    float _peep = 3.0;      //set to 3 during testing to avoid setting off low peep alarm. 
    // float _peep = 0.0;   //uncomment ln 40 and comment ln 39 once _peep functionality is implemented
    float _pp = 0.0;
};

#endif

/*NOTES:
The software filtering equation and flow was pulled from: https://www.norwegiancreations.com/2016/03/arduino-tutorial-simple-high-pass-band-pass-and-band-stop-filtering/
EMA stands for exponential moving average filter. it is still recommended that hardware filters be implemented into the circuit. 
*/