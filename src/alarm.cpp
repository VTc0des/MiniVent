#ifdef ALM
#include <Arduino.h>
#include "Wire.h"
#include "ventproperties.h"
#include "panel.h"

// Initialize ventilator state object.
VentIO vio;

//Initialize alarm settings object
AlarmSettings as = {false, false, false, 0, 0, 0};

//Initialize sensor state object
SensorParameters sp = {0,0,0};

//initialize time constants
TimeConsts tc  = {0,0,0,0,0,0,0,0, 10000, 500, 4000}; 

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
int brightness = 255;
String warning[3] = {"      MiniVent", "    ALARM PANEL", "CHECK SENSOR MODULE."};
String displayParams[3] = {"PIP = ", "PEEP = ", "PP = "};

//declare all functions
void printWarning();
void setBacklight(uint8_t r, uint8_t g, uint8_t b);
void readPressure();
void ihold();
void algorithm();
void indicateLED();
void displayAlert();
void updateDisplay();

void setup ()    
{
    Serial.begin(9600);
    lcd.begin(20,4);
    //set LED pins as outputs
    vio.LED(); 
    printWarning();
    //CALIBRATE
}

void loop ()
{
    //poll button for I-HOLD input
    vio.ihold_button.poll();

    //if button pushed
    if (vio.ihold_button.getButtonState())
    {
        //send signal to master microcontroller
        analogWrite(ihold_main, 255); //send high signal to master
        //if signal comes back from master indicating the 
        //motors are at the beginning of the inspiratory cycle go to the function
        if (map(analogRead(ihold_alarm), 0, 1023, 0, 255))
        {
            ihold();
        } 
    }
    else {
        analogWrite(ihold_main, 0); //send 0 to indicate no button push
    }

    //read for set amount of time.
    readPressure();

    /*tc.currihold = millis(); //take current time. 
    if (tc.currihold - tc.previhold >= tc.inspHold)
    {
        tc.previhold = tc.currihold;
        readPressure();
    }*/
}

void printWarning()
{
    lcd.clear();
    setBacklight(0,128,128); //set blue-green color for display
    for (int i = 0; i < 3; i++)
    {
        lcd.setCursor(0, i+1);
        lcd.print(warning[i]);
    }
    
    delay(tc.displayTime); //display for 2 seconds
    lcd.clear();
}

void setBacklight(uint8_t r, uint8_t g, uint8_t b)
{
    // normalize the red LED - its brighter than the rest!
    r = map(r, 0, 255, 0, 100);
    g = map(g, 0, 255, 0, 150);

    r = map(r, 0, 255, 0, brightness);
    g = map(g, 0, 255, 0, brightness);
    b = map(b, 0, 255, 0, brightness);

    // common anode so invert!
    r = map(r, 0, 255, 255, 0);
    g = map(g, 0, 255, 255, 0);
    b = map(b, 0, 255, 255, 0);

    analogWrite(RED_LIGHT, r);
    analogWrite(GREEN_LIGHT, g);
    analogWrite(BLUE_LIGHT, b);
}

void readPressure()
{
    algorithm();
}

void ihold()
{
    lcd.clear();
    setBacklight(0,128,128); //set blue-green color for display
    lcd.setCursor(0, 1);
    lcd.print(" I-HOLD IN PROGRESS");
    //while (inspiratory hold period not complete.)
    //{

    //}
    readPressure();
    //turn flags on for testing. 
}

void algorithm()
{
    //smooth data
    //find where concave for PIP 
    //find where convex for PEEP
    //save the PIP and PEEP value into a variable
    //raise flag if PIP, PEEP and breathing initated exceed the appropriate threshold 
    
    //if you have found PIP, PEEP etc. then send to indicateLED();
    indicateLED();
}

void indicateLED()
{
    if (as.highP)
    { //turn on hgih pressure LED
        digitalWrite(ALM_HIGH, HIGH);
    }
    else
    { //turn off LED if clear
        digitalWrite(ALM_HIGH, LOW);
    }

    if (as.lowP)
    { //turn on low pressure LED
        digitalWrite(ALM_LOW, HIGH);
    }
    else
    { //turn off LED if clear
        digitalWrite(ALM_LOW, LOW);
    }

    if (as.insp)
    { //turn on inspiraotry pressure detected LED
        digitalWrite(ALM_INSP, HIGH);
    }
    else
    { //turn off LED if clear
        digitalWrite(ALM_INSP, LOW);
    }

    if (as.highP || as.lowP || as.insp)
    { //turn backlight to RED if any flags are still active
        setBacklight(255, 0, 0);
    }
    else
    { //turn backlight to gree if all is normal
        setBacklight(0, 255, 0);
    }

    displayAlert();
    updateDisplay();
}

void displayAlert()
{
    lcd.clear();
    if (as.highP)
    {
        lcd.setCursor(0, 0);
        lcd.print("   ALERT: HIGH IP");
    }
    if (as.lowP)
    {
        lcd.setCursor(0, 0);
        lcd.print(" ALERT: HIGH PEEP");
    } //turn on greenbacklight

    if (as.insp)
    {
        lcd.setCursor(0, 0);
        lcd.print(" ALERT:INDP. BREATH");
    }
    //if no flags are activated
    if (!as.highP && !as.lowP && !as.insp)
    {
        lcd.setCursor(0, 0);
        lcd.print("     NO ALERTS");
    }
}

void updateDisplay()
{
    //print all statements
    for (int i = 0; i < 3; i++)
    {
        lcd.setCursor(0, i+1);
        switch (i)
        {
        case 0:
            lcd.print(displayParams[i]);
            break;
        case 1:
            lcd.print(displayParams[i]);
            break;
        case 2:
            lcd.print(displayParams[i]);
            break;
        }

        lcd.setCursor(8, i+1);
        switch (i)
        {
        case 0:
            lcd.print(sp.inspiratoryPressure);
            break;
        case 1:
            lcd.print(sp.expiratoryPressure);
            break;
        case 2:
            lcd.print(sp.plateauPressure);
            break;
        }
    }
    delay(tc.displayTime); //display for 2 seconds
}

#endif