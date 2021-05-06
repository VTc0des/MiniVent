#include "pressuremanager.h"
#include "sensorbuffer.h"

// SensorBuffer sb(BUFFER_LENGTH);  // circular buffer of size 100 as defined in constants.h. Circular buffer is not used in this iteration of the code but is HIGHGLY recommended for future iterations. 

PressureManager::PressureManager(uint8_t pressure_pin) : _pressure_pin(pressure_pin) {};

void PressureManager::calibrate() {
    // //intialize sensor buffer with buffer length
    // for (uint16_t i = 0; i < _bufferLength; i++) {
    //     sb.putBack(0);
    // }

    // initialize pressure array of buffer length with 0's upon calibration. 
    for (uint8_t i = 0; i < _bufferLength; i++) {
        _pressureArray[i] = 0;
    }
    
    //initialize counter variable
    _counter = 0;
    _iholdCounter = 0;
    _finalidx = (_bufferLength - 1);
    _processFlag = false;
    
    //sample baseline data point from pressure sensor
    _EMA_S_low = analogRead(_pressure_pin);

    //take first time point to sample data
    _previousRead = millis();
}

void PressureManager::poll() {
    if (_counter < _finalidx) {
        _processFlag = false;
    }
    else if (_counter == _finalidx) {
        _processFlag = true;
    }

    //if sufficient time has passed, then poll the pressure sensor
    _currentRead = millis();
    if (_currentRead - _previousRead > _sample_rate) {
        _previousRead = _currentRead;                   //update the time value 

        //apply low pass filter on raw data that is read in from the analog pin
        _EMA_S_low = ((_EMA_a_low * analogRead(_pressure_pin)) + ((1 - _EMA_a_low) * _EMA_S_low)); 

        _pressureArray[_counter] = _EMA_S_low;

        _counter++;

        //put it into the array
        // sb.putBack(_EMA_S_low);
    }

    if (_counter > _finalidx) {
    // array indices go from 0-99 for a buffer length of 100 so the reset statement must come after the 99th element is added to array. 
        _counter = 0;
    }
}

void PressureManager::iholdPoll() {
    //if its the first sample, record it and update the time so the subsequent samples can be sampled correctly. 
    if (_iholdCounter == 0) {
        _previousRead = millis();
        
        //apply low pass filter on raw data that is read in from the analog pin
        _ihold_EMA_S_low = ((_EMA_a_low * analogRead(_pressure_pin)) + ((1 - _EMA_a_low) * _ihold_EMA_S_low));

        _plateauArray[0] = _ihold_EMA_S_low;

        _iholdCounter++;
    }
    else {
        _currentRead = millis();
        if (_currentRead - _previousRead > _ihold_sample_rate) {
            _previousRead = _currentRead; //update the time value

            //apply low pass filter on raw data that is read in from the analog pin
            _ihold_EMA_S_low = ((_EMA_a_low * analogRead(_pressure_pin)) + ((1 - _EMA_a_low) * _ihold_EMA_S_low));

            _plateauArray[_iholdCounter] = _ihold_EMA_S_low;

            _iholdCounter++;
        }

        if (_iholdCounter > (_ihold_bufferLength - 1)) {
            _iholdCounter = 0;
        }
    }
    Serial.print("Pressure,");
    Serial.println(_plateauArray[_iholdCounter]);
}

double PressureManager::ipProcess() {
    if (_processFlag) {
        _maxInspVal =_pressureArray[0]; //save the first value in the array to compare. 
        
        //blocking statement but this should be relatively fast to not miss much data
        for (uint8_t i = 1; i < _bufferLength; i++) {
            if (_pressureArray[i] > _maxInspVal) {
                //replace the maximum value with the value in the array
                _maxInspVal = _pressureArray[i];
            }
        }

        //convert maximum value that is in analog units to cmH2O to output to the screen. 
        _pip = (_maxInspVal * 100 * _mBar_cmH2O) / 1023;
        return _pip;
    }
}

double PressureManager::epProcess() {
    if (_processFlag) {
        //do things to find this value and return the updated value. 
        return _peep;
    }
}

double PressureManager::ppProcess() {

    //blocking statement but this should be relatively fast to not miss much data
    for (uint8_t i = 1; i < (_ihold_bufferLength - 1); i++) {
        //local variables to save variables to compare in if statement. 
        int _previousVal = _plateauArray[i-1];
        int _lowBoundcurrentVal = 0.98*_plateauArray[i];
        int _highBoundcurrentVal = 1.02*_plateauArray[i];
        int _nextVal = _plateauArray[i+1];

        //if the magnitude of the current value is within 2% of the previous value and next value, then the waveform has plateaud. 
        if (((_previousVal >= _lowBoundcurrentVal) && (_previousVal <= _highBoundcurrentVal)) 
            && ((_nextVal >= _lowBoundcurrentVal) && (_nextVal <= _highBoundcurrentVal))) {
            //sum the plateau value 
            _platVal += _plateauArray[i];
            //update the counter to keep track of how many pleateau there are
            _numPlat++;                 
        }
        //else, keep looking for the plateau
    }

    //calculate average plateau value
    _platVal = _platVal/_numPlat;
    
    //reset counter variable
    _numPlat = 0;

    //convert plateau value that is in analog units to cmH2O to output to the screen.
    _pp = (_platVal * 100 * _mBar_cmH2O) / 1023;

    
    Serial.print("Pleateau Pressure,");
    Serial.println(_pp);
    
    return _pp;
}