#include <Adafruit_NeoPixel.h>
#define PIN 6                        // RGB灯引脚
#define MAX_LED 30                   // 14个RGB灯
#define HALL_SENSOR_PIN 2            // 霍尔传感器连接的引脚

Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAX_LED, PIN, NEO_RGB + NEO_KHZ800);

// 状态变量
bool flowEnabled = false;            // 控制流水灯效果是否启用
bool lastSensorState = HIGH;         // 记录上一次传感器状态
unsigned long lastTriggerTime = 0;   // 上次触发时间
unsigned long cooldownTime = 300;    // 触发冷却时间（毫秒）
unsigned long previousMillis = 0;    // 用于非阻塞延迟
int flowInterval = 100;              // 流水灯效果间隔(毫秒)
int currentLed = 0;                  // 当前点亮的LED索引
int direction = 1;                   // 流水灯方向(1:正向, -1:反向)
int triggerCount = 0;                // 触发次数计数器

void setup() {
  Serial.begin(9600);               // 初始化串口通信，用于调试
  Serial.println("Starting Hall Sensor Control...");
  
  strip.begin();
  strip.setBrightness(100);         // 设置亮度（0-255）
  strip.show();
  pinMode(HALL_SENSOR_PIN, INPUT_PULLUP); // 设置霍尔传感器引脚为上拉输入
  
  // 初始测试所有LED
  Serial.println("Testing all LEDs...");
  for(int i = 0; i < MAX_LED; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 50)); // 蓝色，低亮度
    strip.show();
    delay(50);
  }
  RGB_OFF();
  Serial.println("Test complete.");
  
  // 初始读取传感器状态
  lastSensorState = digitalRead(HALL_SENSOR_PIN);
  Serial.print("Initial sensor state: ");
  Serial.println(lastSensorState);
}

void RGB_Light(int i, int R, int G, int B) {
  uint32_t color = strip.Color(G, R, B);
  strip.setPixelColor(i, color);
}

void RGB_OFF() {
  for (int i = 0; i < MAX_LED; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

void loop() {
  // 读取传感器状态
  bool currentSensorState = digitalRead(HALL_SENSOR_PIN);
  
  // 检测状态变化
  if (currentSensorState != lastSensorState) {
    Serial.print("Sensor state changed: ");
    Serial.print(lastSensorState);
    Serial.print(" -> ");
    Serial.println(currentSensorState);
    
    // 检测到磁铁靠近（从HIGH到LOW的变化）
    if (lastSensorState == HIGH && currentSensorState == LOW) {
      // 检查是否在冷却时间内
      unsigned long currentTime = millis();
      if (currentTime - lastTriggerTime > cooldownTime) {
        flowEnabled = !flowEnabled; // 切换流水灯状态
        lastTriggerTime = currentTime;
        triggerCount++;
        
        Serial.print("Trigger #");
        Serial.print(triggerCount);
        Serial.print(" - Flow enabled: ");
        Serial.println(flowEnabled);
        
        // 如果关闭流水灯，则熄灭所有LED
        if (!flowEnabled) {
          RGB_OFF();
          Serial.println("LEDs turned OFF");
        } else {
          Serial.println("LED flow started");
        }
      } else {
        Serial.println("Ignored trigger (cooling down)");
      }
    }
    
    lastSensorState = currentSensorState;
  }
  
  // 如果流水灯效果启用，则执行流水灯效果
  if (flowEnabled) {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= flowInterval) {
      previousMillis = currentMillis;
      
      // 熄灭所有LED
      RGB_OFF();
      
      // 点亮当前LED
      RGB_Light(currentLed, 0, 0, 255);
      strip.show();
      
      // 更新当前LED索引
      currentLed += direction;
      
      // 检查是否到达两端，如果是则改变方向
      if (currentLed >= MAX_LED - 1) {
        direction = -1;
      } else if (currentLed <= 0) {
        direction = 1;
      }
    }
  }
  
  // 添加小延迟以避免过于频繁的检测
  delay(5);
}