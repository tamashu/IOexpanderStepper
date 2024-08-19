#include "io_expander_stepper.h"

ExpanderStepper::ExpanderStepper(uint8_t* motor_1_INs,uint8_t* motor_2_INs,uint8_t* motor_3_INs,uint8_t* motor_4_INs, int steps_per_revolution ):
  STEPS_PER_REVOLUTION_(steps_per_revolution),STEP_ANGLE_(2.0 * PI / STEPS_PER_REVOLUTION_){
  for(int i=0;i<4;i++){
    motor_pins[0][i] = motor_1_INs[i];
    motor_pins[1][i] = motor_2_INs[i];
    motor_pins[2][i] = motor_3_INs[i];
    motor_pins[3][i] = motor_4_INs[i];

    current_step_[i] =0;
    current_total_step_[i] = 0;
  }
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

  // 初期ステップ位置を設定
  for(int i=0;i<NUM_OF_MOTOR;i++){
    // GPIOピンを出力モードに設定
    mcp_.pinMode(motor_pins[i][0], OUTPUT);
    mcp_.pinMode(motor_pins[i][1], OUTPUT);
    mcp_.pinMode(motor_pins[i][2], OUTPUT);
    mcp_.pinMode(motor_pins[i][3], OUTPUT);

    setMotorStep(current_step_[i],i);
  }
}

// モータを指定速度と位置で目標位置まで動作させる関数
void ExpanderStepper::moveToPosition(float target_postion_rad, float speed_rad_per_sec, int motor_id) {
  float current_position = current_total_step_[motor_id] * STEP_ANGLE_; // 現在の位置 (ラジアン)
  int steps_to_move = (int)((target_postion_rad - current_position) / STEP_ANGLE_); // 移動するステップ数
  
  int direction = steps_to_move > 0 ? 1 : -1; // 移動方向
  steps_to_move = abs(steps_to_move); // 絶対ステップ数に変換

  current_step_[motor_id] = (current_step_[motor_id] + direction) % 8; // 次のステップ位置を計算
  if (current_step_[motor_id] < 0) current_step_[motor_id] += 8; // 負の値を正の範囲に調整
  setMotorStep(current_step_[motor_id],motor_id); // モータのステップを設定
  delayMicroseconds(abs(STEP_ANGLE_ / speed_rad_per_sec * SEC_TO_MICROSEC_)); // 遅延時間を計算

  current_total_step_[motor_id] += direction;  //回転方向に1ステップ+
}

float ExpanderStepper::getCurrentAngle(int motor_id){  //現在の位置（rad)
  float current_position = current_total_step_[motor_id] * STEP_ANGLE_;
  return current_position;
}


//複数台用
void ExpanderStepper::setMotorStep(int step, int motor_id) {
  mcp_.digitalWrite(motor_pins[motor_id][0], STEP_PATTERN_[step][0]);
  mcp_.digitalWrite(motor_pins[motor_id][1], STEP_PATTERN_[step][1]);
  mcp_.digitalWrite(motor_pins[motor_id][2], STEP_PATTERN_[step][2]);
  mcp_.digitalWrite(motor_pins[motor_id][3], STEP_PATTERN_[step][3]);
}

//複数台用
void ExpanderStepper::moveMotorWithSpeed(float speed_rad_per_sec,int motor_id) {
  //speed_rad_per_secの0割りがあるため
  if(abs(speed_rad_per_sec)>=0.001){
    float delayTime = abs(STEP_ANGLE_ / speed_rad_per_sec) * SEC_TO_MICROSEC_; // 各ステップの遅延時間 (マイクロ秒)
    int direction = speed_rad_per_sec > 0 ? 1 : -1; // 方向を決定

    current_step_[motor_id] = (current_step_[motor_id] + direction) % 8;
    if (current_step_[motor_id] < 0) current_step_[motor_id] += 8;
    setMotorStep(current_step_[motor_id],motor_id);
    delayMicroseconds(delayTime);

    current_total_step_[motor_id] += direction;  //回転方向に1ステップ+
  }
}