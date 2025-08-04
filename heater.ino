/************* serial command list ****************/
/*
on     : to turn on the heater
off    : to turn off the heater

numerical value, example 24, 45, 67 etc  : temperature setpoint 
*/
#include "heater.h"

#define t_pin A0
#define h_pin A1

#define red   A2
#define green A3
#define blue  A4

uint32_t runtime = 500;  // program runtime in milliseconds


heater heater(t_pin, h_pin);

void setup() {
  
  Serial.begin(115200);
  Serial.setTimeout(10);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  Serial.print("Starting");
  for(int i = 0; i < 15; i++){
    Serial.print(".");
    delay(150);
  }
  Serial.println("");
  Serial.println("Send temperature setpoint via serial");
  delay(3000);
  
  heater.turn_on();
}

void loop() {
  uint32_t starttime = millis();

  serial_set_temperature(); // send numerical setpoint via serial, example: 45
  heater.update_state();        
  heater.print_data();

  update_led(heater.get_state());

  while(millis() <= (starttime + runtime));
  //Serial.println(millis() - starttime);
}



void update_led(uint8_t n){
  switch(n){
    // idle
    case 0: digitalWrite(red, LOW);
            digitalWrite(green, LOW);
            digitalWrite(blue, HIGH);
            break;
    //heating
    case 1: digitalWrite(red, HIGH);
            digitalWrite(green, LOW);
            digitalWrite(blue, LOW);
            break;
    //temperature reached       
    case 2: digitalWrite(red, LOW);
            digitalWrite(green, HIGH);
            digitalWrite(blue, LOW);
            break;
    //overheat
    case 3: digitalWrite(red, HIGH);
            digitalWrite(green, HIGH);
            digitalWrite(blue, HIGH);
            break;
  }
}

void serial_set_temperature(){
  if(Serial.available()){
    String dummy = Serial.readStringUntil('\n');
    if(dummy == "off"){
      heater.turn_off();
    }
    if(dummy == "on"){
      heater.turn_on();
    }

    float temp = dummy.toFloat();
    if(temp != 0){
      heater.set_temperature(temp);
    }
    
  }
}
