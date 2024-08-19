#include <Arduino.h>
#include "io_expander_stepper.h"

//ステッピングモータ関連
const float GEAR_RATIO = 10.0; //ギア比
// ステッピングモータのステップ数と角度
const int STEPS_PER_REVOLUTION = 4096; // 28BYJ-48のステップ数

//SPI通信のピン
const uint8_t sck_pin = 4;
const uint8_t mosi_pin= 5;
const uint8_t miso_pin =6;
const uint8_t cs_pin = 7;

//ステッピングモータ用のIOピン(Exoander上のピン番号)
uint8_t MOTOR_1_PINs[4]={0,1,2,3};
uint8_t MOTOR_2_PINs[4]={4,5,6,7};
uint8_t MOTOR_3_PINs[4]={8,9,10,11};
uint8_t MOTOR_4_PINs[4]={12,13,14,15};

ExpanderStepper expander_stepper(MOTOR_1_PINs,MOTOR_2_PINs,MOTOR_3_PINs,MOTOR_4_PINs,STEPS_PER_REVOLUTION);


// const uint8_t IN1 = 4;
// const uint8_t IN2 = 5;
// const uint8_t IN3 = 6;
// const uint8_t IN4 = 7;

// ExpanderStepper expander_stepper(IN1,IN2,IN3,IN4,STEPS_PER_REVOLUTION);




void setup() {
  Serial.println("MCP23xxx Test!");
  expander_stepper.init(cs_pin,sck_pin,miso_pin,mosi_pin);
}

void loop() {
  expander_stepper.moveMotorWithSpeed(1.57,0);
  expander_stepper.moveMotorWithSpeed(-1.57,1);
  expander_stepper.moveMotorWithSpeed(0.8,2);
  expander_stepper.moveMotorWithSpeed(-0.8,3);
}