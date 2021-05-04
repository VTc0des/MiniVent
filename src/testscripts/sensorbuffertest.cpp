#ifdef SBTST
#include "sensorbuffer.h" //include alarmdisplay.h and alarmproperties.h separetly to avoid cyclic inclusion of libraries.
#include <Arduino.h>
#include <Wire.h>

#define PS_PIN A2

const uint8_t _sample_rate = 20;   //poll for a new pressure sample every 20 ms
const float _EMA_a_low = 0.7;      //initialization of EMA alpha
const uint8_t _bufferLength = 100;

//variables to hold baseline and current reading
int _baseline = 0;
int _pressureReading = 0;

int _EMA_S_low = 0; //initialization of EMA S

//variables to track time
unsigned long _currentRead;
unsigned long _previousRead;
//pin location for the pressure sensor.
uint8_t _pressure_pin = PS_PIN;

SensorBuffer sb(_bufferLength);

void setup() {
  Serial.begin(9600);

  for (uint16_t i = 0; i < _bufferLength; i++) {
    sb.putBack(0);
  }

  Serial.println("Populated SensorBuffer...");
  Serial.println("Printing contents of SensorBuffer..");
  _EMA_S_low = analogRead(_pressure_pin);
  _baseline = _EMA_S_low;
  _previousRead = millis();
}

void loop() {

  for (uint16_t i = 0; i < _bufferLength; i++) {
    Serial.print("i: ");
    Serial.print(i);
    Serial.print(" sb: ");
    Serial.println(sb.get(i));
  }

    // Add another break.
  Serial.println();
  _currentRead = millis();
    if (_currentRead - _previousRead > _sample_rate) {
        _previousRead = _currentRead;
        _pressureReading = analogRead(_pressure_pin);

        //filter raw data
        _EMA_S_low = ((_EMA_a_low * _pressureReading) + ((1 - _EMA_a_low) * _EMA_S_low)); //run the EMA

        //put it into the array
        sb.putBack(_EMA_S_low);

        //check if current val > previous val
        if (sb.get(_bufferLength-1) > sb.get(_bufferLength-2)) {
          Serial.println("increasing");
        }
        else if (sb.get(_bufferLength-1) < sb.get(_bufferLength-2)) {
          Serial.println("decreasing");
        }
        else {
          Serial.println("no change");
        }
    }
}

#endif
