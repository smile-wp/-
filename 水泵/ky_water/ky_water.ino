// 水泵霍尔开关控制 - 切换模式
// 磁铁第一次靠近：水泵运行
// 磁铁第二次靠近：水泵暂停
// 循环此模式

// 引脚定义
const int hallSensorPin = 2;    // KY003霍尔传感器连接到数字引脚2（使用中断引脚）
const int pumpRelayPin = 3;     // 水泵继电器控制引脚
const int statusLedPin = 13;    // 状态指示灯
const int pumpLedPin = 12;      // 水泵运行指示灯（可选）

// 变量定义
volatile bool magnetDetected = false;          // 磁铁检测标志
bool pumpState = false;                        // 水泵当前状态：false=停止，true=运行
bool lastHallState = HIGH;                     // 上次霍尔传感器状态
unsigned long lastDebounceTime = 0;            // 上次检测时间（用于防抖动）
unsigned long debounceDelay = 100;             // 防抖动延时（毫秒）
unsigned long pumpStartTime = 0;               // 水泵启动时间
unsigned long minRunTime = 2000;               // 最小运行时间（防止频繁开关，单位：毫秒）

// 中断服务函数
void magnetISR() {
  magnetDetected = true;
}

void setup() {
  // 初始化串口通信
  Serial.begin(9600);
  Serial.println("水泵霍尔控制系统 - 切换模式");
  Serial.println("第一次磁铁靠近：启动水泵");
  Serial.println("第二次磁铁靠近：停止水泵");
  
  // 初始化引脚
  pinMode(hallSensorPin, INPUT_PULLUP);    // 霍尔传感器使用内部上拉电阻
  pinMode(pumpRelayPin, OUTPUT);
  pinMode(statusLedPin, OUTPUT);
  pinMode(pumpLedPin, OUTPUT);
  
  // 初始状态
  digitalWrite(pumpRelayPin, LOW);         // 确保水泵初始为关闭状态
  digitalWrite(statusLedPin, LOW);
  digitalWrite(pumpLedPin, LOW);
  
  // 读取初始霍尔状态
  lastHallState = digitalRead(hallSensorPin);
  
  // 配置中断
  // 注意：KY003通常输出低电平当检测到磁铁时
  // 使用CHANGE模式检测上升沿和下降沿
  attachInterrupt(digitalPinToInterrupt(hallSensorPin), magnetISR, FALLING);
  
  Serial.println("系统初始化完成");
  Serial.println("等待磁铁靠近...");
  Serial.println("当前水泵状态: 停止");
}

void loop() {
  // 检查磁铁检测事件
  if (magnetDetected) {
    delay(20);  // 简单延时防抖动
    handleMagnetEvent();
    magnetDetected = false;
  }
  
  // 更新指示灯状态
  updateLEDs();
  
  // 检查串口命令（可选）
  checkSerialCommands();
}

// 处理磁铁事件
void handleMagnetEvent() {
  // 读取当前霍尔状态
  int hallState = digitalRead(hallSensorPin);
  
  // 确保检测到的是磁铁靠近（低电平）
  if (hallState == LOW) {
    // 检查是否满足防抖动条件
    unsigned long currentTime = millis();
    
    if (currentTime - lastDebounceTime > debounceDelay) {
      // 防抖动通过，处理磁铁事件
      lastDebounceTime = currentTime;
      
      Serial.println("检测到磁铁靠近");
      
      // 切换水泵状态
      togglePumpState();
    }
  }
}

// 切换水泵状态
void togglePumpState() {
  // 防止在最小运行时间内重复开关
  if (pumpState && (millis() - pumpStartTime < minRunTime)) {
    Serial.println("水泵刚启动，跳过本次操作");
    return;
  }
  
  // 切换状态
  pumpState = !pumpState;
  
  if (pumpState) {
    // 启动水泵
    turnOnPump();
    pumpStartTime = millis();  // 记录启动时间
    Serial.println("水泵已启动");
  } else {
    // 停止水泵
    turnOffPump();
    Serial.println("水泵已停止");
  }
}

// 开启水泵
void turnOnPump() {
  digitalWrite(pumpRelayPin, HIGH);  // 激活继电器
  digitalWrite(pumpLedPin, HIGH);    // 水泵指示灯亮
}

// 关闭水泵
void turnOffPump() {
  digitalWrite(pumpRelayPin, LOW);   // 关闭继电器
  digitalWrite(pumpLedPin, LOW);     // 水泵指示灯灭
}

// 更新指示灯状态
void updateLEDs() {
  // 状态指示灯闪烁（表示系统正常运行）
  static unsigned long lastBlinkTime = 0;
  static bool blinkState = LOW;
  
  unsigned long currentTime = millis();
  
  if (currentTime - lastBlinkTime > 1000) {  // 每秒闪烁一次
    lastBlinkTime = currentTime;
    blinkState = !blinkState;
    digitalWrite(statusLedPin, blinkState);
  }
}

// 检查串口命令（可选功能）
void checkSerialCommands() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    switch (command) {
      case '1':
        if (!pumpState) {
          pumpState = true;
          turnOnPump();
          Serial.println("手动启动水泵");
        }
        break;
        
      case '0':
        if (pumpState) {
          pumpState = false;
          turnOffPump();
          Serial.println("手动停止水泵");
        }
        break;
        
      case 's':
        Serial.print("水泵状态: ");
        Serial.println(pumpState ? "运行中" : "已停止");
        Serial.print("运行时间: ");
        Serial.print((millis() - pumpStartTime) / 1000);
        Serial.println(" 秒");
        Serial.print("霍尔传感器: ");
        Serial.println(digitalRead(hallSensorPin) ? "无磁铁" : "检测到磁铁");
        break;
        
      case 'r':
        pumpState = false;
        turnOffPump();
        Serial.println("系统已重置");
        break;
    }
  }
}

// 可选：添加安全保护功能
void safetyCheck() {
  // 如果水泵运行时间过长，自动关闭（防止干转）
  static const unsigned long maxRunTime = 5 * 60 * 1000; // 5分钟（可根据需要调整）
  
  if (pumpState && (millis() - pumpStartTime > maxRunTime)) {
    pumpState = false;
    turnOffPump();
    Serial.println("安全保护：水泵运行时间过长，已自动关闭");
    
    // 报警提示
    for (int i = 0; i < 10; i++) {
      digitalWrite(statusLedPin, HIGH);
      delay(100);
      digitalWrite(statusLedPin, LOW);
      delay(100);
    }
  }
}