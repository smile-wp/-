// Arduino LED 闪烁程序
// 引脚定义：LED连接到数字引脚13
/*点亮一个led灯
const int ledPin = 13;  // 使用常量定义引脚号，便于修改

void setup() {
  // 初始化LED引脚为输出模式
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);  // 点亮LED（高电平）
  delay(500);                  // 延时500毫秒（0.5秒）
  digitalWrite(ledPin, LOW);   // 熄灭LED（低电平）
  delay(500);                  // 延时500毫秒
}
*/
//有人靠近，实现灯亮
const int PIR_PIN = 8; // 人体感应模块OUT接D2
const int LED_PIN = 10; // LED接D3（需串联220Ω电阻）

void setup() {
pinMode(PIR_PIN, INPUT);
pinMode(LED_PIN, OUTPUT);
digitalWrite(LED_PIN, LOW); // 初始关闭LED

Serial.begin(9600);
Serial.println("System Ready");
}

void loop() {
int motionState = digitalRead(PIR_PIN); // 读取传感器状态

if (motionState == HIGH) {
digitalWrite(LED_PIN, HIGH); // 检测到人靠近，点亮LED
Serial.println("Motion detected! LED ON");
} else {
digitalWrite(LED_PIN, LOW); // 无人时关闭LED
Serial.println("No motion");
}

delay(100); // 短暂延迟稳定读取
}