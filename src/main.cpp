#include <Arduino.h>
#include "io_expander_stepper.h"

//SPI通信のピン
const uint8_t sck_pin = 4;
const uint8_t mosi_pin= 5;
const uint8_t miso_pin =6;
const uint8_t cs_pin = 7;

//ステッピングモータ用のIOピン(Exoander上のピン番号)
const uint8_t IN1 = 4;
const uint8_t IN2 = 5;
const uint8_t IN3 = 6;
const uint8_t IN4 = 7;

//ステッピングモータ関連
const float GEAR_RATIO = 10.0; //ギア比
// ステッピングモータのステップ数と角度
const int STEPS_PER_REVOLUTION = 4096; // 28BYJ-48のステップ数

ExpanderStepper expander_stepper(IN1,IN2,IN3,IN4,STEPS_PER_REVOLUTION);

void setup() {
  Serial.println("MCP23xxx Test!");
  expander_stepper.init(cs_pin,sck_pin,miso_pin,mosi_pin);
}

void loop() {
  expander_stepper.moveMotorWithSpeed(1.57);
}