#include <Servo.h>

Servo doorServo;
int currentAngle = 0;  // 当前角度
int targetAngle = 0;   // 目标角度
int speedDelay = 10;   // 每次转动后的延迟（毫秒），值越大速度越慢

void setup() {
  doorServo.attach(5);
  doorServo.write(0);   // 初始化为关闭状态
  delay(1000);
}

// 平滑移动函数
void smoothMove(int target) {
  targetAngle = target;
  
  while (currentAngle != targetAngle) {
    if (currentAngle < targetAngle) {
      currentAngle++;
    } else {
      currentAngle--;
    }
    doorServo.write(currentAngle);
    delay(speedDelay);  // 控制速度的关键延迟
  }
}

void loop() {
  // 缓慢开门到80度
  smoothMove(80);
  delay(1000);          // 保持开门状态1秒

  // 缓慢关门到0度
  smoothMove(0);
  delay(1000);          // 保持关门状态1秒

  // 等待9秒后再循环
  delay(500);
}