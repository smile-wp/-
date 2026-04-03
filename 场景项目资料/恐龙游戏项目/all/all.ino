#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// 设置软件串口
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// 创建舵机对象
Servo wingServo;
Servo teethServo;
Servo doorServo;
Servo liftServo1;
Servo liftServo2;

// 引脚定义
// 翼龙翅膀模块
const int HALL_WING = 2;    // 霍尔传感器引脚
const int SERVO_WING = 3;   // 翅膀舵机引脚

// 牙齿咬合模块
const int HALL_TEETH = 4;   // 霍尔传感器引脚
const int SERVO_TEETH = 5;  // 牙齿舵机引脚

// 门开合模块
const int HALL_DOOR = 6;    // 霍尔传感器引脚
const int SERVO_DOOR = 7;   // 门舵机引脚

// LED灯亮模块
const int HALL_LED = 8;     // 霍尔传感器引脚
const int LED_PIN = 9;      // LED引脚

// 升降模块1
const int HALL_LIFT1 = A0;  // 霍尔传感器引脚
const int SERVO_LIFT1 = A1; // 升降舵机1引脚

// 升降模块2
const int HALL_LIFT2 = A2;  // 霍尔传感器引脚
const int SERVO_LIFT2 = A3; // 升降舵机2引脚

// 360度舵机速度定义
const int WING_STOP = 90;      // 停止
const int WING_CW_FAST = 20;   // 顺时针快速

// 180度舵机角度定义
const int TEETH_OPEN = 0;      // 牙齿张开角度
const int TEETH_CLOSE = 50;    // 牙齿闭合角度

const int DOOR_OPEN = 60;       // 门打开角度
const int DOOR_CLOSE =20;     // 门关闭角度

const int LIFT_UP =0;       // 升降模块升起角度
const int LIFT_DOWN = 80;       // 升降模块下降角度

// 音效定义
const int SOUND_WING = 1;      // 翅膀音效 - 001.mp3
const int SOUND_TEETH = 2;     // 牙齿音效 - 002.mp3
const int SOUND_DOOR = 3;      // 门音效 - 003.mp3
const int SOUND_LED = 4;       // LED音效 - 004.mp3
const int SOUND_LIFT = 5;      // 升降音效 - 005.mp3

// 状态变量
bool wingActive = false;
bool teethActive = false;
bool doorActive = false;
bool ledActive = false;
bool lift1Active = false;
bool lift2Active = false;

// 防抖变量
bool lastHallWing = HIGH;
bool lastHallTeeth = HIGH;
bool lastHallDoor = HIGH;
bool lastHallLed = HIGH;
bool lastHallLift1 = HIGH;
bool lastHallLift2 = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 防抖延迟

void setup() {
  // 初始化串口
  Serial.begin(9600);
  
  // 先设置所有引脚模式，但不连接舵机
  pinMode(HALL_WING, INPUT_PULLUP);
  pinMode(HALL_TEETH, INPUT_PULLUP);
  pinMode(HALL_DOOR, INPUT_PULLUP);
  pinMode(HALL_LED, INPUT_PULLUP);
  pinMode(HALL_LIFT1, INPUT_PULLUP);
  pinMode(HALL_LIFT2, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  
  // 初始化LED状态
  digitalWrite(LED_PIN, LOW);      // LED熄灭
  
  Serial.println("开始初始化系统...");
  
  // 先初始化DFPlayer，避免舵机干扰
  Serial.println("初始化DFPlayer ...");
  mySoftwareSerial.begin(9600);
  delay(1000);
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("无法初始化DFPlayer:");
    Serial.println("1. 请检查接线是否正确");
    Serial.println("2. 请检查SD卡是否插入");
    while(true);
  }
  
  Serial.println("DFPlayer Mini 初始化成功!");
  myDFPlayer.volume(20);  // 音量设置为15 (0~30)
  delay(500);
  
  // 逐个初始化舵机，避免电流冲击
  Serial.println("开始初始化舵机...");
  
  // 初始化翅膀舵机
  Serial.println("初始化翅膀舵机...");
  wingServo.attach(SERVO_WING);
  wingServo.write(WING_STOP);
  delay(500);
  
  // 初始化牙齿舵机
  Serial.println("初始化牙齿舵机...");
  teethServo.attach(SERVO_TEETH);
  teethServo.write(TEETH_CLOSE);
  delay(500);
  
  // 初始化门舵机
  Serial.println("初始化门舵机...");
  doorServo.attach(SERVO_DOOR);
  doorServo.write(DOOR_CLOSE);
  delay(500);
  
  // 初始化升降1舵机
  Serial.println("初始化升降1舵机...");
  liftServo1.attach(SERVO_LIFT1);
  liftServo1.write(LIFT_DOWN);
  delay(500);
  
  // 初始化升降2舵机
  Serial.println("初始化升降2舵机...");
  liftServo2.attach(SERVO_LIFT2);
  liftServo2.write(LIFT_DOWN);
  delay(500);
  
  Serial.println("所有模块初始化完成!");
  Serial.println("=== 系统就绪 ===");
  Serial.println("翼龙翅膀模块: 磁铁靠近引脚2");
  Serial.println("牙齿咬合模块: 磁铁靠近引脚4");
  Serial.println("门开合模块: 磁铁靠近引脚6");
  Serial.println("LED灯亮模块: 磁铁靠近引脚8");
  Serial.println("升降模块1: 磁铁靠近引脚A0");
  Serial.println("升降模块2: 磁铁靠近引脚A2");
}

void loop() {
  // 读取所有霍尔传感器状态
  bool currentHallWing = digitalRead(HALL_WING);
  bool currentHallTeeth = digitalRead(HALL_TEETH);
  bool currentHallDoor = digitalRead(HALL_DOOR);
  bool currentHallLed = digitalRead(HALL_LED);
  bool currentHallLift1 = digitalRead(HALL_LIFT1);
  bool currentHallLift2 = digitalRead(HALL_LIFT2);
  
  unsigned long currentTime = millis();
  
  // 检查每个传感器的状态变化（使用统一的防抖时间）
  if ((currentTime - lastDebounceTime) > debounceDelay) {
    
    // 翼龙翅膀模块
    if (currentHallWing == LOW && lastHallWing == HIGH) {
      toggleWing();
      lastDebounceTime = currentTime;
    }
    
    // 牙齿咬合模块
    if (currentHallTeeth == LOW && lastHallTeeth == HIGH) {
      toggleTeeth();
      lastDebounceTime = currentTime;
    }
    
    // 门开合模块
    if (currentHallDoor == LOW && lastHallDoor == HIGH) {
      toggleDoor();
      lastDebounceTime = currentTime;
    }
    
    // LED灯亮模块
    if (currentHallLed == LOW && lastHallLed == HIGH) {
      toggleLed();
      lastDebounceTime = currentTime;
    }
    
    // 升降模块1
    if (currentHallLift1 == LOW && lastHallLift1 == HIGH) {
      toggleLift1();
      lastDebounceTime = currentTime;
    }
    
    // 升降模块2
    if (currentHallLift2 == LOW && lastHallLift2 == HIGH) {
      toggleLift2();
      lastDebounceTime = currentTime;
    }
  }
  
  // 更新上一次状态
  lastHallWing = currentHallWing;
  lastHallTeeth = currentHallTeeth;
  lastHallDoor = currentHallDoor;
  lastHallLed = currentHallLed;
  lastHallLift1 = currentHallLift1;
  lastHallLift2 = currentHallLift2;
  
  // 处理DFPlayer反馈（如果有）
  if (myDFPlayer.available()) {
    printDFPlayerDetail(myDFPlayer.readType(), myDFPlayer.read());
  }
  
  delay(10);
}

// 切换翅膀状态函数
void toggleWing() {
  wingActive = !wingActive;
  
  if (wingActive) {
    // 启动翅膀 - 顺时针快速旋转
    wingServo.write(WING_CW_FAST);
    playSound(SOUND_WING);
    Serial.println("翅膀开始扇动 - 顺时针快速旋转 + 播放音效");
  } else {
    // 停止翅膀
    wingServo.write(WING_STOP);
    Serial.println("翅膀停止扇动");
  }
}

// 切换牙齿状态函数
void toggleTeeth() {
  teethActive = !teethActive;
  
  if (teethActive) {
    // 牙齿张开
    teethServo.write(TEETH_OPEN);
    playSound(SOUND_TEETH);
    Serial.println("牙齿张开 + 播放音效");
  } else {
    // 牙齿闭合
    teethServo.write(TEETH_CLOSE);
    Serial.println("牙齿闭合");
  }
}

// 切换门状态函数
void toggleDoor() {
  doorActive = !doorActive;
  
  if (doorActive) {
    // 门打开
    doorServo.write(DOOR_OPEN);
    playSound(SOUND_DOOR);
    Serial.println("门打开 + 播放音效");
  } else {
    // 门关闭
    doorServo.write(DOOR_CLOSE);
    Serial.println("门关闭");
  }
}

// 切换LED状态函数
void toggleLed() {
  ledActive = !ledActive;
  
  if (ledActive) {
    // LED灯亮
    digitalWrite(LED_PIN, HIGH);
    playSound(SOUND_LED);
    Serial.println("LED灯亮 + 播放音效");
  } else {
    // LED灯灭
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED灯灭");
  }
}

// 切换升降1状态函数
void toggleLift1() {
  lift1Active = !lift1Active;
  
  if (lift1Active) {
    // 升降1升起
    liftServo1.write(LIFT_UP);
    playSound(SOUND_LIFT);
    Serial.println("升降模块1升起 + 播放音效");
  } else {
    // 升降1下降
    liftServo1.write(LIFT_DOWN);
    Serial.println("升降模块1下降");
  }
}

// 切换升降2状态函数
void toggleLift2() {
  lift2Active = !lift2Active;
  
  if (lift2Active) {
    // 升降2升起
    liftServo2.write(LIFT_UP);
    playSound(SOUND_LIFT);
    Serial.println("升降模块2升起 + 播放音效");
  } else {
    // 升降2下降
    liftServo2.write(LIFT_DOWN);
    Serial.println("升降模块2下降");
  }
}

// 播放音效函数
void playSound(int trackNumber) {
  Serial.print("播放音效: ");
  Serial.println(trackNumber);
  myDFPlayer.play(trackNumber);
}

// 打印DFPlayer状态信息
void printDFPlayerDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println("操作超时!");
      break;
    case WrongStack:
      Serial.println("堆栈错误!");
      break;
    case DFPlayerCardInserted:
      Serial.println("SD卡插入!");
      break;
    case DFPlayerCardRemoved:
      Serial.println("SD卡拔出!");
      break;
    case DFPlayerCardOnline:
      Serial.println("SD卡就绪!");
      break;
    case DFPlayerPlayFinished:
      Serial.print("播放完成:");
      Serial.println(value);
      break;
    case DFPlayerError:
      Serial.print("DFPlayer错误:");
      switch (value) {
        case Busy:
          Serial.println("卡忙");
          break;
        case Sleeping:
          Serial.println("休眠中");
          break;
        case SerialWrongStack:
          Serial.println("串口堆栈错误");
          break;
        case CheckSumNotMatch:
          Serial.println("校验和不匹配");
          break;
        case FileIndexOut:
          Serial.println("文件索引超出范围");
          break;
        case FileMismatch:
          Serial.println("找不到文件");
          break;
        case Advertise:
          Serial.println("广告模式");
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}