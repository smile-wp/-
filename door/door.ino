#include <Servo.h> // 调用舵机库:cite[10]

Servo doorServo;   // 创建一个舵机对象，用于控制门:cite[2]
void setup() {
  doorServo.attach(5);  // 将舵机信号线连接到数字引脚5:cite[1]
  doorServo.write(0);   // 初始化门为关闭状态 (0度):cite[2]
  delay(1000);          // 等待系统稳定
}

void loop() {
  // 开门：舵机转动到90度位置:cite[1]
  doorServo.write(80);
  delay(1000);          // 等待舵机动作完成:cite[2]

  // 关门：舵机转回0度位置
  doorServo.write(0);
  delay(1000);          // 等待舵机动作完成

  // 可根据需要调整延时，以控制门开合的间隔时间
  delay(500);
}