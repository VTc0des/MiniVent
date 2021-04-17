#include "nhd_0420d3z.h"
#include "Arduino.h"

NhdDisplay::NhdDisplay(byte pin) : _row(0), _col(0) {

// Create a new hardware serial if we're on SAMD architecture.
#ifdef SAMD
  _serial_ptr = new Uart(&sercom3, 0, 1, SERCOM_RX_PAD_1, UART_TX_PAD_0);
#else
  _serial_ptr = new SoftwareSerial(2, pin);
#endif
}

void NhdDisplay::begin(int baud) {
  // Start Serial connection with default baud.
  _serial_ptr->begin(9600);

#ifdef SAMD
  pinPeripheral(0, PIO_SERCOM); // Assign RX function to pin 0
  pinPeripheral(1, PIO_SERCOM); // Assign TX function to pin 1
#endif
}

void NhdDisplay::print(String st) { _serial_ptr->print(st); }

void NhdDisplay::print(int val) { _serial_ptr->print(String(val)); }

void NhdDisplay::println(String st) { _serial_ptr->println(st); }

void NhdDisplay::setBacklight(byte brightness) {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command for setting brightness.
  _serial_ptr->write(0x53);
  // Send brightness byte.
  _serial_ptr->write(brightness);
}

void NhdDisplay::clearDisplay() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to clear the screen.
  _serial_ptr->write(0x51);
}

void NhdDisplay::setCursor(byte cursor_position, byte line) {

  byte to_write = cursor_position;

  // Change the write value based on the line.
  // This is needed as the display write line 1 to 3 to 2 to 4.
  switch (line) {
  case 1:
    to_write += 0x40;
    break;
  case 2:
    to_write += 0x14;
    break;
  case 3:
    to_write += 0x54;
    break;
  }

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to move the cursor.
  _serial_ptr->write(0x45);
  // Send byte for cursor position.
  _serial_ptr->write(to_write);
}

void NhdDisplay::underlineOn() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to turn underline on.
  _serial_ptr->write(0x47);
}

void NhdDisplay::underlineOff() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to turn underline off.
  _serial_ptr->write(0x48);
}

void NhdDisplay::blinkingOn() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to turn blinking on.
  _serial_ptr->write(0x4B);
}

void NhdDisplay::blinkingOff() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to turn blinking off.
  _serial_ptr->write(0x4C);
}

void NhdDisplay::moveCursorRight() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to move the cursor right by one.
  _serial_ptr->write(0x4A);
}

void NhdDisplay::moveCursorLeft() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to move the cursor left by one.
  _serial_ptr->write(0x49);
}

void NhdDisplay::backspace() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to move cursor left and remove.
  _serial_ptr->write(0x4E);
}

void NhdDisplay::remove() {

  // Together these delete the current character.
  moveCursorRight();
  backspace();
}

