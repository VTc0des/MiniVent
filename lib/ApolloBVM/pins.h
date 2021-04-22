// Address of slave.
#define SLAVE_ADDR 8

// Pin definitions (digital).
//on main MC
#define DISPLAY_PIN 11
#define ENC_CLK_PIN 6
#define ENC_DT_PIN 5
#define ENC_BUTTON_PIN 7
#define STOP_BUTTON_PIN 8
// TODO: Change these pins to the right ones
#define LIMIT_RIGHT_PIN 20
#define LIMIT_LEFT_PIN 21
// TODO: Change this to an available pin.
#define NEAR_PIN 22
// TODO: Change this to a PWM functioning pin.
#define BUZZER_PIN 23

//on alarm MC
//for rgb display
#define RS 7
#define EN 8
#define D4 9
#define D5 10
#define D6 11
#define D7 12
#define RED_LIGHT 3   //backlit red
#define GREEN_LIGHT 5 //backlit green
#define BLUE_LIGHT 6  //backlit blue

//Digital Pin Definitions for I-HOLD Maneuver
#define IHOLD_BUTTON_PIN 4
// #define F2_TO_F1  2
// #define F1_TO_F2 13

// Pin definitions (analog).
//LEDs
#define ALM_HIGH A3
#define ALM_LOW  A4
#define ALM_INSP A5

// Pressure Sensor
#define PS_Vout A2    //pin 3 on pressure sensor