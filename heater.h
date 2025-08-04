#ifndef heater_h
#define heater_h

#include <Arduino.h>

class heater{
  private:
  int temp_pin;
  int heat_pin;
  float temperature;
  float set_temp = 24;
  float margin;        // temperature margin for setpoint
  bool on;
  String state = "Idle";
  uint8_t stn;

  public:
  heater(int T_pin, int H_pin, float mgn = 1);
  float get_temperature();
  uint8_t get_state();
  void print_temperature();
  void set_temperature(float temp);
  void heat_on();
  void heat_off();
  void turn_on();
  void turn_off();
  void update_state();
  void print_data();
};

#endif
