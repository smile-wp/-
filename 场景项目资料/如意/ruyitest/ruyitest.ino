#include <Servo.h>

// 定义引脚
const int hallSensor1 = 2;    // 模块一霍尔传感器
const int servo360_1 = 3;     // 模块一360度舵机
const int hallSensor2 = 4;    // 模块二霍尔传感器  
const int servo360_2 = 5;     // 模块二360度舵机
const int hallSensor3 = 6;    // 模块三霍尔传感器
const int servo360_3 = 9;     // 模块三360度舵机
const int ledPin = 13;        // LED引脚

// 创建舵机对象
Servo servo1;  // 模块一360度舵机
Servo servo2;  // 模块二360度舵机  
Servo servo3;  // 模块三360度舵机

// 状态变量
bool servo1NextDirection = false; // false=顺时针, true=逆时针
bool servo2NextDirection = false; // false=顺时针, true=逆时针
bool servo3State = false;         // false=停止, true=转动
bool lastHall1State = HIGH;
bool lastHall2State = HIGH;
bool lastHall3State = HIGH;

// 舵机转速控制变量
int servo1CurrentSpeed = 90;  // 模块一当前速度 (90=停止)
int servo2CurrentSpeed = 90;  // 模块二当前速度 (90=停止)
int servo1TargetSpeed = 90;   // 模块一目标速度
int servo2TargetSpeed = 90;   // 模块二目标速度
bool servo1Moving = false;    // 模块一是否正在调整速度
bool servo2Moving = false;    // 模块二是否正在调整速度
const int STEP_DELAY = 10;    // 每一步的延迟时间（毫秒），增大此值会减慢速度
const int STEP_SIZE = 5;      // 每一步的速度变化量，减小此值会减慢速度

// 舵机角度/时间控制
const int STOP_SPEED = 90;    // 停止速度

// 旋转角度设置（通过控制旋转时间实现）
unsigned long servo1StartTime = 0;  // 模块一开始旋转的时间
unsigned long servo2StartTime = 0;  // 模块二开始旋转的时间
int servo1RotationTime = 900;       // 模块一旋转时间（毫秒）- 控制"角度"
int servo2RotationTime = 1000;      // 模块二旋转时间（毫秒）- 控制"角度"
bool servo1TimedRotation = false;   // 模块一是否正在进行定时旋转
bool servo2TimedRotation = false;   // 模块二是否正在进行定时旋转

void setup() {
  // 初始化引脚
  pinMode(hallSensor1, INPUT_PULLUP);
  pinMode(hallSensor2, INPUT_PULLUP);
  pinMode(hallSensor3, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  
  // 连接舵机
  servo1.attach(servo360_1);
  servo2.attach(servo360_2);
  servo3.attach(servo360_3);
  
  // 初始化舵机位置（停止）
  servo1.write(STOP_SPEED);
  servo2.write(STOP_SPEED);
  servo3.write(STOP_SPEED);
  
  // 初始化速度变量
  servo1CurrentSpeed = STOP_SPEED;
  servo2CurrentSpeed = STOP_SPEED;
  servo1TargetSpeed = STOP_SPEED;
  servo2TargetSpeed = STOP_SPEED;
  
  // 点亮LED
  digitalWrite(ledPin, HIGH);
  
  Serial.begin(9600);
  Serial.println("系统初始化完成 - 所有舵机均为360度舵机");
  Serial.println("模块一：磁铁第一次靠近顺时针转，第二次靠近逆时针转，第三次靠近顺时针转（交替方向）");
  Serial.println("模块二：磁铁第一次靠近顺时针转，第二次靠近逆时针转，第三次靠近顺时针转（交替方向）");
  Serial.println("默认旋转时间: 模块一=" + String(servo1RotationTime) + "ms, 模块二=" + String(servo2RotationTime) + "ms");
}

void loop() {
  // 读取霍尔传感器状态（使用上拉电阻，磁铁靠近时为LOW）
  bool hall1State = digitalRead(hallSensor1);
  bool hall2State = digitalRead(hallSensor2);
  bool hall3State = digitalRead(hallSensor3);
  
  // 模块一：360度舵机控制 - 交替方向旋转
  if (hall1State == LOW && lastHall1State == HIGH) {
    delay(50); // 防抖动
    
    // 只有在没有定时旋转时才响应新的触发
    if (!servo1TimedRotation) {
      // 根据状态决定旋转方向
      // servo1NextDirection: false=顺时针, true=逆时针
      servo1TargetSpeed = servo1NextDirection ? 180 : 0; // 根据状态选择方向
      servo1TimedRotation = true;
      servo1StartTime = millis(); // 记录开始时间
      
      String directionStr = servo1NextDirection ? "逆时针" : "顺时针";
      Serial.println("模块一：开始" + directionStr + "旋转 " + String(servo1RotationTime) + "ms");
      
      // 切换下一次的方向
      servo1NextDirection = !servo1NextDirection;
      
      servo1Moving = true; // 开始调整速度
    } else {
      Serial.println("模块一：正在旋转中，忽略触发");
    }
    
    delay(300); // 防止重复触发
  }
  lastHall1State = hall1State;
  
  // 模块二：360度舵机控制 - 交替方向旋转
  if (hall2State == LOW && lastHall2State == HIGH) {
    delay(50); // 防抖动
    
    // 只有在没有定时旋转时才响应新的触发
    if (!servo2TimedRotation) {
      // 根据状态决定旋转方向
      // servo2NextDirection: false=顺时针, true=逆时针
      servo2TargetSpeed = servo2NextDirection ? 180 : 0; // 根据状态选择方向
      servo2TimedRotation = true;
      servo2StartTime = millis(); // 记录开始时间
      
      String directionStr = servo2NextDirection ? "逆时针" : "顺时针";
      Serial.println("模块二：开始" + directionStr + "旋转 " + String(servo2RotationTime) + "ms");
      
      // 切换下一次的方向
      servo2NextDirection = !servo2NextDirection;
      
      servo2Moving = true; // 开始调整速度
    } else {
      Serial.println("模块二：正在旋转中，忽略触发");
    }
    
    delay(300);
  }
  lastHall2State = hall2State;
  
  // 模块三：360度舵机控制
  if (hall3State == LOW && lastHall3State == HIGH) {
    delay(50); // 防抖动
    
    if (servo3State) {
      // 停止转动
      servo3.write(STOP_SPEED);
      servo3State = false;
      Serial.println("模块三：舵机停止");
    } else {
      // 开始转动
      servo3.write(0);  // 顺时针转动
      servo3State = true;
      Serial.println("模块三：舵机开始顺时针转动");
    }
    
    delay(300);
  }
  lastHall3State = hall3State;
  
  // 检查模块一定时旋转是否完成
  if (servo1TimedRotation && (millis() - servo1StartTime >= servo1RotationTime)) {
    servo1TargetSpeed = STOP_SPEED;
    servo1TimedRotation = false;
    servo1Moving = true;
    Serial.println("模块一：定时旋转完成");
  }
  
  // 检查模块二定时旋转是否完成
  if (servo2TimedRotation && (millis() - servo2StartTime >= servo2RotationTime)) {
    servo2TargetSpeed = STOP_SPEED;
    servo2TimedRotation = false;
    servo2Moving = true;
    Serial.println("模块二：定时旋转完成");
  }
  
  // 缓慢调整模块一舵机速度
  if (servo1Moving) {
    if (servo1CurrentSpeed != servo1TargetSpeed) {
      // 计算移动方向
      if (servo1CurrentSpeed < servo1TargetSpeed) {
        servo1CurrentSpeed += STEP_SIZE;
        if (servo1CurrentSpeed > servo1TargetSpeed) {
          servo1CurrentSpeed = servo1TargetSpeed;
        }
      } else {
        servo1CurrentSpeed -= STEP_SIZE;
        if (servo1CurrentSpeed < servo1TargetSpeed) {
          servo1CurrentSpeed = servo1TargetSpeed;
        }
      }
      
      servo1.write(servo1CurrentSpeed);
      delay(STEP_DELAY); // 控制速度变化速率
    } else {
      servo1Moving = false; // 停止调整
    }
  }
  
  // 缓慢调整模块二舵机速度
  if (servo2Moving) {
    if (servo2CurrentSpeed != servo2TargetSpeed) {
      // 计算移动方向
      if (servo2CurrentSpeed < servo2TargetSpeed) {
        servo2CurrentSpeed += STEP_SIZE;
        if (servo2CurrentSpeed > servo2TargetSpeed) {
          servo2CurrentSpeed = servo2TargetSpeed;
        }
      } else {
        servo2CurrentSpeed -= STEP_SIZE;
        if (servo2CurrentSpeed < servo2TargetSpeed) {
          servo2CurrentSpeed = servo2TargetSpeed;
        }
      }
      
      servo2.write(servo2CurrentSpeed);
      delay(STEP_DELAY); // 控制速度变化速率
    } else {
      servo2Moving = false; // 停止调整
    }
  }
  
  // 通过串口命令修改参数
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input == "STATUS") {
      Serial.println("=== 系统状态 ===");
      String module1Direction = servo1NextDirection ? "逆时针" : "顺时针";
      Serial.println("模块一: " + String(servo1TimedRotation ? "转动" : "停止") + 
                    ", 下次方向: " + module1Direction + 
                    ", 旋转时间: " + String(servo1RotationTime) + "ms");
      String module2Direction = servo2NextDirection ? "逆时针" : "顺时针";
      Serial.println("模块二: " + String(servo2TimedRotation ? "转动" : "停止") + 
                    ", 下次方向: " + module2Direction + 
                    ", 旋转时间: " + String(servo2RotationTime) + "ms");
      Serial.println("模块三: " + String(servo3State ? "转动" : "停止"));
    } else if (input.startsWith("S1_TIME:")) {
      int newTime = input.substring(8).toInt();
      if (newTime > 0 && newTime <= 10000) {
        servo1RotationTime = newTime;
        Serial.println("模块一旋转时间设置为: " + String(servo1RotationTime) + "ms");
      }
    } else if (input.startsWith("S2_TIME:")) {
      int newTime = input.substring(8).toInt();
      if (newTime > 0 && newTime <= 10000) {
        servo2RotationTime = newTime;
        Serial.println("模块二旋转时间设置为: " + String(servo2RotationTime) + "ms");
      }
    } else if (input == "S1_RESET") {
      // 重置模块一方向为顺时针
      servo1NextDirection = false;
      Serial.println("模块一方向已重置为顺时针");
    } else if (input == "S2_RESET") {
      // 重置模块二方向为顺时针
      servo2NextDirection = false;
      Serial.println("模块二方向已重置为顺时针");
    } else if (input == "RESET_ALL") {
      // 重置所有模块方向为顺时针
      servo1NextDirection = false;
      servo2NextDirection = false;
      Serial.println("所有模块方向已重置为顺时针");
    } else if (input == "HELP") {
      Serial.println("=== 串口命令帮助 ===");
      Serial.println("STATUS - 显示系统状态");
      Serial.println("S1_TIME:[毫秒] - 设置模块一旋转时间 (1-10000ms)");
      Serial.println("S2_TIME:[毫秒] - 设置模块二旋转时间 (1-10000ms)");
      Serial.println("S1_RESET - 重置模块一方向为顺时针");
      Serial.println("S2_RESET - 重置模块二方向为顺时针");
      Serial.println("RESET_ALL - 重置所有模块方向为顺时针");
      Serial.println("HELP - 显示此帮助信息");
    }
  }
  
  delay(10); // 主循环短暂延迟
}