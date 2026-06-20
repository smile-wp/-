#include <Servo.h>

Servo myServo;  // 创建舵机对象

void setup() {
  myServo.attach(9);  // 将舵机信号线连接到数字引脚9
  Serial.begin(9600); // 初始化串口通信
  Serial.println("舵机测试开始。");
}

void loop() {
  // 测试1：停止
  Serial.println("停止");
  myServo.write(100);
  delay(2000);

  // 测试2：顺时针旋转
  Serial.println("顺时针旋转");
  myServo.write(0);
  delay(2000);

  // 测试3：逆时针旋转
  Serial.println("逆时针旋转");
  myServo.write(100);
  delay(2000);

  // 测试4：再次停止
  Serial.println("停止");
  myServo.write(0);
  delay(2000);
}