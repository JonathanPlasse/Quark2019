#ifndef CONTROL_HPP
#define CONTROL_HPP

// Control structure
typedef struct {
  float reference;
  float measurement;
  float command;
} control_t;

float speed(const control_t* control, const control_t* last_control, float deltat_t);

#endif
