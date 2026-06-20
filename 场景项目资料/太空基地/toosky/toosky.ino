#include <Servo.h>                // 舵机控制库
#include <Adafruit_NeoPixel.h>    // WS2812灯带控制库

// ====== 1. 触摸控制WS2812灯带配置 ======
const int touchPin = 2;           // 触摸传感器接D2
#define LED_PIN     6             // WS2812灯带数据线接D6
#define NUM_LEDS    30            // ★★★ 重要：请修改为你灯带上的LED数量 ★★★

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// 呼吸灯控制变量
uint8_t breathBrightness = 0;     // 当前亮度值 (0-200)
int8_t fadeAmount = 1;            // 每次亮度变化量 (可正可负)
bool breathingEnabled = false;    // 呼吸灯总开关，由触摸控制

// 非阻塞定时控制
unsigned long breathPreviousMillis = 0;
const long breathInterval = 10;   // 亮度更新间隔(毫秒)

// 触摸检测状态机
int lastTouchState = HIGH;        // 上次触摸状态（默认高电平，因为使用了上拉）
int currentTouchState;            // 当前触摸状态
unsigned long lastDebounceTime = 0; // 上次状态变化时间
const long debounceDelay = 50;    // 消抖延时(毫秒)

// ====== 2. 霍尔控制90度舵机配置 ======
const int hallPin1 = 4;           // 第一个霍尔开关（开门）接D4
Servo doorServo;                  // 创建90度舵机对象
const int servoPin1 = 9;          // 90度舵机信号线接D9
int lastHallState1 = HIGH;        // 霍尔开关1的上次状态
bool doorIsOpen = false;          // 门的状态
const int doorOpenAngle = 90;     // 开门角度
const int doorCloseAngle = 0;     // 关门角度

// ====== 3. 霍尔控制360度舵机与LED配置 ======
const int hallPin2 = 5;           // 第二个霍尔开关（转动）接D5
Servo rotateServo;                // 创建360度舵机对象
const int servoPin2 = 10;         // 360度舵机信号线接D10
const int rotationLedPin = 7;     // 转动状态LED接D7
int lastHallState2 = HIGH;        // 霍尔开关2的上次状态
bool shouldRotate = false;        // 控制360度舵机是否转动的标志
const int stopSpeed = 1500;       // 停止信号
const int rotateSpeed = 1700;     // 顺时针转动速度

void setup() {
  Serial.begin(9600); // 启动串口，用于调试输出

  // 1. 初始化触摸和灯带
  pinMode(touchPin, INPUT_PULLUP); // 启用内部上拉电阻（重要！）
  strip.begin();                    // 初始化NeoPixel对象
  strip.show();                     // 初始化为全灭
  strip.setBrightness(255);         // 设置全局亮度上限

  // 2. 初始化90度舵机模块
  pinMode(hallPin1, INPUT_PULLUP);
  doorServo.attach(servoPin1);
  doorServo.write(doorCloseAngle);
  delay(500); // 给舵机初始化的时间

  // 3. 初始化360度舵机与LED模块
  pinMode(hallPin2, INPUT_PULLUP);
  pinMode(rotationLedPin, OUTPUT);
  digitalWrite(rotationLedPin, LOW);
  rotateServo.attach(servoPin2);
  rotateServo.writeMicroseconds(stopSpeed);
}

void loop() {
  unsigned long currentMillis = millis(); // 获取当前时间

  // ====== 1. 触摸检测与状态切换 (点动开关) ======
  // 读取当前触摸状态
  int reading = digitalRead(touchPin);
  
  // 如果状态发生变化（从高到低或从低到高）
  if (reading != lastTouchState) {
    lastDebounceTime = currentMillis; // 重置消抖计时器
  }
  
  // 检查消抖时间是否已过
  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    // 如果消抖后的状态与当前记录的状态不同
    if (reading != currentTouchState) {
      currentTouchState = reading;
      
      // 检测到下降沿（从高电平变为低电平）：触摸事件发生
      if (currentTouchState == LOW) {
        // 执行触摸操作
        handleTouch();
        Serial.println("Touch detected - state toggled.");
      }
    }
  }
  
  lastTouchState = reading; // 保存当前状态用于下次比较

  // ====== 2. 更新呼吸灯效果 (如果开启) ======
  if (breathingEnabled) {
    updateBreathEffect(currentMillis);
  }

  // ====== 3. 处理霍尔开关控制90度舵机开门 ======
  int hallState1 = digitalRead(hallPin1);
  if (hallState1 == LOW && lastHallState1 == HIGH) {
    delay(50); // 消抖
    if (!doorIsOpen) {
      doorServo.write(doorOpenAngle);
      Serial.println("Door OPEN.");
      doorIsOpen = true;
    } else {
      doorServo.write(doorCloseAngle);
      Serial.println("Door CLOSE.");
      doorIsOpen = false;
    }
    delay(300); // 等待舵机动作完成
  }
  lastHallState1 = hallState1;

  // ====== 4. 处理霍尔开关控制360度舵机与LED ======
  int hallState2 = digitalRead(hallPin2);
  if (hallState2 == LOW && lastHallState2 == HIGH) {
    delay(50); // 消抖
    shouldRotate = !shouldRotate;
    if (shouldRotate) {
      rotateServo.writeMicroseconds(rotateSpeed);
      digitalWrite(rotationLedPin, HIGH);
      Serial.println("360 Servo START.");
    } else {
      rotateServo.writeMicroseconds(stopSpeed);
      digitalWrite(rotationLedPin, LOW);
      Serial.println("360 Servo STOP.");
    }
  }
  lastHallState2 = hallState2;
}

// ========== 灯带控制函数 ==========

// 处理触摸事件
void handleTouch() {
  breathingEnabled = !breathingEnabled; // 切换呼吸灯开关状态

  if (breathingEnabled) {
    Serial.println("Breathing light ON.");
  } else {
    // 关闭时，熄灭所有灯珠并重置呼吸参数
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    breathBrightness = 0;  // 重置亮度
    fadeAmount = 1;        // 重置变化方向
    Serial.println("Breathing light OFF.");
  }
}

// 更新呼吸灯效果 (非阻塞)
void updateBreathEffect(unsigned long currentMillis) {
  if (currentMillis - breathPreviousMillis >= breathInterval) {
    breathPreviousMillis = currentMillis;

    // 1. 将当前亮度应用到所有LED（蓝青色效果）
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, 0, breathBrightness, breathBrightness);
    }
    strip.show();

    // 2. 更新亮度值，准备下一次变化
    breathBrightness += fadeAmount;

    // 3. 在亮度到达边界时反转变化方向
    if (breathBrightness <= 0 || breathBrightness >= 200) {
      fadeAmount = -fadeAmount;
    }
  }
}