# Arduino Uno控制360度连续旋转舵机详细教程

## 一、认识360度舵机

### 什么是360度舵机？

360度舵机（又称连续旋转舵机）是一种特殊的伺服电机，它**不能控制旋转的角度**，而是**控制旋转的方向和速度**。这与我们常见的180度舵机有本质区别。

### 360度舵机 vs 180度舵机对比表

| 特性               | 180度舵机（标准舵机）    | 360度舵机（连续旋转舵机）  |
| :----------------- | :----------------------- | :------------------------- |
| **控制对象**       | 角度位置（0°～180°）     | 旋转方向和速度             |
| **转动范围**       | 有限角度（不能连续旋转） | 可无限连续旋转             |
| **内部结构**       | 有位置反馈电位器         | 无位置反馈（或反馈被移除） |
| **Arduino控制**    | `write(角度)`            | `write(速度值)`            |
| **中点值（停止）** | 90°对应中间位置          | 90°对应停止                |
| **应用场景**       | 机械臂、云台、转向机构   | 小车驱动、传送带、旋转平台 |

**重要提示**：MG90S通常是180度舵机，但市面上也有360度版本的MG90S（或类似尺寸的360度舵机，如FT90R）。购买时请务必确认产品说明。

------

## 二、360度舵机参数与接线

### 典型360度微型舵机参数（以DM-S0090D/FT90R为例）

- **工作电压**：4.8V ~ 6V（建议5V）
- **工作电流**：100-250mA
- **扭矩**：约1.5-1.6kg·cm
- **旋转类型**：360度连续旋转
- **重量**：约9克
- **接口**：三线制（与普通舵机相同）

### 舵机线序（与180度舵机完全相同）

| 线色          | 功能            | 连接Arduino     |
| :------------ | :-------------- | :-------------- |
| **橙色/黄色** | 信号线（PWM）   | 数字引脚（如9） |
| **红色**      | 电源正极（VCC） | 5V              |
| **棕色/黑色** | 电源负极（GND） | GND             |

### 硬件连接图

text

```
Arduino Uno                 360度舵机
    5V  ------------------- 红线 (VCC)
    GND ------------------- 棕线 (GND)
    数字引脚9 ------------- 橙线 (信号)
```



------

## 三、360度舵机的控制原理

### PWM信号控制解读

360度舵机使用与180度舵机相同的PWM协议：周期20ms（50Hz），通过改变高电平脉冲宽度来控制。

| 脉冲宽度 | 对应`write()`值 | 360度舵机行为          |
| :------- | :-------------- | :--------------------- |
| 0.5ms    | 0               | **最大速度顺时针**旋转 |
| 1.5ms    | 90              | **停止**（中点）       |
| 2.5ms    | 180             | **最大速度逆时针**旋转 |

**速度控制规律**：

- **0～89**：顺时针旋转，数值越小速度越快
- **90**：停止
- **91～180**：逆时针旋转，数值越大速度越快

**重要提示**：不同品牌的360度舵机，其中点（停止点）可能略有偏差。有些舵机在`write(90)`时可能不会完全停止，需要微调到`write(91)`或`write(89)`才能停止。建议实际测试找出准确的停止值。

------

## 四、基础控制程序

### 准备工作：包含Servo库

cpp

```
#include <Servo.h>  // Arduino自带库，无需额外安装
Servo myServo;       // 创建舵机对象
```



### 程序1：基础速度控制（正转-停止-反转循环）

cpp

```c++
#include <Servo.h>

Servo myServo;  // 创建舵机对象

void setup() {
  myServo.attach(9);  // 将舵机连接到引脚9
  myServo.write(90);  // 初始状态：停止
  delay(1000);        // 等待1秒
}

void loop() {
  // 顺时针旋转（速度由慢到快演示）
  myServo.write(70);   // 较慢顺时针
  delay(2000);
  
  myServo.write(50);   // 中等顺时针
  delay(2000);
  
  myServo.write(20);   // 快速顺时针
  delay(2000);
  
  myServo.write(90);   // 停止
  delay(2000);
  
  // 逆时针旋转（速度由慢到快演示）
  myServo.write(110);  // 较慢逆时针
  delay(2000);
  
  myServo.write(130);  // 中等逆时针
  delay(2000);
  
  myServo.write(160);  // 快速逆时针
  delay(2000);
  
  myServo.write(90);   // 停止
  delay(2000);
}
```



### 程序2：使用`writeMicroseconds()`精确控制

对于需要更精确控制的场合，可以使用`writeMicroseconds()`函数直接设置脉冲宽度（微秒）。

cpp

```
#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(9);
  
  // 设置最小和最大脉冲宽度（可选，但推荐）
  // 参数：引脚, 最小脉冲(us), 最大脉冲(us)
  // myServo.attach(9, 500, 2500);
}

void loop() {
  // 顺时针最大速度 (500μs脉冲)
  myServo.writeMicroseconds(500);
  delay(2000);
  
  // 停止 (1500μs脉冲)
  myServo.writeMicroseconds(1500);
  delay(2000);
  
  // 逆时针最大速度 (2500μs脉冲)
  myServo.writeMicroseconds(2500);
  delay(2000);
  
  // 停止
  myServo.writeMicroseconds(1500);
  delay(2000);
}
```



### 程序3：串口控制速度和方向

通过串口监视器输入速度值（0-180）来控制舵机。

cpp

```
#include <Servo.h>

Servo myServo;
int speedValue = 90;  // 默认停止

void setup() {
  Serial.begin(9600);// 初始化串口通信
  myServo.attach(9);
  myServo.write(speedValue);
  
  Serial.println("360度舵机控制程序启动");
  Serial.println("请输入速度值 (0-180):");
  Serial.println("0=最快顺时针, 90=停止, 180=最快逆时针");
}

void loop() {
  if (Serial.available() > 0) {
    int input = Serial.parseInt();
    
    if (input >= 0 && input <= 180) {
      speedValue = input;
      myServo.write(speedValue);
      
      // 判断方向并显示
      Serial.print("设置速度: ");
      Serial.print(speedValue);
      Serial.print(" - ");
      
      if (speedValue < 90) {
        Serial.print("顺时针旋转, 速度值: ");
        Serial.println(90 - speedValue);
      } else if (speedValue > 90) {
        Serial.print("逆时针旋转, 速度值: ");
        Serial.println(speedValue - 90);
      } else {
        Serial.println("停止");
      }
    } else {
      Serial.println("请输入0-180之间的数值");
    }
  }
}
```



------

## 五、传感器控制360度舵机实验

### 实验1：霍尔传感器控制方向切换

**材料**：霍尔传感器模块（如A3144）、磁铁

**接线**：

- 霍尔传感器VCC → Arduino 5V
- 霍尔传感器GND → Arduino GND
- 霍尔传感器OUT → Arduino 引脚2

**工作原理**：当磁铁靠近时顺时针旋转，远离时逆时针旋转。

cpp

```
#include <Servo.h>

Servo myServo;
int hallPin = 2;      // 霍尔传感器引脚
int hallState;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  pinMode(hallPin, INPUT_PULLUP);  // 启用内部上拉
  myServo.write(90);  // 初始停止
}

void loop() {
  hallState = digitalRead(hallPin);  // LOW表示有磁场
  
  if (hallState == LOW) {
    // 磁铁靠近：顺时针旋转
    myServo.write(30);  // 较快顺时针
    Serial.println("磁铁靠近 - 顺时针旋转");
  } else {
    // 磁铁远离：逆时针旋转
    myServo.write(150); // 较快逆时针
    Serial.println("磁铁远离 - 逆时针旋转");
  }
  delay(50);
}
```



### 实验2：超声波传感器控制速度

**材料**：HC-SR04超声波传感器

**接线**：

- HC-SR04 VCC → Arduino 5V
- HC-SR04 GND → Arduino GND
- HC-SR04 Trig → Arduino 引脚7
- HC-SR04 Echo → Arduino 引脚6

**工作原理**：距离越近，旋转速度越快（顺时针）；距离越远，旋转速度越慢，超过阈值则停止。

cpp

```
#include <Servo.h>

Servo myServo;
const int trigPin = 7;
const int echoPin = 6;

long duration;
int distance;
int speedValue;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // 测量距离
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  // 限制距离范围（5cm - 50cm）
  if (distance < 5) distance = 5;
  if (distance > 50) distance = 50;
  
  // 映射：距离越近，速度越快（顺时针）
  // 5cm -> 0 (最快顺时针)
  // 50cm -> 90 (停止)
  speedValue = map(distance, 5, 50, 0, 90);
  
  myServo.write(speedValue);
  
  Serial.print("距离: ");
  Serial.print(distance);
  Serial.print(" cm, 速度值: ");
  Serial.print(speedValue);
  Serial.print(" - ");
  
  if (speedValue < 90) {
    Serial.print("顺时针, 速度: ");
    Serial.println(90 - speedValue);
  } else {
    Serial.println("停止");
  }
  
  delay(100);
}
```



### 实验3：触摸模块控制多档速度

**材料**：触摸传感器模块（TTP223）

**接线**：

- 触摸模块VCC → Arduino 5V
- 触摸模块GND → Arduino GND
- 触摸模块OUT → Arduino 引脚3

**工作原理**：每次触摸切换一档速度（停止 → 慢速顺时针 → 快速顺时针 → 慢速逆时针 → 快速逆时针 → 停止...）

cpp

```
#include <Servo.h>

Servo myServo;
int touchPin = 3;
int touchState;
int lastTouchState = HIGH;
int mode = 0;  // 0:停止, 1:慢顺, 2:快顺, 3:慢逆, 4:快逆

// 预定义速度值
int speeds[] = {90, 70, 30, 110, 150};
String modeNames[] = {"停止", "慢速顺时针", "快速顺时针", "慢速逆时针", "快速逆时针"};

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  pinMode(touchPin, INPUT);
  myServo.write(speeds[0]);  // 初始停止
  Serial.println("触摸切换速度模式");
}

void loop() {
  touchState = digitalRead(touchPin);
  
  // 检测触摸（上升沿）
  if (touchState == HIGH && lastTouchState == LOW) {
    // 切换到下一个模式
    mode = (mode + 1) % 5;  // 0-4循环
    
    myServo.write(speeds[mode]);
    
    Serial.print("模式 ");
    Serial.print(mode);
    Serial.print(": ");
    Serial.println(modeNames[mode]);
    
    delay(200);  // 防抖
  }
  
  lastTouchState = touchState;
  delay(10);
}
```



### 实验4：两个360度舵机协同工作（差速驱动）

**接线**：

- 舵机1信号线 → 引脚9
- 舵机2信号线 → 引脚10
- 两个舵机的电源和地线并联到Arduino的5V和GND（注意电流限制，建议使用外部电源）

**工作原理**：实现类似小车的运动：前进、后退、左转、右转、停止。

cpp

```
#include <Servo.h>

Servo leftMotor;   // 左侧舵机（模拟左轮）
Servo rightMotor;  // 右侧舵机（模拟右轮）

void setup() {
  leftMotor.attach(9);
  rightMotor.attach(10);
  
  // 初始停止
  leftMotor.write(90);
  rightMotor.write(90);
  delay(1000);
}

void loop() {
  // 前进：两个舵机都顺时针旋转
  leftMotor.write(70);   // 顺时针
  rightMotor.write(70);  // 顺时针
  delay(2000);
  
  // 后退：两个舵机都逆时针旋转
  leftMotor.write(110);  // 逆时针
  rightMotor.write(110); // 逆时针
  delay(2000);
  
  // 左转：左侧停止，右侧顺时针
  leftMotor.write(90);   // 停止
  rightMotor.write(70);  // 顺时针
  delay(2000);
  
  // 右转：右侧停止，左侧顺时针
  leftMotor.write(70);   // 顺时针
  rightMotor.write(90);  // 停止
  delay(2000);
  
  // 旋转：左右反向旋转
  leftMotor.write(70);   // 顺时针
  rightMotor.write(110); // 逆时针
  delay(2000);
  
  // 停止
  leftMotor.write(90);
  rightMotor.write(90);
  delay(2000);
}
```



------

## 六、360度舵机的高级控制技巧

### 技巧1：校准中点（停止点）

由于制造公差，不同舵机的停止点可能略有差异。可以通过以下程序测试并找到准确的停止值。

cpp

```
#include <Servo.h>

Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  Serial.println("360度舵机中点校准程序");
  Serial.println("请输入测试值(80-100):");
}

void loop() {
  if (Serial.available() > 0) {
    int testValue = Serial.parseInt();
    
    if (testValue >= 80 && testValue <= 100) {
      myServo.write(testValue);
      Serial.print("测试值: ");
      Serial.println(testValue);
      Serial.println("舵机是否停止？(输入下一个值继续测试)");
    } else {
      Serial.println("请输入80-100之间的数值");
    }
  }
}
```



通过反复测试，找到能让舵机完全停止的值，记为`STOP_VALUE`。

### 技巧2：平滑速度变化（加减速）

直接跳变速度可能会导致电流冲击，可以通过循环实现平滑加速/减速。

cpp

```
#include <Servo.h>

Servo myServo;

void smoothChange(int targetSpeed, int stepDelay) {
  int currentSpeed = myServo.read();  // 读取当前速度
  
  if (currentSpeed < targetSpeed) {
    for (int i = currentSpeed; i <= targetSpeed; i++) {
      myServo.write(i);
      delay(stepDelay);
    }
  } else if (currentSpeed > targetSpeed) {
    for (int i = currentSpeed; i >= targetSpeed; i--) {
      myServo.write(i);
      delay(stepDelay);
    }
  }
}

void setup() {
  myServo.attach(9);
  myServo.write(90);  // 停止
  delay(1000);
}

void loop() {
  // 平滑加速到顺时针最大速度
  smoothChange(20, 20);
  delay(2000);
  
  // 平滑减速到停止
  smoothChange(90, 20);
  delay(2000);
  
  // 平滑加速到逆时针最大速度
  smoothChange(160, 20);
  delay(2000);
  
  // 平滑减速到停止
  smoothChange(90, 20);
  delay(2000);
}
```



------

## 七、常见问题及解决方法

### Q1：舵机不转动

- **检查电源**：确认红线连接到5V，棕线连接到GND
- **检查信号线**：确认橙线连接到正确的数字引脚
- **检查代码**：确认引脚号与程序一致
- **检查速度值**：如果设置90，舵机应该停止；设置其他值才转动

### Q2：舵机应该停止时却缓慢转动

- **中点偏移**：舵机的停止点可能不是精确的90°，需要校准
- **尝试调整**：改为91或89，找到真正的停止值

### Q3：舵机抖动或不平稳

- **供电不足**：尝试使用外部电源供电（特别是驱动多个舵机时）
- **PWM信号不稳定**：检查连线是否牢固
- **机械负载过大**：减少负载或使用更大扭矩的舵机

### Q4：舵机只能一个方向转动

- **检查代码逻辑**：确保写入的值覆盖了0-180范围
- **舵机损坏**：可能是内部驱动电路故障

### Q5：Arduino重启或舵机异常

- **电源过载**：单个Arduino 5V引脚最大电流约500mA，一个舵机工作电流可达250mA，驱动两个以上建议使用外部电源

------

## 八、360度舵机应用项目拓展

### 项目1：简易遥控小车

- 使用两个360度舵机作为左右轮驱动
- 配合蓝牙模块（HC-05/HC-06）实现手机遥控
- 实现前进、后退、左转、右转、旋转等功能

### 项目2：自动巡线机器人

- 使用2-3个红外巡线传感器
- 根据传感器状态调整左右舵机速度，实现沿黑线行驶

### 项目3：雷达扫描仪

- 一个180度舵机控制超声波传感器上下扫描
- 一个360度舵机控制整个云台连续旋转
- 结合Processing绘制雷达图

### 项目4：自动喂食器

- 使用360度舵机驱动螺旋送料机构
- 配合RTC时钟模块，定时旋转一定时间送出定量食物

------

## 九、注意事项

1. **电源管理**：360度舵机工作时电流较大，驱动多个舵机时务必使用外部电源
2. **机械安装**：确保舵机安装牢固，避免运行时晃动
3. **负载匹配**：不要超过舵机的扭矩极限，特别是长时间运行的项目
4. **散热考虑**：长时间高速旋转时，舵机会发热，注意通风散热
5. **程序中位校准**：实际项目中，建议先通过校准程序找到准确的停止值

------

## 十、总结

360度连续旋转舵机虽然外观和控制接口与180度舵机相似，但**控制逻辑完全不同**：

| 对比维度 | 180度舵机     | 360度舵机       |
| :------- | :------------ | :-------------- |
| 控制目标 | 角度位置      | 速度+方向       |
| 典型应用 | 定位控制      | 运动控制        |
| 程序重点 | `write(角度)` | `write(速度值)` |
| 90度含义 | 中间位置      | 停止            |

掌握360度舵机的控制方法后，你就可以用它来制作各种运动机构——从简单的旋转平台到复杂的差速驱动小车。结合各种传感器，可以创造出丰富的互动项目。

**下一步学习建议**：

1. 尝试制作一个两轮差速驱动小车
2. 学习使用L298N电机驱动板驱动更大功率的电机
3. 探索PID控制算法，实现更精确的速度控制
4. 结合无线模块（蓝牙/WiFi）实现远程控制

祝你在创客之路上不断进步！如有疑问，欢迎在Arduino社区交流讨论。