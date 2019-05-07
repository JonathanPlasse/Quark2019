#include "rst.hpp"
#include "math.h"

Rst::Rst(float* reference, float* measurement, float* control,
         float min_control, float max_control,
         float error_threshold, float pwm_threshold) :
_reference(reference), _measurement(measurement), _control(control),
_min_control(min_control), _max_control(max_control),
_error_threshold(error_threshold), _pwm_threshold(pwm_threshold) {
  reset();
}

void Rst::set_rst(float* r, float* s, float* t, uint8_t order) {
  _r = r;
  _s = s;
  _t = t;
  _order = order;
  reset();
}

void Rst::reset() {
  for (int i = 0 ; i <= MAX_RST_ORDER ; i++) {
    _reference_hist[i] = 0;
    _measurement_hist[i] = 0;
    _control_hist[i] = 0;
  }
}
void Rst::compute() {
  // Update history
  for (int i = 0 ; i < _order ; i++) {
    _reference_hist[_order-i] = _reference_hist[_order-i-1];
    _measurement_hist[_order-i] = _measurement_hist[_order-i-1];
    _control_hist[_order-i] = _control_hist[_order-i-1];
  }
  _reference_hist[0] = *_reference;
  _measurement_hist[0] = *_measurement;

  // Compute partial control signal
  _control_hist[0] = 0;
  for (int i = 0 ; i <= _order ; i++) {
    _control_hist[0] += _t[i]*_reference_hist[i]
                      - _r[i]*_measurement_hist[i];
  }

  // Control signal dead zone before integration to avoid limit cycle
  if ((fabsf(_reference_hist[0] - _measurement_hist[0]) < _error_threshold)
   && (fabsf(_control_hist[0]) < _pwm_threshold)
   && (_reference_hist[0] == _reference_hist[1])) {
    _control_hist[0] = 0;
  }

  // Compute final control signal
  for (int i = 1 ; i <= _order ; i++) {
    _control_hist[0] -= _s[i]*_control_hist[i];
  }

  // Sature control signal
  if (_control_hist[0] > _max_control)
    _control_hist[0] = _max_control;
  else if (_control_hist[0] < _min_control)
    _control_hist[0] = _min_control;

  // Update control signal
  *_control = _control_hist[0];
}
