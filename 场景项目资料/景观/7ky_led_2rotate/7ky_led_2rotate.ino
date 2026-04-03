#include <Servo.h>

// 定义霍尔传感器引脚
const int HALL_SENSOR_1 = 2;  // 控制灯亮
const int HALL_SENSOR_2 = 3;  // 控制灯灭
const int HALL_SENSOR_3 = 4;  // 控制灯亮
const int HALL_SENSOR_4 = 5;  // 控制灯灭
const int HALL_SENSOR_5 = 6;  // 控制灯亮
const int HALL_SENSOR_6 = 7;  // 控制灯灭
const int HALL_SENSOR_7 = 8;  // 控制灯亮

// 定义LED引脚
const int LED_PIN = 13;

// 定义旋转台引脚
const int HALL_TURNTABLE1_PIN = 9;  // 旋转台1霍尔传感器
const int SERVO_TURNTABLE1_PIN = 11; // 旋转台1舵机控制
const int HALL_TURNTABLE2_PIN = 10;  // 旋转台2霍尔传感器
const int SERVO_TURNTABLE2_PIN = 12; // 旋转台2舵机控制

// 存储传感器状态
int sensorState1 = 0;
int sensorState2 = 0;
int sensorState3 = 0;
int sensorState4 = 0;
int sensorState5 = 0;
int sensorState6 = 0;
int sensorState7 = 0;

// 旋转台控制变量
Servo turntableServo1;
Servo turntableServo2;

bool lastTurntable1MagnetState = false;
bool lastTurntable2MagnetState = false;
bool turntable1IsRotated = false;
bool turntable2IsRotated = false;
int turntableAngle1 = 0;
int turntableAngle2 = 0;
unsigned long lastTurntable1DebounceTime = 0;
unsigned long lastTurntable2DebounceTime = 0;
const unsigned long debounceDelay = 300; // 防抖延迟时间(毫秒)

// 为每个LED传感器添加独立的防抖时间变量
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long lastDebounceTime4 = 0;
unsigned long lastDebounceTime5 = 0;
unsigned long lastDebounceTime6 = 0;
unsigned long lastDebounceTime7 = 0;

// 传感器2和3同时触发检测变量
bool sensor2Active = false;
bool sensor3Active = false;
unsigned long sensor2TriggerTime = 0;
unsigned long sensor3TriggerTime = 0;
const unsigned long simultaneousWindow = 50; // 同时触发判定窗口(毫秒)

void setup() {
  // 初始化串口通信
  Serial.begin(9600);
  
  // 设置传感器引脚为输入模式，启用上拉电阻
  pinMode(HALL_SENSOR_1, INPUT_PULLUP);
  pinMode(HALL_SENSOR_2, INPUT_PULLUP);
  pinMode(HALL_SENSOR_3, INPUT_PULLUP);
  pinMode(HALL_SENSOR_4, INPUT_PULLUP);
  pinMode(HALL_SENSOR_5, INPUT_PULLUP);
  pinMode(HALL_SENSOR_6, INPUT_PULLUP);
  pinMode(HALL_SENSOR_7, INPUT_PULLUP);
  
  // 设置旋转台霍尔传感器引脚为输入模式，启用上拉电阻
  pinMode(HALL_TURNTABLE1_PIN, INPUT_PULLUP);
  pinMode(HALL_TURNTABLE2_PIN, INPUT_PULLUP);
  
  // 设置LED引脚为输出模式
  pinMode(LED_PIN, OUTPUT);
  
  // 初始关闭LED
  digitalWrite(LED_PIN, LOW);
  
  // 初始化旋转台舵机
  turntableServo1.attach(SERVO_TURNTABLE1_PIN);
  turntableServo2.attach(SERVO_TURNTABLE2_PIN);
  
  // 初始化旋转台位置
  turntableServo1.write(turntableAngle1);
  turntableServo2.write(turntableAngle2);
  
  Serial.println("系统初始化完成，等待霍尔传感器触发...");
  Serial.println("传感器1、3、5、7控制灯亮");
  Serial.println("传感器2、4、6控制灯灭");
  Serial.println("旋转台1和2已就绪 - 初始位置: 0度");
  Serial.println("传感器2和3已启用同时触发检测");
}

void loop() {
  // 读取所有传感器的状态
  sensorState1 = digitalRead(HALL_SENSOR_1);
  sensorState2 = digitalRead(HALL_SENSOR_2);
  sensorState3 = digitalRead(HALL_SENSOR_3);
  sensorState4 = digitalRead(HALL_SENSOR_4);
  sensorState5 = digitalRead(HALL_SENSOR_5);
  sensorState6 = digitalRead(HALL_SENSOR_6);
  sensorState7 = digitalRead(HALL_SENSOR_7);
  
  // 处理LED控制
  handleLEDControl();
  
  // 处理旋转台控制
  handleTurntable1Control();
  handleTurntable2Control();
  
  // 短暂延迟以减少循环频率
  delay(10);
}

// LED控制函数（改进版 - 包含同时触发检测）
void handleLEDControl() {
  // 检查传感器2（控制灯灭）
  if (sensorState2 == LOW && !sensor2Active && (millis() - lastDebounceTime2) > debounceDelay) {
    sensor2Active = true;
    sensor2TriggerTime = millis();
    
    // 检查传感器3是否在最近也触发了
    if (sensor3Active && (millis() - sensor3TriggerTime) < simultaneousWindow) {
      Serial.println("传感器2和3同时触发 - 忽略传感器2操作");
    } else {
      digitalWrite(LED_PIN, LOW);
      Serial.println("传感器2触发 - LED灭");
    }
    lastDebounceTime2 = millis();
  }
  
  // 传感器2释放时重置状态
  if (sensorState2 == HIGH && sensor2Active) {
    sensor2Active = false;
  }
  
  // 检查传感器3（控制灯亮）
  if (sensorState3 == LOW && !sensor3Active && (millis() - lastDebounceTime3) > debounceDelay) {
    sensor3Active = true;
    sensor3TriggerTime = millis();
    
    // 检查传感器2是否在最近也触发了
    if (sensor2Active && (millis() - sensor2TriggerTime) < simultaneousWindow) {
      Serial.println("传感器3和2同时触发 - 忽略传感器3操作");
    } else {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("传感器3触发 - LED亮");
    }
    lastDebounceTime3 = millis();
  }
  
  // 传感器3释放时重置状态
  if (sensorState3 == HIGH && sensor3Active) {
    sensor3Active = false;
  }
  
  // 其他传感器处理保持不变
  if (sensorState1 == LOW && (millis() - lastDebounceTime1) > debounceDelay) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("传感器1触发 - LED亮");
    lastDebounceTime1 = millis();
  }
  
  if (sensorState4 == LOW && (millis() - lastDebounceTime4) > debounceDelay) {
    digitalWrite(LED_PIN, LOW);
    Serial.println("传感器4触发 - LED灭");
    lastDebounceTime4 = millis();
  }
  
  if (sensorState5 == LOW && (millis() - lastDebounceTime5) > debounceDelay) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("传感器5触发 - LED亮");
    lastDebounceTime5 = millis();
  }
  
  if (sensorState6 == LOW && (millis() - lastDebounceTime6) > debounceDelay) {
    digitalWrite(LED_PIN, LOW);
    Serial.println("传感器6触发 - LED灭");
    lastDebounceTime6 = millis();
  }
  
  if (sensorState7 == LOW && (millis() - lastDebounceTime7) > debounceDelay) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("传感器7触发 - LED亮");
    lastDebounceTime7 = millis();
  }
}

// 旋转台1控制函数
void handleTurntable1Control() {
  bool currentMagnetState = (digitalRead(HALL_TURNTABLE1_PIN) == LOW);
  
  // 检测磁铁状态变化 (从无磁场到有磁场)
  if (currentMagnetState && !lastTurntable1MagnetState) {
    // 防抖检查
    if (millis() - lastTurntable1DebounceTime > debounceDelay) {
      lastTurntable1DebounceTime = millis();
      
      // 切换旋转状态
      if (turntable1IsRotated) {
        turntableAngle1 = 0; // 旋转回0度
        Serial.println("旋转台1: 磁铁靠近 - 旋转回0度");
      } else {
        turntableAngle1 = 180; // 旋转到180度
        Serial.println("旋转台1: 磁铁靠近 - 旋转到180度");
      }
      
      // 更新舵机位置
      turntableServo1.write(turntableAngle1);
      
      // 切换状态标志
      turntable1IsRotated = !turntable1IsRotated;
    }
  }
  
  // 更新上一次磁铁状态
  lastTurntable1MagnetState = currentMagnetState;
}

// 旋转台2控制函数
void handleTurntable2Control() {
  bool currentMagnetState = (digitalRead(HALL_TURNTABLE2_PIN) == LOW);
  
  // 检测磁铁状态变化 (从无磁场到有磁场)
  if (currentMagnetState && !lastTurntable2MagnetState) {
    // 防抖检查
    if (millis() - lastTurntable2DebounceTime > debounceDelay) {
      lastTurntable2DebounceTime = millis();
      
      // 切换旋转状态
      if (turntable2IsRotated) {
        turntableAngle2 = 0; // 旋转回0度
        Serial.println("旋转台2: 磁铁靠近 - 旋转回0度");
      } else {
        turntableAngle2 = 180; // 旋转到180度
        Serial.println("旋转台2: 磁铁靠近 - 旋转到180度");
      }
      
      // 更新舵机位置
      turntableServo2.write(turntableAngle2);
      
      // 切换状态标志
      turntable2IsRotated = !turntable2IsRotated;
    }
  }
  
  // 更新上一次磁铁状态
  lastTurntable2MagnetState = currentMagnetState;
}