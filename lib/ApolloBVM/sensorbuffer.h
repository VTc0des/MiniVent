#ifndef sensorbuffer_h
#define sensorbuffer_h

#include <Arduino.h>


class SensorBuffer {
public:
    SensorBuffer(uint16_t length);

    uint16_t get(uint16_t idx);
    void put_back(uint16_t value);

private:

    uint16_t* _array;
    uint16_t _length;
    uint16_t _cur_pos;

};

#endif