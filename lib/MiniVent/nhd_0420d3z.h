#ifndef nhd_0420d3z_h
#define nhd_0420d3z_h
#include "Arduino.h"

#ifdef SAMD
#include "wiring_private.h"
#else
#include "SoftwareSerial.h"
#endif

using byte = unsigned char;

class NhdDisplay {

public:
  NhdDisplay(byte pin);

  // Methods for beggining serial communication.
  void begin(int baud);

  // Methods for printing.
  void print(String st);
  void print(int val);
  void println(String st);

  // Methods for display adjustments.
  void setBacklight(byte brightness);
  void clearDisplay();
  void setCursor(byte cursor_position, byte line);

  // Methods for underline.
  void underlineOn();
  void underlineOff();

  // Methods for blinking.
  void blinkingOn();
  void blinkingOff();

  // Methods for single character shifting.
  void moveCursorRight();
  void moveCursorLeft();

  // Methods for deleting characters.
  void backspace();
  void remove();

  // Helper function to pad integers to 2 digits.
  static String zeroPad(int in) {
    // This helper function ensures integers have 2 decimal places.

    // Make a temp char array to hold the 2 digits.
    char digit[4];

    // Pad with a 0 if integer is 1 digit.
    sprintf(digit, "%02d", in);

    return String(digit);
  }

private:
#ifdef SAMD
  Uart *_serial_ptr;
#else
  SoftwareSerial *_serial_ptr;
#endif

  // Pointer to serial interface.

  // Current row and column of the cursor.
  byte _row;
  byte _col;
};
#endif
