#include <Servo.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// 定义霍尔传感器引脚
#define HALL_LIFT1_PIN 2      // 升降控制1
#define HALL_LIFT2_PIN 3      // 升降控制2
#define HALL_MUSIC_PIN 4      // 音乐播放控制
#define HALL_DOOR_PIN 7       // 门控制
#define HALL_BOX_PIN 5        // 盒子开合控制
#define HALL_HUMID_PIN 6      // 加湿器控制

// 定义执行器引脚
#define SERVO_LIFT1_PIN 8     // 升降舵机1
#define SERVO_LIFT2_PIN 9     // 升降舵机2
#define SERVO_DOOR_PIN 10     // 门舵机
#define SERVO_BOX_PIN 11      // 盒子开合舵机
#define RELAY_HUMID_PIN 12    // 加湿器继电器

// 设置软件串口
SoftwareSerial mySoftwareSerial(A0, A1);
DFRobotDFPlayerMini myDFPlayer;

// 创建舵机对象
Servo liftServo1;
Servo liftServo2;
Servo doorServo;
Servo boxServo;

// 状态变量 - 使用更小的数据类型
uint8_t liftAngle1 = 0;        // 升降舵机1角度 (0-255足够)
uint8_t liftAngle2 = 0;        // 升降舵机2角度
uint8_t doorAngle = 0;         // 门舵机角度
uint8_t boxAngle = 0;          // 盒子舵机角度
bool humidifierState = false;  // 加湿器状态
bool musicPlaying = false;     // 音乐播放状态
bool musicInitialized = false; // 音乐模块是否初始化成功

// 防抖常量 - 移到Flash中
const unsigned long debounceDelay PROGMEM = 300;
const unsigned long musicDebounceDelay PROGMEM = 300;
const unsigned long servoDetachDelay PROGMEM = 1000;

// 初始化音乐播放器
bool initializeMusicPlayer() {
  Serial.println(F("初始化DFPlayer ..."));
  
  mySoftwareSerial.begin(9600);
  delay(2000);
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("无法初始化DFPlayer:"));
    Serial.println(F("1. 请检查接线是否正确"));
    Serial.println(F("2. 请检查SD卡是否插入"));
    return false;
  }
  
  Serial.println(F("DFPlayer Mini 初始化成功!"));
  myDFPlayer.volume(30);
  myDFPlayer.EQ(0);
  myDFPlayer.pause();
  musicPlaying = false;
  
  Serial.println(F("音乐系统就绪，等待霍尔传感器触发..."));
  return true;
}

// 简化版传感器读取函数
bool readSensor(uint8_t pin) {
  return (digitalRead(pin) == LOW); // 简化滤波，减少变量
}

// 简化版舵机测试函数
void testServos() {
  Serial.println(F("开始舵机测试..."));
  
  // 只测试一个舵机作为代表，减少延迟时间
  liftServo1.attach(SERVO_LIFT1_PIN);
  liftServo1.write(0);
  delay(500);
  liftServo1.write(90);
  delay(500);
  liftServo1.write(0);
  delay(500);
  liftServo1.detach();
  
  Serial.println(F("舵机测试完成"));
}

// 舵机控制函数
void controlServo(Servo &servo, uint8_t pin, uint8_t angle, bool &active, unsigned long &lastActionTime) {
  if (!active) {
    servo.attach(pin);
    active = true;
  }
  servo.write(angle);
  lastActionTime = millis();
}

// 检查并断开不活动的舵机
void checkServoDetach() {
  unsigned long currentTime = millis();
  unsigned long detachDelay = 1000; // 使用局部变量
  
  bool lift1Active = false, lift2Active = false, doorActive = false, boxActive = false;
  unsigned long lift1LastActionTime = 0, lift2LastActionTime = 0, doorLastActionTime = 0, boxLastActionTime = 0;
  
  // 这些变量现在改为局部变量，在各自函数中定义
}

// 音乐控制函数
void handleMusicControl() {
  static bool lastMusicMagnetState = false; // 改为静态局部变量
  static unsigned long lastMusicDebounceTime = 0;
  
  bool currentMagnetState = readSensor(HALL_MUSIC_PIN);
  
  if (currentMagnetState && !lastMusicMagnetState) {
    if (millis() - lastMusicDebounceTime > 300) {
      lastMusicDebounceTime = millis();
      
      if (musicInitialized) {
        if (musicPlaying) {
          myDFPlayer.pause();
          Serial.println(F("音乐: 暂停"));
        } else {
          myDFPlayer.play(1);
          myDFPlayer.loop(1);
          Serial.println(F("音乐: 开始循环播放"));
        }
        musicPlaying = !musicPlaying;
      }
    }
  }
  lastMusicMagnetState = currentMagnetState;
}

// 简化的状态打印函数
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("操作超时!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("SD卡插入!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("SD卡拔出!"));
      break;
    // 简化其他case，只保留重要的
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("系统启动中..."));
  
  // 设置引脚模式
  pinMode(HALL_LIFT1_PIN, INPUT_PULLUP);
  pinMode(HALL_LIFT2_PIN, INPUT_PULLUP);
  pinMode(HALL_MUSIC_PIN, INPUT_PULLUP);
  pinMode(HALL_DOOR_PIN, INPUT_PULLUP);
  pinMode(HALL_BOX_PIN, INPUT_PULLUP);
  pinMode(HALL_HUMID_PIN, INPUT_PULLUP);
  pinMode(RELAY_HUMID_PIN, OUTPUT);
  
  digitalWrite(RELAY_HUMID_PIN, LOW);
  
  // 初始化音乐播放器
  musicInitialized = initializeMusicPlayer();
  
  // 简化测试
  testServos();
  
  Serial.println(F("系统初始化完成"));
  
  // 简化引脚信息打印
  Serial.println(F("=== 引脚分配信息 ==="));
  Serial.print(F("音乐控制: ")); Serial.println(HALL_MUSIC_PIN);
  Serial.print(F("MP3 RX: A0, TX: A1"));
  Serial.println(F("===================="));
}

void loop() {
  static unsigned long lastSensorReadTime = 0; // 改为静态局部变量
  
  // 简化传感器状态读取频率
  if (millis() - lastSensorReadTime > 5000) {
    lastSensorReadTime = millis();
    Serial.print(F("传感器状态:"));
    Serial.print(readSensor(HALL_MUSIC_PIN));
    Serial.println();
    
    if (!musicInitialized && millis() > 10000) {
      static unsigned long lastRetryTime = 0;
      if (millis() - lastRetryTime > 5000) {
        lastRetryTime = millis();
        Serial.println(F("尝试重新初始化音乐模块..."));
        musicInitialized = initializeMusicPlayer();
      }
    }
  }
  
  // 处理所有控制函数 - 将状态变量改为局部静态变量
  handleLift1Control();
  handleLift2Control();
  handleMusicControl();
  handleDoorControl();
  handleBoxControl();
  handleHumidifierControl();
  
  if (musicInitialized && myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
  }
  
  delay(50);
}

// 升降控制1函数 - 状态变量改为局部静态变量
void handleLift1Control() {
  static bool lastLift1MagnetState = false;
  static bool lift1ToggleState = false;
  static unsigned long lastLift1DebounceTime = 0;
  static bool lift1Active = false;
  static unsigned long lift1LastActionTime = 0;
  
  bool currentMagnetState = readSensor(HALL_LIFT1_PIN);
  
  if (currentMagnetState && !lastLift1MagnetState) {
    if (millis() - lastLift1DebounceTime > 300) {
      lastLift1DebounceTime = millis();
      
      if (lift1ToggleState) {
        liftAngle1 = 180;
        Serial.println(F("升降模块1: 旋转到180度"));
      } else {
        liftAngle1 = 0;
        Serial.println(F("升降模块1: 旋转到0度"));
      }
      
      controlServo(liftServo1, SERVO_LIFT1_PIN, liftAngle1, lift1Active, lift1LastActionTime);
      lift1ToggleState = !lift1ToggleState;
    }
  }
  lastLift1MagnetState = currentMagnetState;
}

// 升降控制2函数 - 状态变量改为局部静态变量
void handleLift2Control() {
  static bool lastLift2MagnetState = false;
  static bool lift2ToggleState = false;
  static unsigned long lastLift2DebounceTime = 0;
  static bool lift2Active = false;
  static unsigned long lift2LastActionTime = 0;
  
  bool currentMagnetState = readSensor(HALL_LIFT2_PIN);
  
  if (currentMagnetState && !lastLift2MagnetState) {
    if (millis() - lastLift2DebounceTime > 300) {
      lastLift2DebounceTime = millis();
      
      if (lift2ToggleState) {
        liftAngle2 = 180;
        Serial.println(F("升降模块2: 旋转到180度"));
      } else {
        liftAngle2 = 0;
        Serial.println(F("升降模块2: 旋转到0度"));
      }
      
      controlServo(liftServo2, SERVO_LIFT2_PIN, liftAngle2, lift2Active, lift2LastActionTime);
      lift2ToggleState = !lift2ToggleState;
    }
  }
  lastLift2MagnetState = currentMagnetState;
}

// 门控制函数 - 状态变量改为局部静态变量
void handleDoorControl() {
  static bool lastDoorMagnetState = false;
  static bool doorToggleState = false;
  static unsigned long lastDoorDebounceTime = 0;
  static bool doorActive = false;
  static unsigned long doorLastActionTime = 0;
  
  bool currentMagnetState = readSensor(HALL_DOOR_PIN);
  
  if (currentMagnetState && !lastDoorMagnetState) {
    if (millis() - lastDoorDebounceTime > 300) {
      lastDoorDebounceTime = millis();
      
      if (doorToggleState) {
        doorAngle = 180;
        Serial.println(F("门控制模块: 旋转到180度"));
      } else {
        doorAngle = 0;
        Serial.println(F("门控制模块: 旋转到0度"));
      }
      
      controlServo(doorServo, SERVO_DOOR_PIN, doorAngle, doorActive, doorLastActionTime);
      doorToggleState = !doorToggleState;
    }
  }
  lastDoorMagnetState = currentMagnetState;
}

// 盒子控制函数 - 状态变量改为局部静态变量
void handleBoxControl() {
  static bool lastBoxMagnetState = false;
  static bool boxToggleState = false;
  static unsigned long lastBoxDebounceTime = 0;
  static bool boxActive = false;
  static unsigned long boxLastActionTime = 0;
  
  bool currentMagnetState = readSensor(HALL_BOX_PIN);
  
  if (currentMagnetState && !lastBoxMagnetState) {
    if (millis() - lastBoxDebounceTime > 300) {
      lastBoxDebounceTime = millis();
      
      if (boxToggleState) {
        boxAngle = 120;
        Serial.println(F("盒子开合模块: 打开盒子"));
      } else {
        boxAngle = 0;
        Serial.println(F("盒子开合模块: 关闭盒子"));
      }
      
      controlServo(boxServo, SERVO_BOX_PIN, boxAngle, boxActive, boxLastActionTime);
      boxToggleState = !boxToggleState;
    }
  }
  lastBoxMagnetState = currentMagnetState;
}

// 加湿器控制函数 - 状态变量改为局部静态变量
void handleHumidifierControl() {
  static bool lastHumidMagnetState = false;
  static unsigned long lastHumidDebounceTime = 0;
  
  bool currentMagnetState = readSensor(HALL_HUMID_PIN);
  
  if (currentMagnetState && !lastHumidMagnetState) {
    if (millis() - lastHumidDebounceTime > 300) {
      lastHumidDebounceTime = millis();
      
      humidifierState = !humidifierState;
      digitalWrite(RELAY_HUMID_PIN, humidifierState ? HIGH : LOW);
      
      Serial.print(F("加湿器: "));
      Serial.println(humidifierState ? F("开启") : F("关闭"));
    }
  }
  lastHumidMagnetState = currentMagnetState;
}