#include "step_response.hpp"
#include "binary_serial.hpp"

bool step_response(Motor* motor, Encoder* encoder, uint32_t* sample_time) {
  // Initialization of the structure for communication
  static config_t config;
  static measure_t measure;

  // Indicate if initialization has been done
  static bool init = false;

  static uint8_t nb_measure_done = 0;
  static uint16_t nb_sample_done = 0;

  // Initialize
  if (!init) {
    // Receive configuration
    read_data(&config, sizeof(config));

    // Set sample time
    // *sample_time = config.sample_time;

    // Initialize counter
    nb_measure_done = 0;
    nb_sample_done = 0;

    // Power motor
    motor->set_pwm(config.pwm);

    // Indicate initialization has been done
    init = true;
  }

  // Handle the step response
  if (nb_measure_done < config.nb_measure) {
    static uint32_t last_position;

    // Get the measurement
    measure.timestamp = millis();
    last_position = measure.position;
    measure.position = encoder->read();
    measure.speed = (float)(measure.position-last_position)
      / config.sample_time * 1000;

    // Send data
    if (nb_sample_done < config.nb_sample) {
      write_data(&measure, sizeof(measure));
      nb_sample_done++;
    }
    // Stop motor and wait
    else if (nb_sample_done < config.nb_sample + config.wait_time / config.sample_time) {
      motor->set_pwm(0);
      nb_sample_done++;
    }
    // Power the motor for another step response
    else if (nb_measure_done + 1 < config.nb_measure) {
      nb_measure_done++;
      nb_sample_done = 0;
      motor->set_pwm(config.pwm);
    }
    // Indicate not all step response has been done
    return false;
  }

  // Reset initialization indicator
  init = false;

  // Indicate all step response has been done
  return true;
}
