#include <Servo.h>

// 定义引脚
const int hallSensor1 = 2;    // 模块一霍尔传感器
const int servo180_1 = 3;     // 模块一180度舵机
const int hallSensor2 = 4;    // 模块二霍尔传感器  
const int servo180_2 = 5;     // 模块二180度舵机
const int hallSensor3 = 6;    // 模块三霍尔传感器
const int servo360 = 9;       // 模块三360度舵机
const int ledPin = 13;        // LED引脚

// 创建舵机对象
Servo servo1;  // 模块一180度舵机
Servo servo2;  // 模块二180度舵机  
Servo servo3;  // 模块三360度舵机

// 状态变量
bool servo1State = false;     // false=0度, true=180度
bool servo2State = false;     // false=0度, true=180度
bool servo3State = false;     // false=停止, true=转动
bool lastHall1State = HIGH;
bool lastHall2State = HIGH;
bool lastHall3State = HIGH;

void setup() {
  // 初始化引脚
  pinMode(hallSensor1, INPUT_PULLUP);
  pinMode(hallSensor2, INPUT_PULLUP);
  pinMode(hallSensor3, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  
  // 连接舵机
  servo1.attach(servo180_1);
  servo2.attach(servo180_2);
  servo3.attach(servo360);
  
  // 初始化舵机位置
  servo1.write(0);    // 初始0度
  servo2.write(0);    // 初始0度
  servo3.write(90);   // 360度舵机停止位置
  
  // 点亮LED
  digitalWrite(ledPin, HIGH);
  
  Serial.begin(9600);
  Serial.println("系统初始化完成");
}

void loop() {
  // 读取霍尔传感器状态（使用上拉电阻，磁铁靠近时为LOW）
  bool hall1State = digitalRead(hallSensor1);
  bool hall2State = digitalRead(hallSensor2);
  bool hall3State = digitalRead(hallSensor3);
  
  // 模块一：180度舵机控制
  if (hall1State == LOW && lastHall1State == HIGH) {
    // 检测到下降沿（磁铁靠近）
    delay(50); // 防抖动
    
    if (servo1State) {
      // 当前在180度，回到0度
      servo1.write(0);
      servo1State = false;
      Serial.println("模块一：舵机转到0度");
    } else {
      // 当前在0度，转到180度
      servo1.write(120);
      servo1State = true;
      Serial.println("模块一：舵机转到180度");
    }
    
    delay(300); // 防止重复触发
  }
  lastHall1State = hall1State;
  
  // 模块二：180度舵机控制（与模块一相同逻辑）
  if (hall2State == LOW && lastHall2State == HIGH) {
    delay(50); // 防抖动
    
    if (servo2State) {
      servo2.write(0);
      servo2State = false;
      Serial.println("模块二：舵机转到0度");
    } else {
      servo2.write(180);
      servo2State = true;
      Serial.println("模块二：舵机转到180度");
    }
    
    delay(300);
  }
  lastHall2State = hall2State;
  
  // 模块三：360度舵机控制
  if (hall3State == LOW && lastHall3State == HIGH) {
    delay(50); // 防抖动
    
    if (servo3State) {
      // 停止转动（90度位置使360度舵机停止）
      servo3.write(90);
      servo3State = false;
      Serial.println("模块三：舵机停止");
    } else {
      // 开始转动（0度或180度使360度舵机旋转，方向不同）
      servo3.write(0);  // 顺时针转动
      servo3State = true;
      Serial.println("模块三：舵机开始转动");
    }
    
    delay(300);
  }
  lastHall3State = hall3State;
  
  delay(10); // 主循环短暂延迟
}