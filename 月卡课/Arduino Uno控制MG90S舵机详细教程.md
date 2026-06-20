# Arduino Uno控制MG90S舵机详细教程

## 一、材料准备

1. Arduino Uno开发板 ×1
2. MG90S舵机 ×1
3. 杜邦线（公对公） ×3
4. USB数据线（Type-B） ×1
5. 电脑（已安装Arduino IDE）

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

## 三、硬件连接

### 接线步骤：

1. 将舵机红线连接到Arduino的 **5V** 引脚
2. 将舵机棕线连接到Arduino的 **GND** 引脚
3. 将舵机橙线连接到Arduino的 **数字引脚9**

**安全提示**：

- 避免舵机长时间堵转（遇到阻力仍尝试转动）
- 如需要驱动多个舵机，建议使用外部电源

## 四、软件环境搭建

### 安装Arduino IDE：

1. 访问 https://www.arduino.cc/en/software
2. 下载对应操作系统的Arduino IDE
3. 安装并打开软件

### 驱动安装（Windows）：

- 首次连接Arduino时，系统可能会自动安装驱动
- 如未自动安装，可从设备管理器手动安装

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



## 六、进阶实验

### 实验1：电位器控制舵机

**额外材料**：

- 10kΩ电位器 ×1
- 面包板 ×1
- 杜邦线 ×若干

**接线**：

1. 电位器中间引脚 → Arduino A0
2. 电位器两侧引脚 → Arduino 5V和GND

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



## 七、常见问题及解决方法

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

## 八、项目拓展建议

1. **机械臂制作**：使用3-4个舵机制作简易机械臂
2. **摄像头云台**：两个舵机实现上下左右转动
3. **智能小车转向**：舵机控制小车前轮方向
4. **太阳跟踪系统**：根据光线传感器控制舵机转动

## 九、注意事项

1. **电源保护**：避免将舵机直接连接到Arduino的5V引脚超过2个
2. **机械保护**：避免用手强行转动舵机
3. **程序保护**：不要发送超过180度的角度值
4. **接线安全**：通电前仔细检查接线，避免短路

## 十、学习资源推荐

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

**下一步学习建议**：
掌握舵机基础后，可以尝试：

1. 学习使用舵机驱动板控制多个舵机
2. 结合超声波传感器制作避障系统
3. 使用蓝牙模块实现手机遥控舵机

祝你学习顺利！如有问题，欢迎在Arduino社区提问交流。