#ifndef RTS_HPP
#define RTS_HPP

#include <stdint.h>

#define MAX_RST_ORDER 4

class Rst {
public:
  // Initialize Rst class.
  Rst(float* reference, float* measurement, float* control,
      float min_control, float max_control);

  // Set rst polynomials coefficient.
  void set_rst(float* r, float* s, float* t, uint8_t order);

  // Initialize rst history.
  void reset();

  // Compute control.
  void compute();

private:
  // System variable
  float* _reference;
  float* _measurement;
  float* _control;

  // Control signal interval
  float _min_control;
  float _max_control;

  // System variable history
  float _reference_hist[MAX_RST_ORDER+1];
  float _measurement_hist[MAX_RST_ORDER+1];
  float _control_hist[MAX_RST_ORDER+1];

  // Order of the rst controller
  uint8_t _order;

  // Coefficient of the rst
  float* _r;
  float* _s;
  float* _t;
};

#endif
