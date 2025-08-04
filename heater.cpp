#include "Arduino.h"
#include "heater.h"

heater::heater(int T_pin, int H_pin, float mgn = 1){
    temp_pin = T_pin;
    heat_pin = H_pin;
    margin = mgn;
    on = false;
    stn = 0;
    pinMode(temp_pin, INPUT);
    pinMode(heat_pin, OUTPUT);
    digitalWrite(heat_pin, LOW);
  }

float heater::get_temperature(){
  const float BETA = 3950; // should match the Beta Coefficient of the thermistor
  float analogValue = analogRead(temp_pin);
  temperature = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  return temperature;
}

uint8_t heater::get_state(){
  return stn;
}

void heater::print_temperature(){
  Serial.println(get_temperature());
}

void heater::set_temperature(float temp){
  set_temp = temp;
}

void heater::heat_on(){
  digitalWrite(heat_pin, HIGH);
}

void heater::heat_off(){
  digitalWrite(heat_pin, LOW);
}

void heater::turn_on(){
  on = true;
  update_state();
}

void heater::turn_off(){
  on = false;
  update_state();
}

void heater::update_state(){
  if(on){
    float temp = get_temperature();
    if(temp < (set_temp - margin)){
      state = "Heating";
      heat_on();
      stn = 1;
    }
    if((temp >= (set_temp - margin)) && (temp <= (set_temp + margin))){
      state = "Target Reached";
      heat_off();
      stn = 2;
    }
    if(temp > (set_temp + margin)){
      state = "OverHeated";
      heat_off();
      stn = 3;
    }
  }
  else{
    state = "Idle";
    heat_off();
    stn = 0;
  }
}

void heater::print_data(){
  Serial.print("Set Temperature: ");
  Serial.print(set_temp);
  Serial.print('\t');
  Serial.print("Current Temperature: ");
  Serial.print(temperature);
  Serial.print('\t');
  Serial.print("State: ");
  Serial.println(state);
}