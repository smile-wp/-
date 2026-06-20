void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);  // GPIO2连接LED
  Serial.println("LED测试程序启动");
}

void loop() {
  digitalWrite(2, HIGH);
  Serial.println("LED亮");
  delay(1000);
  
  digitalWrite(2, LOW);
  Serial.println("LED灭");
  delay(1000);
}