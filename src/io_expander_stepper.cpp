#include "io_expander_stepper.h"

ExpanderStepper::ExpanderStepper(uint8_t IN1,uint8_t IN2,uint8_t IN3,uint8_t IN4, int steps_per_revolution):
  IN1_(IN1),IN2_(IN2),IN3_(IN3),IN4_(IN4),STEPS_PER_REVOLUTION_(steps_per_revolution),STEP_ANGLE_(2.0 * PI / STEPS_PER_REVOLUTION_)
{
}

ExpanderStepper::ExpanderStepper(uint8_t cs_pin, uint8_t sck_pin,uint8_t miso_pin,uint8_t mosi_pin, uint8_t IN1,uint8_t IN2,uint8_t IN3,uint8_t IN4, int steps_per_revolution):
  CS_PIN_(cs_pin), SCK_PIN_(sck_pin), MISO_PIN_(miso_pin),MOSI_PIN_(mosi_pin),
  IN1_(IN1),IN2_(IN2),IN3_(IN3),IN4_(IN4),STEPS_PER_REVOLUTION_(steps_per_revolution),STEP_ANGLE_(2.0 * PI / STEPS_PER_REVOLUTION_)

{

}
ExpanderStepper::~ExpanderStepper()
{
  mcp_.digitalWrite(IN1_, LOW);
  mcp_.digitalWrite(IN2_, LOW);
  mcp_.digitalWrite(IN3_, LOW);
  mcp_.digitalWrite(IN4_, LOW);
}


void ExpanderStepper::init(uint8_t cs_pin, uint8_t sck_pin,uint8_t miso_pin,uint8_t mosi_pin){
  CS_PIN_ = cs_pin;
  SCK_PIN_ = sck_pin;
  MISO_PIN_ = miso_pin;
  MOSI_PIN_ = mosi_pin;

  if (!mcp_.begin_SPI(CS_PIN_,SCK_PIN_,MISO_PIN_,MOSI_PIN_)) {
    Serial.println("Error.");
    while (1);
  }

  // GPIOピンを出力モードに設定
  mcp_.pinMode(IN1_, OUTPUT);
  mcp_.pinMode(IN2_, OUTPUT);
  mcp_.pinMode(IN3_, OUTPUT);
  mcp_.pinMode(IN4_, OUTPUT);

  // 初期ステップ位置を設定
  setMotorStep(current_step_);
}

// モータのステップを設定する関数
void ExpanderStepper::setMotorStep(int step) {
  mcp_.digitalWrite(IN1_, STEP_PATTERN_[step][0]);
  mcp_.digitalWrite(IN2_, STEP_PATTERN_[step][1]);
  mcp_.digitalWrite(IN3_, STEP_PATTERN_[step][2]);
  mcp_.digitalWrite(IN4_, STEP_PATTERN_[step][3]);
}

// モータを指定速度で動作させる関数（rad/sで入力）
void ExpanderStepper::moveMotorWithSpeed(float speed_rad_per_sec) {
  //speed_rad_per_secの0割りがあるため
  if(abs(speed_rad_per_sec)>=0.001){
    float delayTime = abs(STEP_ANGLE_ / speed_rad_per_sec) * SEC_TO_MICROSEC_; // 各ステップの遅延時間 (マイクロ秒)
    int direction = speed_rad_per_sec > 0 ? 1 : -1; // 方向を決定

    current_step_ = (current_step_ + direction) % 8;
    if (current_step_ < 0) current_step_ += 8;
    setMotorStep(current_step_);
    delayMicroseconds(delayTime);

    current_total_step_ += direction;  //回転方向に1ステップ+
  }
}


// モータを指定速度と位置で目標位置まで動作させる関数
void ExpanderStepper::moveToPosition(float target_postion_rad, float speed_rad_per_sec) {
  float current_position = current_total_step_ * STEP_ANGLE_; // 現在の位置 (ラジアン)
  int steps_to_move = (int)((target_postion_rad - current_position) / STEP_ANGLE_); // 移動するステップ数
  
  int direction = steps_to_move > 0 ? 1 : -1; // 移動方向
  steps_to_move = abs(steps_to_move); // 絶対ステップ数に変換

  current_step_ = (current_step_ + direction) % 8; // 次のステップ位置を計算
  if (current_step_ < 0) current_step_ += 8; // 負の値を正の範囲に調整
  setMotorStep(current_step_); // モータのステップを設定
  delayMicroseconds(abs(STEP_ANGLE_ / speed_rad_per_sec * SEC_TO_MICROSEC_)); // 遅延時間を計算

  current_total_step_ += direction;  //回転方向に1ステップ+
}

float ExpanderStepper::getCurrentAngle(){  //現在の位置（rad)
  float current_position = current_total_step_ * STEP_ANGLE_;
  return current_position;
}