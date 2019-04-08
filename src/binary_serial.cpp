#include "binary_serial.hpp"

void read_data(void* data, size_t nb_bytes) {
  while (Serial.available() < nb_bytes);
  Serial.readBytes((byte*) data, nb_bytes);
}

void write_data(void* data, size_t nb_bytes) {
  byte* byte_data = (byte*) data;
  Serial.write(byte_data, nb_bytes);
}
