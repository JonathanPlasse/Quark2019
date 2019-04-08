#ifndef RTS_HPP
#define RTS_HPP

#include <stdint.h>

#define MAX_RST_ORDER 4

class Rst {
public:
  // Initialize Rst class.
  Rst(float* setpoint, float* measure, float* command);
  // Set rst coefficient.
  void set_rst(float* r, float* s, float* t, uint8_t order);
  // Initialize rst history.
  void reset();
  // Compute command.
  void compute();

private:
  // System variable
  float* setpoint;
  float* measure;
  float* command;
  // System variable history
  float setpoint_hist[MAX_RST_ORDER+1];
  float measure_hist[MAX_RST_ORDER+1];
  float command_hist[MAX_RST_ORDER+1];
  // Order of the rst controller
  uint8_t _order;
  // Coefficient of the rst
  float* _r;
  float* _s;
  float* _t;

};

#endif
