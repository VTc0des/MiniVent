#ifdef TST

#include "constants.h"
#include "pins.h"
#include <Wire.h>
#include <Servo.h>
#include <trajfactory.h>

// Address of slave.
#define SLAVE_ADDR 8

// Pin definitions.
#define SERVO_PIN 9

// Limits of servo.
#define SERVO_MIN 2400
#define SERVO_MAX 550

// Pins for MC connections
#define F2_TO_F1 2
#define F1_TO_F2 13

// Init trajectory generation and current pointer.
TrajFactory tf = TrajFactory();
Trajectory* traj_ptr = 0;

// Init servo class.
Servo servo;

// Init current ventilator params.
int rr;
float ie;
int setpoint;
float hold;
int delta_t;
char state;

// Init I-Hold params
bool iholdComplete;
int currIHoldStep;
unsigned long startIHoldTime;

void moveTo(int pos, int delta_t){

  // Write current position instruction to console.
  Serial.println(pos);

  // Move the servos to next setpoint 
  servo.writeMicroseconds(SERVO_MIN-pos);

  // Wait delta time.
  delay(delta_t);
}

void stop(){

  // If we have already generated a trajectory, follow it till the end.
  if (traj_ptr != 0) {
    while (traj_ptr->getCurrentStep() != 0) {
      moveTo(traj_ptr->nextStep(), traj_ptr->getDeltaTime());
    }
  // If not, got to 0 position.
  } else {
    moveTo(0, 100);
  }
}

void recieveTraj(int num_entries) {

  // Read the state from incoming transmission.
  char recieved_state = Wire.read();

  // Either change machine state or load new settings.
  switch (recieved_state) {
    case 'X':
      state = 'X';
      break;
    case 'L':
      if (state !='I') {
        state = 'L';
      }
      
      // Load new params
      byte high = Wire.read();
      byte low = Wire.read();
      setpoint = (high << 8) | low;
      Serial.print("setpoint = ");
      Serial.println(setpoint);

      rr = Wire.read();

      byte inhale = Wire.read();
      byte exhale = Wire.read();
      ie = float(inhale) / exhale;

      byte hold_s = Wire.read();
      byte hold_dec = Wire.read();
      hold = hold_s + hold_dec/100.0;

      delta_t = Wire.read();

      break;
  }
}

void setup() {

  // Join i2c bus with address SLAVE_ADDR.
  Wire.begin(SLAVE_ADDR);

  // Register callback function for i2c receive.
  Wire.onReceive(recieveTraj);

  // Start serial output for trajectory monitoring.
  Serial.begin(9600);

  // Attach servo.
  servo.attach(SERVO_PIN);

  // Initialize I-Hold parameters
  iholdComplete = false;

  /// Congifure pinMode for communicating F1 <-> F2;
  pinMode(F2_TO_F1, INPUT);
  pinMode(F1_TO_F2, OUTPUT);

  // Set state to off.
  state = 'X';
}

void loop() {
  //if you are not already in ihold or load states, set state to begin ihold if button push is recieved. 
  if (digitalRead(F2_TO_F1) == HIGH && state != 'I' && state != 'C' && state != 'L') {
    state = 'C';
    //state C for confirm ihold
  } 

  // Run device in different modes.
  switch (state) {
    // Device is on, continue following trajectory.
    case 'O': 
      moveTo(traj_ptr->nextStep(), traj_ptr->getDeltaTime());
      break;

    // Device has been instructed to shutdown.
    case 'X':
      stop();
      break;

    // Device has recieved a new trajectory, load and start.
    case 'L':

      // Stop motion.
      stop();

      // If assigned ptr, delete contents
      if (traj_ptr != 0) {
        delete traj_ptr;
      }

      // Build new trajectory
      traj_ptr = tf.build(rr, ie, setpoint, hold, delta_t);

      // Set device on.
      state = 'O';
      break;

    // Device has been instructed to maintain inspiratory hold manuever
    case 'I':

      currIHoldStep = traj_ptr->nextStep();
      moveTo(currIHoldStep, traj_ptr->getDeltaTime());

      // Send signal to F2 that ihold initiated.
      if (currIHoldStep == setpoint) {
        digitalWrite(F1_TO_F2, HIGH);
        
        // Start clock on IHold
        startIHoldTime = millis();

        // Check if 0.5s have passed in while loop. 
        while (millis() - startIHoldTime <= IHOLD_TIME*10000) {
          iholdComplete = true;
        } //end of while
      } 
      else {
        digitalWrite(F1_TO_F2, LOW);
        if (iholdComplete){
          iholdComplete = false;
          state = 'L';
        }
      }
      break;

    // Device has recieved confirm ihold state, load ihold trajectory then start
    case 'C':

      Serial.println("IN CASE C");

      // Stop motion.
      stop();

      if (traj_ptr != 0){
        delete traj_ptr;
      }
      
      // Build new trajectory
      traj_ptr = tf.build(rr, ie, setpoint, IHOLD_TIME, delta_t);

      // Set device on.
      state = 'I';
      break;
  }
} //end of loop

#endif
