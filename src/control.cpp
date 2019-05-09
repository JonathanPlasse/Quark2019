#include "control.hpp"

float speed(const control_t* control, const control_t* last_control, float deltat_t) {
  return (control->measurement - last_control->measurement) / deltat_t;
}
