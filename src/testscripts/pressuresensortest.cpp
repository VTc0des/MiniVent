#include <Arduino.h>


#define pressureSignalPin A1 //need
#define sampleRate 20 //sampling rate of 20 ms
int i;

double readPressure;
double savePressure[50];

float EMA_a_low = 0.5;    //initialization of EMA alpha

int EMA_S_low = 0;        //initialization of EMA S

unsigned long currentRead;
unsigned long previousRead;
double baseline;

void setup() {
  Serial.begin(9600); //baudrate 9600

  baseline = analogRead(pressureSignalPin); // obtains first value once start up

  //initialize variables
  i = 0;

  previousRead = millis();
}

void loop() {
  //sample pressure
  currentRead = millis();
  if (currentRead - previousRead > sampleRate)
  {
    previousRead = currentRead;
    readPressure = (analogRead(pressureSignalPin));

    //save pressure
    savePressure[i] = readPressure;
    Serial.print("Raw:");
    Serial.print(readPressure);

    i++;

    EMA_S_low = (EMA_a_low * readPressure) + ((1 - EMA_a_low) * EMA_S_low); //run the EMA
    
    Serial.print(" ");
    Serial.print("Filter:");
    Serial.println(EMA_S_low);
  }

  if (i == 50) {
    i = 0; //reset i to 0.
  }
}