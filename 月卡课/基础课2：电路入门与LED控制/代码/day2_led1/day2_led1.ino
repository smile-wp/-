int ledPin = 8;  // 定义LED连接的引脚

void setup() {
  pinMode(ledPin, OUTPUT);  // 设置引脚为输出模式
  Serial.begin(9600);
  Serial.println("LED电路测试开始");
}

void loop() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED亮起");
  delay(500);
  
  digitalWrite(ledPin, LOW);
  Serial.println("LED熄灭");
  delay(500);
}