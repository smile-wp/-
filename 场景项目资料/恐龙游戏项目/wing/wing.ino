#include <Servo.h>

Servo myServo;

void setup() {
  Serial.begin(9600);
  Serial.println("360度舵机测试开始");z
  
  myServo.attach(9);  // 连接到引脚9
  delay(1000);        // 等待舵机初始化
  
  // 先停止舵机
  myServo.write(90);
  Serial.println("舵机停止");
  delay(2000);
}

void loop() {
  // 对于360度连续旋转舵机：
  // 90 = 停止
  // 0-89 = 顺时针旋转 (0最快，89最慢)
  // 91-180 = 逆时针旋转 (91最慢，180最快)
  
  // 测试顺时针旋转 - 中等速度
  Serial.println("顺时针旋转 - 中等速度");
  myServo.write(20);  // 值越小，顺时针速度越快
  delay(5000);
  
 
  
}