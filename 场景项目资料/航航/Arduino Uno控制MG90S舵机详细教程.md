# Arduino Uno控制MG90S舵机(180)详细教程

------

## 一、材料准备

- Arduino Uno开发板 ×1
- MG90S舵机 ×1
- 杜邦线（公对公） ×3
- USB数据线（Type-B） ×1
- 电脑（已安装Arduino IDE）

------

## 二、舵机基础知识

### MG90S舵机参数：

- 工作电压：4.8V-6V（建议使用5V）
- 工作电流：100-250mA
- 扭矩：1.8-2.2kg·cm
- 旋转角度：0-180度
- 重量：13.4g
- 接口：三线制（信号、电源、地线）

### 舵机线序：

- **橙色/黄色线**：信号线（PWM信号）
- **红色线**：电源正极（VCC）
- **棕色/黑色线**：电源负极（GND）

------

## 2.1 180度舵机与360度舵机的区别

| 特性            | 180度舵机（标准舵机）                      | 360度舵机（连续旋转舵机）                                    |
| :-------------- | :----------------------------------------- | :----------------------------------------------------------- |
| **控制方式**    | 通过PWM信号控制角度位置                    | 通过PWM信号控制旋转方向和速度                                |
| **转动范围**    | 0°～180°（角度控制）                       | 连续360°旋转（方向+速度控制）                                |
| **内部结构**    | 有位置反馈电位器，可闭环控制角度           | 无位置反馈，通常移除限位，可无限旋转                         |
| **应用场景**    | 机械臂、云台、转向机构等需要精确定位的场合 | 小车驱动、传送带、旋转平台等需要连续旋转的场合               |
| **Arduino控制** | `myservo.write(angle)` 设置角度            | `myservo.write(speed)` 设置速度（90为停止，0为全速正转，180为全速反转） |
| **能否互换**    | 不能直接互换，硬件不同                     | 不能直接互换，硬件不同                                       |

**注意**：MG90S是180度舵机，本教程主要针对它。如果使用360度舵机，控制方式会有差异（见上表）。

------

## 三、硬件连接

### 接线步骤：

1. 将舵机红线连接到Arduino的 **5V** 引脚
2. 将舵机棕线连接到Arduino的 **GND** 引脚
3. 将舵机橙线连接到Arduino的 **数字引脚9**

**安全提示**：

- 避免舵机长时间堵转（遇到阻力仍尝试转动）
- 如需要驱动多个舵机，建议使用外部电源

------

## 四、软件环境搭建

### 安装Arduino IDE：

1. 访问 https://www.arduino.cc/en/software
2. 下载对应操作系统的Arduino IDE
3. 安装并打开软件

### 驱动安装（Windows）：

- 首次连接Arduino时，系统可能会自动安装驱动
- 如未自动安装，可从设备管理器手动安装

------

## 五、基础控制程序

### 程序1：舵机基础扫动

cpp

```
#include <Servo.h>  // 包含舵机库

Servo myservo;  // 创建舵机对象
int pos = 0;    // 存储舵机位置的变量

void setup() {
  myservo.attach(9);  // 将舵机连接到数字引脚9
}

void loop() {
  // 从0度到180度缓慢转动
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);  // 设置舵机位置
    delay(15);           // 等待15ms让舵机转动到指定位置
  }
  
  // 从180度回到0度
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
}
```



### 程序2：舵机精确位置控制

cpp

```
#include <Servo.h>

Servo myservo;

void setup() {
  Serial.begin(9600);   // 初始化串口通信
  myservo.attach(9);    // 连接舵机到引脚9
  
  Serial.println("舵机控制程序已启动");
  Serial.println("输入角度值 (0-180):");
}

void loop() {
  // 通过串口输入控制舵机
  if (Serial.available() > 0) {
    int angle = Serial.parseInt();  // 读取串口输入的整数
    
    if (angle >= 0 && angle <= 180) {
      myservo.write(angle);  // 转动到指定角度
      Serial.print("舵机已转动到: ");
      Serial.print(angle);
      Serial.println(" 度");
    } else {
      Serial.println("请输入0-180之间的角度值");
    }
  }
}
```



### 程序3：舵机多位置预设

cpp

```
#include <Servo.h>

Servo myservo;

// 定义几个常用位置
const int positions[] = {0, 45, 90, 135, 180};
const int numPositions = 5;
int currentPosition = 0;

void setup() {
  myservo.attach(9);
  myservo.write(90);  // 初始位置设为90度
  delay(1000);        // 等待1秒
}

void loop() {
  // 循环遍历所有预设位置
  for (int i = 0; i < numPositions; i++) {
    myservo.write(positions[i]);  // 转动到预设位置
    delay(1000);                  // 保持1秒
  }
}
```



------

## 六、进阶实验

### 实验1：电位器控制舵机

**额外材料**：

- 10kΩ电位器 ×1
- 面包板 ×1
- 杜邦线 ×若干

**接线**：

- 电位器中间引脚 → Arduino A0
- 电位器两侧引脚 → Arduino 5V和GND

**程序代码**：

cpp

```
#include <Servo.h>

Servo myservo;
int potPin = A0;    // 电位器连接模拟引脚A0
int potValue;       // 存储电位器读数
int angle;          // 转换后的角度值

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(potPin);          // 读取电位器值 (0-1023)
  angle = map(potValue, 0, 1023, 0, 180); // 映射到0-180度
  myservo.write(angle);                   // 设置舵机角度
  
  Serial.print("电位器值: ");
  Serial.print(potValue);
  Serial.print(" 角度: ");
  Serial.println(angle);
  
  delay(20);  // 短暂延迟，稳定读数
}
```



### 实验2：两个舵机协同工作

**接线**：

- 第二个舵机信号线连接到数字引脚10

**程序代码**：

cpp

```
#include <Servo.h>

Servo servo1;
Servo servo2;

void setup() {
  servo1.attach(9);   // 第一个舵机
  servo2.attach(10);  // 第二个舵机
}

void loop() {
  // 两个舵机同步运动
  for (int i = 0; i <= 180; i++) {
    servo1.write(i);
    servo2.write(180 - i);  // 反向运动
    delay(10);
  }
  
  for (int i = 180; i >= 0; i--) {
    servo1.write(i);
    servo2.write(180 - i);
    delay(10);
  }
}
```



------

## 七、传感器控制舵机实验

### 实验3：霍尔传感器控制舵机

**霍尔传感器**可以检测磁场变化。这里使用**霍尔开关模块（如A3144）**，当磁铁靠近时输出低电平，否则输出高电平。我们可以用它来控制舵机在两个预设角度之间切换。

**额外材料**：

- 霍尔传感器模块 ×1
- 磁铁 ×1
- 面包板 ×1
- 杜邦线 ×若干

**接线**：

- 霍尔传感器VCC → Arduino 5V
- 霍尔传感器GND → Arduino GND
- 霍尔传感器OUT → Arduino 数字引脚2

**工作原理**：

- 当磁铁靠近传感器时，数字引脚2读取到LOW，舵机转到180度；
- 磁铁离开时，引脚2为HIGH，舵机回到0度。

**程序代码**：

cpp

```c++
#include <Servo.h>

Servo myservo;
int hallPin = 2;      // 霍尔传感器输出引脚
int hallState;        // 存储传感器状态

void setup() {
  myservo.attach(9);
  pinMode(hallPin, INPUT_PULLUP); // 启用内部上拉电阻，使信号更稳定
  myservo.write(0);    // 初始位置0度
  delay(500);
}

void loop() {
  hallState = digitalRead(hallPin); // 读取传感器状态（LOW表示有磁场）
  
  if (hallState == LOW) {  // 磁铁靠近
    myservo.write(180);     // 转到180度
  } else {                  // 磁铁远离
    myservo.write(0);       // 回到0度
  }
  delay(50); // 防抖
}
```



**拓展**：可以通过磁铁的位置来控制舵机角度连续变化（例如使用线性霍尔传感器，输出模拟量），但需要模拟输入引脚。

------

### 实验4：超声波传感器控制舵机

**HC-SR04超声波传感器**可以测量距离。我们可以根据距离来控制舵机角度：距离越近，角度越大（或越小）。

**额外材料**：

- HC-SR04超声波传感器 ×1
- 面包板 ×1
- 杜邦线 ×若干

**接线**：

- HC-SR04 VCC → Arduino 5V
- HC-SR04 GND → Arduino GND
- HC-SR04 Trig → Arduino 数字引脚7
- HC-SR04 Echo → Arduino 数字引脚6

**工作原理**：

- 通过Trig触发测距，Echo接收回波，计算距离。
- 将距离（范围假设为2cm~40cm）映射到0~180度，舵机相应转动。

**程序代码**：

cpp

```
#include <Servo.h>

Servo myservo;

const int trigPin = 7;
const int echoPin = 6;

long duration;
int distance;
int angle;

void setup() {
  myservo.attach(9);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // 发送超声波脉冲
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // 读取回波时间
  duration = pulseIn(echoPin, HIGH);
  
  // 计算距离（单位：厘米）
  distance = duration * 0.034 / 2;
  
  // 限制有效测量范围（2~40cm）
  if (distance < 2) distance = 2;
  if (distance > 40) distance = 40;
  
  // 将距离映射到角度（距离越近，角度越大）
  angle = map(distance, 2, 40, 180, 0);
  
  myservo.write(angle);
  
  Serial.print("距离: ");
  Serial.print(distance);
  Serial.print(" cm, 角度: ");
  Serial.println(angle);
  
  delay(100);
}
```



**注意**：可以根据实际应用调整映射关系（例如距离近时角度小，只需交换map的上下限）。

------

### 实验5：触摸模块控制舵机

**触摸传感器模块（如TTP223）**可以检测人体触摸，输出数字信号（高/低）。我们可以用触摸来切换舵机的多个位置。

**额外材料**：

- 触摸传感器模块 ×1
- 面包板 ×1
- 杜邦线 ×若干

**接线**：

- 触摸模块VCC → Arduino 5V
- 触摸模块GND → Arduino GND
- 触摸模块OUT → Arduino 数字引脚3

**工作原理**：

- 每次触摸，舵机角度变化一次（例如从0°→90°→180°→0°循环）。
- 可以通过状态机实现多位置切换。

**程序代码**：

cpp

```
#include <Servo.h>

Servo myservo;
int touchPin = 3;
int touchState;
int lastTouchState = HIGH;  // 记录上一次触摸状态
int angleIndex = 0;         // 当前角度索引
int angles[] = {0, 90, 180}; // 预设角度

void setup() {
  myservo.attach(9);
  pinMode(touchPin, INPUT);
  myservo.write(angles[0]);
  Serial.begin(9600);
}

void loop() {
  touchState = digitalRead(touchPin);
  
  // 检测触摸的上升沿（从低到高，因为模块通常触摸输出高电平）
  if (touchState == HIGH && lastTouchState == LOW) {
    // 切换到下一个角度
    angleIndex = (angleIndex + 1) % 3;  // 0,1,2循环
    myservo.write(angles[angleIndex]);
    
    Serial.print("触摸！当前角度: ");
    Serial.println(angles[angleIndex]);
    
    delay(200); // 简单防抖
  }
  
  lastTouchState = touchState;
  delay(10);
}
```



**注意**：不同触摸模块输出电平可能不同，如果模块是触摸输出低电平，需要调整判断条件。

------

## 八、常见问题及解决方法

### Q1：舵机不转动

- **检查电源**：确认红线连接到5V，棕线连接到GND
- **检查信号线**：确认橙线连接到正确的数字引脚
- **检查代码**：确认引脚号与程序一致

### Q2：舵机抖动

- **供电不足**：尝试使用外部电源供电
- **程序延迟不当**：调整delay()时间

### Q3：舵机只能转动一定角度

- **检查角度范围**：MG90S只能转动0-180度
- **检查接线**：确认所有连接牢固

### Q4：Arduino IDE报错

- **确认库文件**：Servo库是Arduino内置库，无需额外安装
- **检查板卡选择**：Tools → Board → Arduino Uno
- **检查端口选择**：Tools → Port → 选择正确的COM口

### Q5：传感器读数不稳定

- **检查接线**：确保电源和地线连接牢固
- **添加滤波**：在程序中增加简单的平均值滤波

------

## 九、项目拓展建议

1. **机械臂制作**：使用3-4个舵机制作简易机械臂
2. **摄像头云台**：两个舵机实现上下左右转动
3. **智能小车转向**：舵机控制小车前轮方向
4. **太阳跟踪系统**：根据光线传感器控制舵机转动
5. **智能门锁**：结合触摸传感器和舵机实现触摸开门
6. **自动感应垃圾桶**：用超声波传感器检测人手靠近，舵机打开盖子

------

## 十、注意事项

1. **电源保护**：避免将舵机直接连接到Arduino的5V引脚超过2个
2. **机械保护**：避免用手强行转动舵机
3. **程序保护**：不要发送超过180度的角度值
4. **接线安全**：通电前仔细检查接线，避免短路

------

## 十一、学习资源推荐

1. **官方文档**：
   - Arduino官网：[www.arduino.cc](https://www.arduino.cc/)
   - Servo库文档：[www.arduino.cc/reference/en/libraries/servo/](https://www.arduino.cc/reference/en/libraries/servo/)
2. **社区支持**：
   - Arduino中文社区：[www.arduino.cn](https://www.arduino.cn/)
   - GitHub相关项目
3. **进阶学习**：
   - 学习PWM（脉冲宽度调制）原理
   - 了解舵机内部结构和工作原理
   - 尝试使用外部电源和电容滤波

------

## 下一步学习建议

掌握舵机基础后，可以尝试：

1. 学习使用舵机驱动板控制多个舵机
2. 结合蓝牙模块实现手机遥控舵机
3. 将多个传感器融合，制作更智能的交互项目
4. 探索360度舵机的速度控制，用于制作小车

祝你学习顺利！如有问题，欢迎在Arduino社区提问交流。