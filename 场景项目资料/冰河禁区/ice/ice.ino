/*
 * 冰河禁区棋盘游戏 - 最终功能完整版
 * 引脚分配：
 * 模块一：转盘霍尔(2)，360度舵机(3)
 * 模块二：金钱霍尔(15)，MP3模块(10,11)，破产按键(4)
 * 模块三：触手霍尔(14)，180度舵机(5)
 * 
 * 功能：
 * 1. 磁铁靠近金钱霍尔(15) -> 播放金钱音效(0001.mp3)
 * 2. 按下破产按键(4) -> 播放破产音效(0002.mp3)
 * 3. 磁铁靠近触手霍尔(14) -> 播放Boss音效(0003.mp3)并摆动触手
 * 4. 磁铁靠近转盘霍尔(2) -> 360度舵机转动3秒停止
 */

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Servo.h>

// ====== 引脚定义 ======
// 模块一：大转盘
const int PIN_TURNTABLE_HALL = 2;     // 转盘霍尔 - 中断0
const int PIN_TURNTABLE_SERVO = 3;    // 360度舵机

// 模块二：音效与按键
const int PIN_MONEY_HALL = 15;        // 金钱霍尔 - A1的数字引脚号
const int PIN_BANKRUPT_BTN = 4;       // 破产按键
const int PIN_MP3_TX = 10;            // MP3模块RX
const int PIN_MP3_RX = 11;            // MP3模块TX

// 模块三：Boss触手
const int PIN_TENTACLE_HALL = 14;     // 触手霍尔 - A0的数字引脚号
const int PIN_TENTACLE_SERVO = 5;     // 180度舵机

// ====== 全局对象 ======
SoftwareSerial mp3Serial(PIN_MP3_RX, PIN_MP3_TX); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
Servo turntableServo;     // 转盘舵机
Servo tentacleServo;      // 触手舵机

// ====== 状态变量 ======
// 转盘状态
bool isTurntableSpinning = false;
unsigned long turntableStartTime = 0;
const unsigned long TURNTABLE_SPIN_TIME = 3000; // 转盘旋转3秒

// 触手状态
bool isTentacleActive = false;
unsigned long tentacleStartTime = 0;
const unsigned long TENTACLE_MOVE_TIME = 2000; // 触手动作时间

// 按键状态
bool lastBtnState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50;

// 霍尔传感器防抖
unsigned long lastMoneyTrigger = 0;
unsigned long lastTentacleTrigger = 0;
unsigned long lastTurntableTrigger = 0;
const unsigned long HALL_COOLDOWN = 1000; // 霍尔触发冷却时间

// ====== 音频文件索引 ======
enum SoundEffects {
  SOUND_MONEY = 1,    // 0001.mp3 - 金钱音效
  SOUND_BANKRUPT = 2, // 0002.mp3 - 破产音效
  SOUND_BOSS = 3      // 0003.mp3 - Boss音效
};

// ====== 初始化函数 ======
void setup() {
  // 1. 初始化串口
  Serial.begin(9600);
  Serial.println(F("=================================="));
  Serial.println(F("  冰河禁区棋盘游戏 - 最终版"));
  Serial.println(F("=================================="));
  
  // 2. 设置引脚模式
  pinMode(PIN_TURNTABLE_HALL, INPUT_PULLUP);
  pinMode(PIN_MONEY_HALL, INPUT_PULLUP);
  pinMode(PIN_TENTACLE_HALL, INPUT_PULLUP);
  pinMode(PIN_BANKRUPT_BTN, INPUT_PULLUP);
  
  // 3. 初始化舵机
  turntableServo.attach(PIN_TURNTABLE_SERVO);
  tentacleServo.attach(PIN_TENTACLE_SERVO);
  turntableServo.write(90);   // 360度舵机：90=停止
  tentacleServo.write(90);    // 180度舵机：90=中间位置
  
  // 4. 初始化MP3模块
  Serial.println(F("初始化MP3模块..."));
  mp3Serial.begin(9600);
  
  // 等待模块启动
  delay(2000);
  
  if (!myDFPlayer.begin(mp3Serial)) {
    Serial.println(F("MP3模块初始化失败!"));
    Serial.println(F("请检查:"));
    Serial.println(F("1. 接线是否正确"));
    Serial.println(F("2. SD卡是否插入"));
    Serial.println(F("3. 音量电位器是否调至中间"));
    Serial.println(F("系统将继续运行，但无声音"));
  } else {
    Serial.println(F("MP3模块初始化成功!"));
    myDFPlayer.volume(15);  // 音量0-30
    myDFPlayer.EQ(0);       // 均衡器模式
    
    // 播放启动提示音
    myDFPlayer.play(SOUND_MONEY);
    delay(500);
  }
  
  // 5. 显示操作说明
  Serial.println(F("\n=== 操作说明 ==="));
  Serial.println(F("1. 磁铁靠近金钱霍尔(15/A1) -> 金钱音效"));
  Serial.println(F("2. 按下破产按键(4) -> 破产音效"));
  Serial.println(F("3. 磁铁靠近触手霍尔(14/A0) -> Boss音效 + 触手动作"));
  Serial.println(F("4. 磁铁靠近转盘霍尔(2) -> 转盘旋转3秒"));
  Serial.println(F("=================================="));
  Serial.println(F("系统就绪!"));
  Serial.println(F("串口输入 'h' 查看调试命令"));
}

// ====== 主循环 ======
void loop() {
  unsigned long currentTime = millis();
  
  // 1. 检测转盘霍尔（引脚2）
  checkTurntableHall(currentTime);
  
  // 2. 检测金钱霍尔（引脚15/A1）
  checkMoneyHall(currentTime);
  
  // 3. 检测破产按键（引脚4）
  checkBankruptButton(currentTime);
  
  // 4. 检测触手霍尔（引脚14/A0）
  checkTentacleHall(currentTime);
  
  // 5. 更新转盘状态（如果正在旋转）
  updateTurntable(currentTime);
  
  // 6. 更新触手动画
  updateTentacleAnimation(currentTime);
}

// ====== 转盘霍尔检测函数 ======
void checkTurntableHall(unsigned long currentTime) {
  int hallState = digitalRead(PIN_TURNTABLE_HALL);
  
  // 检测到磁铁（低电平）
  if (hallState == LOW) {
    if (currentTime - lastTurntableTrigger > HALL_COOLDOWN) {
      startTurntable();
      lastTurntableTrigger = currentTime;
    }
  }
}

// ====== 金钱霍尔检测函数 ======
void checkMoneyHall(unsigned long currentTime) {
  int hallState = digitalRead(PIN_MONEY_HALL);
  
  // 检测到磁铁（低电平）
  if (hallState == LOW) {
    if (currentTime - lastMoneyTrigger > HALL_COOLDOWN) {
      playMoneySound();
      lastMoneyTrigger = currentTime;
    }
  }
}

// ====== 按键检测函数 ======
void checkBankruptButton(unsigned long currentTime) {
  int btnState = digitalRead(PIN_BANKRUPT_BTN);
  
  // 按键按下（低电平）
  if (btnState == LOW && lastBtnState == HIGH) {
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
      playBankruptSound();
      lastDebounceTime = currentTime;
    }
  }
  
  lastBtnState = btnState;
}

// ====== 触手霍尔检测函数 ======
void checkTentacleHall(unsigned long currentTime) {
  int hallState = digitalRead(PIN_TENTACLE_HALL);
  
  // 检测到磁铁（低电平）
  if (hallState == LOW) {
    if (currentTime - lastTentacleTrigger > HALL_COOLDOWN) {
      triggerTentacleAction(currentTime);
      lastTentacleTrigger = currentTime;
    }
  }
}

// ====== 转盘控制函数 ======
void startTurntable() {
  if (!isTurntableSpinning) {
    Serial.println(F("[转盘] 开始旋转3秒"));
    isTurntableSpinning = true;
    turntableStartTime = millis();
    turntableServo.write(70);  // 正转，中等速度（0-90：正转，数值越小速度越快）
  }
}

void stopTurntable() {
  if (isTurntableSpinning) {
    Serial.println(F("[转盘] 停止旋转"));
    isTurntableSpinning = false;
    turntableServo.write(90);  // 360度舵机停止信号
  }
}

void updateTurntable(unsigned long currentTime) {
  if (isTurntableSpinning && (currentTime - turntableStartTime > TURNTABLE_SPIN_TIME)) {
    stopTurntable();
  }
}

// ====== 触手控制函数 ======
void triggerTentacleAction(unsigned long currentTime) {
  if (!isTentacleActive) {
    Serial.println(F("[触手] Boss被激活!"));
    isTentacleActive = true;
    tentacleStartTime = currentTime;
    
    // 播放Boss音效
    myDFPlayer.play(SOUND_BOSS);
  }
}

void updateTentacleAnimation(unsigned long currentTime) {
  if (isTentacleActive) {
    unsigned long elapsed = currentTime - tentacleStartTime;
    
    if (elapsed < TENTACLE_MOVE_TIME) {
      // 创建摆动动画：在60-120度之间摆动
      float progress = elapsed / (float)TENTACLE_MOVE_TIME;
      int angle = 90 + 30 * sin(progress * 4 * PI); // 摆动2个周期
      tentacleServo.write(angle);
    } else {
      // 动画结束，回到中间位置
      tentacleServo.write(90);
      isTentacleActive = false;
      Serial.println(F("[触手] 恢复平静"));
    }
  }
}

// ====== 音效播放函数 ======
void playMoneySound() {
  Serial.println(F("[音效] 金钱音效"));
  myDFPlayer.play(SOUND_MONEY);
}

void playBankruptSound() {
  Serial.println(F("[音效] 破产音效"));
  myDFPlayer.play(SOUND_BANKRUPT);
}

// ====== 串口调试命令 ======
void serialEvent() {
  if (Serial.available()) {
    char command = Serial.read();
    
    switch (command) {
      case '1':
        Serial.println(F("播放金钱音效"));
        playMoneySound();
        break;
        
      case '2':
        Serial.println(F("播放破产音效"));
        playBankruptSound();
        break;
        
      case '3':
        Serial.println(F("播放Boss音效"));
        myDFPlayer.play(SOUND_BOSS);
        break;
        
      case 's':
        Serial.println(F("启动转盘旋转3秒"));
        startTurntable();
        break;
        
      case 'x':
        Serial.println(F("停止转盘"));
        stopTurntable();
        break;
        
      case 't':
        Serial.println(F("触发触手动作"));
        triggerTentacleAction(millis());
        break;
        
      case 'd':
        printDebugInfo();
        break;
        
      case 'h':
        printHelp();
        break;
        
      case 'r':
        Serial.println(F("系统重置"));
        resetSystem();
        break;
        
      case 'v':
        myDFPlayer.volume(30);
        Serial.println(F("音量设为最大(30)"));
        break;
        
      case 'p':
        testAllPins();
        break;
        
      case 'm':
        // 测试转盘速度
        Serial.println(F("测试转盘速度: 慢"));
        turntableServo.write(80);
        delay(2000);
        turntableServo.write(90);
        delay(1000);
        
        Serial.println(F("测试转盘速度: 中"));
        turntableServo.write(70);
        delay(2000);
        turntableServo.write(90);
        delay(1000);
        
        Serial.println(F("测试转盘速度: 快"));
        turntableServo.write(60);
        delay(2000);
        turntableServo.write(90);
        break;
    }
  }
}

// ====== 调试函数 ======
void printDebugInfo() {
  Serial.println(F("\n=== 系统状态 ==="));
  Serial.print(F("转盘状态: "));
  Serial.println(isTurntableSpinning ? "旋转中" : "停止");
  
  Serial.print(F("触手状态: "));
  Serial.println(isTentacleActive ? "激活" : "静止");
  
  Serial.print(F("金钱霍尔(15): "));
  Serial.println(digitalRead(PIN_MONEY_HALL) == HIGH ? "无磁铁" : "磁铁靠近");
  
  Serial.print(F("触手霍尔(14): "));
  Serial.println(digitalRead(PIN_TENTACLE_HALL) == HIGH ? "无磁铁" : "磁铁靠近");
  
  Serial.print(F("转盘霍尔(2): "));
  Serial.println(digitalRead(PIN_TURNTABLE_HALL) == HIGH ? "无磁铁" : "磁铁靠近");
  
  Serial.print(F("破产按键(4): "));
  Serial.println(digitalRead(PIN_BANKRUPT_BTN) == HIGH ? "未按下" : "按下");
  
  if (isTurntableSpinning) {
    unsigned long elapsed = millis() - turntableStartTime;
    Serial.print(F("转盘已旋转: "));
    Serial.print(elapsed);
    Serial.println(F("毫秒"));
  }
  
  Serial.println(F("================\n"));
}

void testAllPins() {
  Serial.println(F("\n=== 引脚状态测试 ==="));
  Serial.print(F("转盘霍尔(2): "));
  Serial.println(digitalRead(PIN_TURNTABLE_HALL));
  
  Serial.print(F("金钱霍尔(15): "));
  Serial.println(digitalRead(PIN_MONEY_HALL));
  
  Serial.print(F("触手霍尔(14): "));
  Serial.println(digitalRead(PIN_TENTACLE_HALL));
  
  Serial.print(F("破产按键(4): "));
  Serial.println(digitalRead(PIN_BANKRUPT_BTN));
  
  Serial.println(F("测试完成"));
}

void printHelp() {
  Serial.println(F("\n=== 串口调试命令 ==="));
  Serial.println(F("1 - 播放金钱音效"));
  Serial.println(F("2 - 播放破产音效"));
  Serial.println(F("3 - 播放Boss音效"));
  Serial.println(F("s - 启动转盘旋转3秒"));
  Serial.println(F("x - 停止转盘"));
  Serial.println(F("t - 触发触手动作"));
  Serial.println(F("d - 显示系统状态"));
  Serial.println(F("p - 测试所有引脚状态"));
  Serial.println(F("m - 测试转盘速度"));
  Serial.println(F("r - 重置系统"));
  Serial.println(F("v - 设置最大音量"));
  Serial.println(F("h - 显示帮助"));
  Serial.println(F("====================\n"));
}

void resetSystem() {
  // 停止转盘
  stopTurntable();
  
  // 触手回到中间位置
  tentacleServo.write(90);
  isTentacleActive = false;
  
  Serial.println(F("系统已重置"));
}