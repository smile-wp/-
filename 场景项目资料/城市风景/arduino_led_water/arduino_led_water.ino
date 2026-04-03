#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

const int TOUCH_PIN = 2;
const int LED_PIN = 13;
const int HUMIDIFIER_PIN = 7;

bool systemState = false;
int lastTouchState = LOW;

void setup() {
  Serial.begin(9600);
  
  // 先初始化基础设备
  pinMode(TOUCH_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(HUMIDIFIER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(HUMIDIFIER_PIN, LOW);
  
  Serial.println("基础设备初始化完成");
  
  // 尝试初始化音乐模块（非阻塞）
  delay(2000);
  mySoftwareSerial.begin(9600);
  
  Serial.println("尝试连接DFPlayer...");
  delay(2000);
  
  bool dfPlayerReady = false;
  
  // 尝试多次初始化
  for(int i = 0; i < 3; i++) {
    if (myDFPlayer.begin(mySoftwareSerial)) {
      dfPlayerReady = true;
      Serial.println("DFPlayer连接成功!");
      myDFPlayer.volume(30);
      myDFPlayer.EQ(0);
      delay(100);
      myDFPlayer.stop();
      break;
    } else {
      Serial.println("DFPlayer尝试连接失败，5秒后重试...");
      delay(5000);
    }
  }
  
  if (!dfPlayerReady) {
    Serial.println("警告: DFPlayer未就绪，系统将在无音乐模式下运行");
  }
  
  Serial.println("系统就绪 - 触摸控制LED和加湿器");
}

void loop() {
  int touchState = digitalRead(TOUCH_PIN);
  
  if (touchState == HIGH && lastTouchState == LOW) {
    systemState = !systemState;
    
    if (systemState) {
      // 开启所有设备
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(HUMIDIFIER_PIN, LOW);
      
      // 尝试播放音乐（如果DFPlayer就绪）
      myDFPlayer.loop(1);
      Serial.println("系统启动 - 所有设备工作");
    } else {
      // 关闭所有设备
      digitalWrite(LED_PIN, LOW);
      digitalWrite(HUMIDIFIER_PIN, HIGH);
      myDFPlayer.stop();
      Serial.println("系统停止 - 所有设备关闭");
    }
    
    delay(500);
  }
  
  lastTouchState = touchState;
  delay(100);
}