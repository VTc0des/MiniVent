#ifdef PST

/*Credit for the filtering algorithm:
 * https://www.norwegiancreations.com/2016/03/arduino-tutorial-simple-high-pass-band-pass-and-band-stop-filtering/
 */

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define REDLITE 3
#define GREENLITE 5
#define BLUELITE 6

#define AWAKE_ALARM A3
#define PEEP_ALARM A4
#define PIP_ALARM A5

#define PS_PIN A2
#define SAMPLE_RATE 20    //sampling rate of 20 ms
#define SAMPLE_SIZE 100

#define F2_TO_F1 2
#define F1_TO_F2 13

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// you can change the overall brightness by range 0 -> 255
const uint8_t brightness = 255;

const uint8_t led_on = 255;
const uint8_t led_off = 0;

const float mBar_cmH2O = 1.01972;

uint8_t i;

int readPressure;
int savePressure[SAMPLE_SIZE];

float EMA_a_low = 0.7; //initialization of EMA alpha
int EMA_S_low = 0; //initialization of EMA S

unsigned long currentRead;
unsigned long previousRead;
double baseline;

float pip;
float peep;
float pp;
float max_pip = 35; //35 cmH2O
//const String display_lines[3] = {"PIP = ", "PEEP = ", "PP = "};

void printWarning();
void startDisplay();
void read_filter();
void calculatePIP();
void displayPIP();
void setBacklight(uint8_t r, uint8_t g, uint8_t b);

void setup()
{
  Serial.begin(9600); //baudrate 9600

  lcd.begin(20, 4);

  // Congifure pinMode for communicating F2 <-> F1;
  pinMode(F2_TO_F1, OUTPUT);
  pinMode(F1_TO_F2, INPUT);

  printWarning();
  startDisplay();

  //initialize variables
  pip = 0;
  peep = 0;
  pp = 0;
  i = 0;

  baseline = analogRead(PS_PIN); // obtains first value once start up
  // baseline = (baseline * 100 * mBar_cmH2O) / 1023;
  //filter raw data
//  EMA_S_low = ((EMA_a_low * baseline) + ((1 - EMA_a_low) * EMA_S_low)); //run the EMA

  EMA_S_low = baseline; //set EMA_S_low for t=0

  previousRead = millis();
}

void loop()
{
  read_filter();

  //if all samples have been collected, reset counter, process and display
  if (i == SAMPLE_SIZE)
  {
    i = 0; //reset i to 0.
    Serial.println("CALCULATING");
    calculatePIP();
    displayPIP();
  }
}

void printWarning() {
  setBacklight(0, 128, 128);
  lcd.setCursor(6, 1);
  lcd.print("MINIVENT");
  delay(2000);
  lcd.clear();
  lcd.setCursor(6, 1);
  lcd.print("WARNING");
  lcd.setCursor(1, 2);
  lcd.print("USE PEDI SIZED BAG");
  delay(2000);
  lcd.clear();
}

void startDisplay() {
  setBacklight(0, 128, 128);
  lcd.setCursor(0, 1);
  lcd.print("PIP = ");
  lcd.setCursor(0, 2);
  lcd.print("PEEP = ");
  lcd.setCursor(0, 3);
  lcd.print("PP = ");
}

void read_filter() {
  //sample pressure
  currentRead = millis();
  if (currentRead - previousRead > SAMPLE_RATE)
  {
    previousRead = currentRead;
    readPressure = (analogRead(PS_PIN));

    //filter raw data
    EMA_S_low = ((EMA_a_low * readPressure) + ((1 - EMA_a_low) * EMA_S_low)); //run the EMA

    savePressure[i] = EMA_S_low;

//    Serial.print("Baseline:");
//    Serial.print(baseline);
//    Serial.print(" ");
//    Serial.print("Pressure:");
    Serial.print("Pressure,");
    Serial.println(savePressure[i]);

    //increment counter
    i++;
  }
}

void calculatePIP() {
  int maxValue = savePressure[0]; //save first value in array
  for (uint8_t j = 0; j < SAMPLE_SIZE; j++) {
    if (savePressure[j] > maxValue) {
      maxValue = savePressure[j];
    }
  }

  //  Serial.print("Max Value");
  //  Serial.println(maxValue);
  //  Serial.print("ADC PIP Value ");
  //  Serial.println((maxValue - baseline));
  Serial.print("mx value,");
  Serial.println(maxValue - baseline);
  //convert to voltage output, then to mBar, then to cmH2O
  pip = ((maxValue - baseline) * 100 * mBar_cmH2O) / 1023;
//  pip = (maxValue * 100 * mBar_cmH2O) / 1023;
  

//    Serial.print("PIP");
//    Serial.println(pip);
}

void displayPIP() {
  if (pip > max_pip) {
    //print to first line
//    lcd.setCursor(0, 0);
//    lcd.print("ALERT: HIGH IP");
    setBacklight(255, 0, 0);
    analogWrite(PIP_ALARM, led_on);
  }
//  else if (pip < baseline) {
//    //print to first line
////    lcd.setCursor(0, 0);
////    lcd.print("ALERT: AWAKE");
//    setBacklight(255, 0, 0);
//    analogWrite(AWAKE_ALARM, led_on);
//  }
  else {
    lcd.setCursor(4, 0);
    lcd.print("VENTILATING");
//    lcd.print("BASE = ");
//    lcd.setCursor(8, 0);
//    lcd.print(baseline);
    setBacklight(0, 255, 0);
    analogWrite(PIP_ALARM, led_off);
    analogWrite(AWAKE_ALARM, led_off);
  }

  lcd.setCursor(8, 1);
  lcd.print(pip);
  lcd.setCursor(8, 2);
  lcd.print(peep);
  lcd.setCursor(8, 3);
  lcd.print(pp);
}

void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!
  // r = map(r, 0, 255, 0, 100);
  // g = map(g, 0, 255, 0, 150);

  r = map(r, 0, 255, 0, 255);
  g = map(g, 0, 255, 0, 255);

  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);

  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);
  //  Serial.print("R = "); Serial.print(r, DEC);
  //  Serial.print(" G = "); Serial.print(g, DEC);
  //  Serial.print(" B = "); Serial.println(b, DEC);
  analogWrite(REDLITE, r);
  analogWrite(GREENLITE, g);
  analogWrite(BLUELITE, b);
}

#endif