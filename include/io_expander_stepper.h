#pragma once 

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

#define NUM_OF_MOTOR 4

class ExpanderStepper
{
public:
  ExpanderStepper(uint8_t* motor_1_INs,uint8_t* motor_2_INs,uint8_t* motor_3_INs,uint8_t* motor_4_INs, int steps_per_revolution );
  ~ExpanderStepper();

  //複数台用
  void init(uint8_t cs_pin, uint8_t sck_pin,uint8_t miso_pin,uint8_t mosi_pin);
  void moveToPosition(float target_postion_rad, float speed_rad_per_sec, int motor_id); //目標位置(rad/s)
  float getCurrentAngle(int motor_id);  //現在の位置（rad)
  void setMotorStep(int step,int motor_id);
  void moveMotorWithSpeed(float speed_rad_per_sec,int motor_id);  ////目標速度で回転(raa/s)
private:
  Adafruit_MCP23X17 mcp_; //IOexpanderのクラス
  // GPIOピンの定義（IO expander上のピン番号）
  uint8_t IN1_;   
  uint8_t IN2_; 
  uint8_t IN3_; 
  uint8_t IN4_; 

  //モータ複数第制御用
  uint8_t motor_pins[NUM_OF_MOTOR][4];  //各モータにつき制御pinは4つ

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
  int current_step_[NUM_OF_MOTOR];

  //現在のトータルのステップ数
  int current_total_step_[NUM_OF_MOTOR];
};

