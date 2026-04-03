/*
 * 项目：呼吸灯
 * 功能：模拟呼吸效果，LED缓慢变亮再变暗
 * 知识点：PWM、for循环、analogWrite()
 */

int ledPin = 9;  // 必须使用PWM引脚
int brightness = 0;  // 当前亮度值
int fadeAmount = 5;  // 每次变化的亮度值

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("呼吸灯效果开始");
}

void loop() {
  // 输出PWM信号控制亮度
  analogWrite(ledPin, brightness);
  
  // 串口显示当前亮度值
  Serial.print("当前亮度: ");
  Serial.println(brightness);
  
  // 改变亮度值
  brightness = brightness + fadeAmount;
  
  // 反转变化方向
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;  // 改变变化方向
  }
  
  // 控制呼吸速度
  delay(30);
}