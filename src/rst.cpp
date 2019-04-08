#include "rst.hpp"

Rst::Rst(float* setpoint, float* measure, float* command) :
_setpoint(setpoint), _measure(measure), _command(command) {
  init_history();
}

void Rst::set_rst(float* r, float* s, float* t, uint8_t order) {
  _r = r;
  _s = s;
  _t = t;
  _order = order;
  init_history();
}

void Rst::init_history() {
  for (int i = 0 ; i <= MAX_RST_ORDER ; i++) {
    setpoint_hist[i] = 0;
    measure_hist[i] = 0;
    command_hist[i] = 0;
  }
}
void Rst::compute() {
  
}
