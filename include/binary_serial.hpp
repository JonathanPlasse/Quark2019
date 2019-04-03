#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <Arduino.h>

void read_data(void* data, size_t nb_bytes);
void write_data(void* data, size_t nb_bytes);

#endif
