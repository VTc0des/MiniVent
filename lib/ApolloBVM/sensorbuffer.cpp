#include "sensorbuffer.h"

SensorBuffer::SensorBuffer(uint16_t length)
{
    _array = new uint16_t[length];
    _cur_pos = 0;

    for (uint16_t i = 0; i < length; i++)
        *(_array + i) = 0;
    
}

uint16_t SensorBuffer::get(uint16_t idx) {
    return *(_array + (_cur_pos  + idx) % _length);
}
    
void SensorBuffer::put_back(uint16_t value) {
    
    // Set current position in array to new value.
    Serial.println(_cur_pos  );
    Serial.println(value);
    *(_array + _cur_pos) = value;

    // Advance index of next spot mod length of array (to loop around).
    _cur_pos = (_cur_pos + 1) % _length;
}