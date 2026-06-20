#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// 设置软件串口
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// 霍尔传感器引脚
const int hallSensorPin = 2;

// 变量用于检测状态变化
int lastHallState = HIGH;
int currentHallState;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // 防抖延迟(毫秒)

// 当前播放的歌曲编号
int currentTrack = 1;
// 总歌曲数量（根据您的SD卡中的实际歌曲数量修改）
const int totalTracks = 10;

void setup() {
  // 初始化硬件串口
  Serial.begin(115200);
  
  // 初始化软件串口
  mySoftwareSerial.begin(9600);
  
  // 设置霍尔传感器引脚为输入模式
  pinMode(hallSensorPin, INPUT_PULLUP);
  
  Serial.println();
  Serial.println(F("初始化DFPlayer ..."));
  
  // 延迟一段时间确保模块启动
  delay(2000);
  
  // 尝试初始化DFPlayer
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("无法初始化DFPlayer:"));
    Serial.println(F("1. 请检查接线是否正确"));
    Serial.println(F("2. 请检查SD卡是否插入"));
    while(true);
  }
  
  Serial.println(F("DFPlayer Mini 初始化成功!"));
  
  // 设置初始参数
  myDFPlayer.volume(15);  // 设置音量(0~30)
  myDFPlayer.EQ(0);       // 设置均衡器
  
  Serial.println(F("系统就绪，等待磁铁靠近霍尔传感器..."));
  Serial.println(F("每次磁铁靠近将播放下一首歌曲"));
}

void loop() {
  // 读取霍尔传感器状态
  int reading = digitalRead(hallSensorPin);
  
  // 检查是否状态变化（防抖处理）
  if (reading != lastHallState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // 状态稳定后的处理
    if (reading != currentHallState) {
      currentHallState = reading;
      
      // 检测到磁铁靠近（低电平触发）
      if (currentHallState == LOW) {
        playNextTrack();
      }
    }
  }
  
  lastHallState = reading;
  
  // 处理DFPlayer反馈（如果有）
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
  }
}

// 播放下一首歌曲
void playNextTrack() {
  currentTrack++;
  if (currentTrack > totalTracks) {
    currentTrack = 1; // 循环回到第一首
  }
  
  Serial.print(F("播放第 "));
  Serial.print(currentTrack);
  Serial.println(F(" 首歌曲"));
  
  myDFPlayer.play(currentTrack);
}

// 打印DFPlayer的详细状态信息
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("操作超时!"));
      break;
    case WrongStack:
      Serial.println(F("堆栈错误!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("SD卡插入!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("SD卡拔出!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("SD卡就绪!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("播放完成:"));
      Serial.println(value);
      break;
    case DFPlayerError:
      Serial.print(F("错误:"));
      switch (value) {
        case Busy:
          Serial.println(F("卡忙"));
          break;
        case Sleeping:
          Serial.println(F("休眠中"));
          break;
        case SerialWrongStack:
          Serial.println(F("串口堆栈错误"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("校验和不匹配"));
          break;
        case FileIndexOut:
          Serial.println(F("文件索引超出范围"));
          break;
        case FileMismatch:
          Serial.println(F("找不到文件"));
          break;
        case Advertise:
          Serial.println(F("广告模式"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}