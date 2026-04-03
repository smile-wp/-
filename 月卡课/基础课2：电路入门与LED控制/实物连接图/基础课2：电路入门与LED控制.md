## 基础课2：电路入门与点亮LED

### 🎯 今日学习目标

1. 认识基本电子元件和面包板结构
2. 学会安全连接简单电路
3. 掌握LED的控制原理
4. 学会使用电阻保护LED

### ⏰ 课程时间分配（90分钟）

#### **第一部分：课程导入（10分钟）**

- **复习回顾（5分钟）**：提问上节课内容
- **实物展示（5分钟）**：展示各种电子元件，让学生触摸感受

#### **第二部分：理论知识讲解（20分钟）**

**1. 电子元件基础（10分钟）**

text

```
电子元件家族：
├── 电源类
│   ├── 电池
│   └── 电源模块
├── 控制类
│   ├── Arduino
│   └── 开关
├── 输出类
│   ├── LED（发光二极管）
│   ├── 蜂鸣器
│   └── 电机
├── 输入类
│   ├── 按钮
│   ├── 传感器
│   └── 电位器
└── 辅助类
    ├── 电阻
    ├── 电容
    └── 面包板
```



**2. 面包板结构详解（5分钟）**

![image-20251218145333096](C:\Users\13092\AppData\Roaming\Typora\typora-user-images\image-20251218145333096.png)



**3. 电路基础概念（5分钟）**

- 电压、电流、电阻
- 欧姆定律：V = I × R
- LED工作电压：通常2-3V
- 限流电阻计算：R = (电源电压 - LED电压) / 工作电流

#### **第三部分：动手实践（40分钟）**

**安全须知（5分钟）**

- 断电接线
- 检查极性
- 使用合适电阻
- 不要短路电源

**实践1：搭建第一个外部LED电路（15分钟）**

**电路连接步骤：**

1. Arduino GND → 面包板负极行

2. Arduino引脚8 → 面包板某列

3. 220Ω电阻一脚接引脚8线，另一脚接新行

4. LED长脚（正极）接电阻，短脚（负极）接GND行

   ![](D:\交互资料代码\月卡课\2.jpg)

   ![1](D:\交互资料代码\月卡课\1.jpg)

**验证代码：**

![image-20251222171413026](C:\Users\13092\AppData\Roaming\Typora\typora-user-images\image-20251222171413026.png)

cpp

```
int ledPin = 8;  // 定义LED连接的引脚

void setup() {
  pinMode(ledPin, OUTPUT);  // 设置引脚为输出模式
  Serial.begin(9600);
  Serial.println("LED电路测试开始");
}

void loop() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED亮起");
  delay(500);
  
  digitalWrite(ledPin, LOW);
  Serial.println("LED熄灭");
  delay(500);
}
```



**实践2：多LED控制（15分钟）**

**电路连接：**

text

```
LED1：引脚8 → 电阻 → LED正 → LED负 → GND
LED2：引脚9 → 电阻 → LED正 → LED负 → GND
LED3：引脚10 → 电阻 → LED正 → LED负 → GND
```



**控制代码：**

cpp

```
/*
 * 项目：交通信号灯模拟
 * 红灯亮3秒，黄灯亮1秒，绿灯亮3秒
 */

// 定义引脚
int redPin = 8;
int yellowPin = 9;
int greenPin = 10;

void setup() {
  // 初始化所有引脚
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("交通信号灯模拟开始");
}

void loop() {
  // 红灯亮
  digitalWrite(redPin, HIGH);
  Serial.println("红灯亮 - 停车");
  delay(3000);
  
  // 红灯灭，黄灯亮
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, HIGH);
  Serial.println("黄灯亮 - 准备");
  delay(1000);
  
  // 黄灯灭，绿灯亮
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, HIGH);
  Serial.println("绿灯亮 - 通行");
  delay(3000);
  
  // 绿灯灭，黄灯亮
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, HIGH);
  Serial.println("黄灯亮 - 减速");
  delay(1000);
  
  digitalWrite(yellowPin, LOW);
}
```



**🔍 代码知识点详解：**

- **变量定义**：`int ledPin = 8;` 创建整数类型变量
- **引脚初始化**：需要为每个使用的引脚设置模式
- **顺序执行**：代码从上到下顺序执行
- **注释使用**：`//`单行注释，`/* */`多行注释

**实践3：故障排除练习（5分钟）**

- 故意制造常见错误让学生解决：
  1. LED反接
  2. 忘记接电阻
  3. 引脚号写错
  4. 面包板连接错误

#### **第四部分：知识总结与扩展（15分钟）**

**知识总结（5分钟）**

- 电子元件识别
- 面包板使用技巧
- 电路连接规范
- 基础程序结构

**扩展活动：创意灯光秀（10分钟）**

cpp

```
/*
 * 扩展：生日蜡烛效果
 * 模拟蜡烛被吹灭的效果
 */

int ledPin = 8;
int buttonPin = 2;  // 添加一个按钮模拟吹气

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // 使用内部上拉电阻
  
  // 初始状态：蜡烛亮着
  digitalWrite(ledPin, HIGH);
  Serial.println("蜡烛已点燃");
}

void loop() {
  // 检查是否被"吹灭"（按钮按下）
  if(digitalRead(buttonPin) == LOW) {
    Serial.println("检测到吹气！");
    
    // 蜡烛闪烁几次然后熄灭
    for(int i = 0; i < 3; i++) {
      digitalWrite(ledPin, LOW);
      delay(100);
      digitalWrite(ledPin, HIGH);
      delay(100);
    }
    
    // 最终熄灭
    digitalWrite(ledPin, LOW);
    Serial.println("蜡烛已熄灭");
    
    // 等待重新点燃
    delay(3000);
    digitalWrite(ledPin, HIGH);
    Serial.println("蜡烛重新点燃");
  }
  
  // 蜡烛随机轻微闪烁（模拟火焰）
  int flicker = random(5);  // 生成0-4的随机数
  if(flicker == 0) {
    analogWrite(ledPin, 200);  // 稍暗
    delay(50);
    analogWrite(ledPin, 255);  // 恢复亮度
  }
  
  delay(100);
}
```



**🔍 新知识点：**

- `digitalRead(pin)`：读取数字引脚状态
- `INPUT_PULLUP`：使用内部上拉电阻
- `random(max)`：生成随机数
- `analogWrite(pin, value)`：PWM输出（需要PWM引脚）

**家庭作业：**

1. 设计一个十字路口的交通灯系统（4个方向）
2. 研究不同阻值电阻对LED亮度的影响