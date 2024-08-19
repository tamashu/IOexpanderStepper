#pragma once 

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

#define CS_PIN 6

class ExpanderStepper
{
public:
  ExpanderStepper(uint8_t IN1,uint8_t IN2,uint8_t IN3,uint8_t IN4, int steps_per_revolution);
  ExpanderStepper(uint8_t cs_pin, uint8_t sck_pin,uint8_t miso_pin,uint8_t mosi_pin, uint8_t IN1,uint8_t IN2,uint8_t IN3,uint8_t IN4, int steps_per_revolution);
  ~ExpanderStepper();

  void init(uint8_t cs_pin, uint8_t sck_pin,uint8_t miso_pin,uint8_t mosi_pin);
  void setMotorStep(int step);
  void moveMotorWithSpeed(float speed_rad_per_sec);
  void moveToPosition(float target_postion_rad, float speed_rad_per_sec); //目標位置(rad/s)
  float getCurrentAngle();  //現在の位置（rad)
private:
  Adafruit_MCP23X17 mcp_; //IOexpanderのクラス
  // GPIOピンの定義（IO expander上のピン番号）
  const uint8_t IN1_;   
  const uint8_t IN2_; 
  const uint8_t IN3_; 
  const uint8_t IN4_; 

  //  //SPIピン
  uint8_t CS_PIN_;
  uint8_t SCK_PIN_;
  uint8_t MISO_PIN_;
  uint8_t MOSI_PIN_;

  //ステッピングモータ関連  
  const int STEPS_PER_REVOLUTION_; // ステッピングモータのステップ数と角度
  const float STEP_ANGLE_; // ステップごとのラジアン角

  // 1-2相励磁式ステッピングモータのステッピングパターン
  const int STEP_PATTERN_[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1}
  };

  const uint32_t SEC_TO_MICROSEC_ = 1000000;  //秒からマイクロ秒の変換

  // 現在のステップ位置
  int current_step_ = 0;

  //現在のトータルのステップ数
  int current_total_step_ = 0;
};

