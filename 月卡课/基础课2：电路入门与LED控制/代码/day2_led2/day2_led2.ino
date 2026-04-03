/*
 * 项目：交通信号灯模拟
 * 红灯亮3秒，黄灯亮1秒，绿灯亮3秒
 */

// 定义引脚
int redPin = 8;
int yellowPin = 9;
int greenPin = 10;

void setup() {
  // 初始化所有引脚
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("交通信号灯模拟开始");
}

void loop() {
  // 红灯亮
  digitalWrite(redPin, HIGH);
  Serial.println("红灯亮 - 停车");
  delay(3000);
  
  // 红灯灭，黄灯亮
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, HIGH);
  Serial.println("黄灯亮 - 准备");
  delay(1000);
  
  // 黄灯灭，绿灯亮
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, HIGH);
  Serial.println("绿灯亮 - 通行");
  delay(3000);
  
  // 绿灯灭，黄灯亮
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, HIGH);
  Serial.println("黄灯亮 - 减速");
  delay(1000);
  
  digitalWrite(yellowPin, LOW);
}