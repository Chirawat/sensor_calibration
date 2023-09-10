#include "RoboFi.h"
#include <EEPROM.h>

#define EEPROM_SIZE 2

bool _calibration_mode = false;
int val_l, val_h, val_avg;
int ref_data;

void setup() {
  Serial.begin(9600);
  pinMode(36, INPUT);
  EEPROM.begin(EEPROM_SIZE);

  if(sw_OK_press()){
    beep();
    _calibration_mode = true;
    Serial.println("Calibration mode activated");
    while(_calibration_mode){
      Serial.print("Reading white...");
      sw_OK();
      val_l = analogRead(36);
      Serial.println(val_l);
      Serial.print("Reading black...");
      sw_OK();
      val_h = analogRead(36);
      Serial.println(val_h);

      Serial.print("Average: ");
      val_avg = (val_h + val_l) / 2;
      Serial.println(val_avg);

      // Write data to EEPROM (2 bytes of data)
      EEPROM.write(0, val_avg);
      EEPROM.write(1, val_avg>>8);
      EEPROM.commit();

      Serial.println("Press OK to continue...");
      sw_OK();
      
      _calibration_mode = false;
    }
  }
  Serial.println("Normal Mode");

  // Restore 2 bytes of data using bitwise operation
  ref_data = EEPROM.read(0);
  ref_data = ref_data | EEPROM.read(1)<<8;
}

void loop() {
  int sensor = analogRead(36);
  Serial.print("Sensor: ");
  Serial.print(sensor);
  Serial.print(", Ref_data: ");
  Serial.print(ref_data);
  Serial.print(", Result: ");
  Serial.println(sensor > ref_data ? "Black" : "White");
  delay(100);
}
