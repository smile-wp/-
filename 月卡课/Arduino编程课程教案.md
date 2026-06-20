# Arduino编程课程教案（每节90分钟）

## 课程结构设计

每节课包含：

1. 课前准备（5分钟）
2. 课程导入（10分钟）
3. 理论知识讲解（20分钟）
4. 动手实践（40分钟）
5. 知识总结与扩展（15分钟）

------

## 基础课1：认识Arduino Uno板和IDE软件

### 🎯 今日学习目标

1. 认识Arduino Uno板的主要部件
2. 学会安装和配置Arduino IDE软件
3. 完成第一个程序上传并理解程序结构
4. 学会使用串口监视器调试程序

### ⏰ 课程时间分配（90分钟）

#### **第一部分：课程导入（10分钟）**

- **破冰活动（5分钟）**：展示有趣的Arduino项目视频（机械臂、智能小车、光立方）
- **提出问题（5分钟）**：
  - "计算机怎么控制硬件？"
  - "Arduino是什么？能做什么？"
  - "你们想用Arduino创造什么？"

#### **第二部分：理论知识讲解（20分钟）**

**1. Arduino是什么？（5分钟）**

- 开源电子原型平台
- 硬件（各种板子）+ 软件（IDE）+ 社区
- 应用领域：机器人、智能家居、艺术装置等

**2. Arduino Uno板详细讲解（10分钟）**

![deepseek_mermaid_20251218_b9664a](D:\下载\deepseek_mermaid_20251218_b9664a.png)



**3. Arduino IDE安装演示（5分钟）**

- 下载、安装、基本界面介绍
- 工具栏功能：验证、上传、新建、打开、保存

#### **第三部分：动手实践（40分钟）**

**实践1：环境搭建（15分钟）**

1. 连接Arduino到电脑
2. 安装驱动程序（如果需要）
3. 选择开发板和端口
4. 测试连接

**实践2：第一个程序Blink（15分钟）**

cpp

```
/*
 * 项目：眨眼LED
 * 功能：让板载LED每秒闪烁一次
 * 知识点：setup()、loop()、pinMode()、digitalWrite()、delay()
 */

void setup() {
  // 初始化代码，只运行一次
  pinMode(13, OUTPUT);  // 设置13号引脚为输出模式
  Serial.begin(9600);   // 初始化串口通信，波特率9600
  Serial.println("Arduino已启动！");  // 向串口发送消息
}

void loop() {
  // 主循环代码，重复运行
  digitalWrite(13, HIGH);   // 输出高电平，LED亮
  Serial.println("LED亮");   // 串口输出状态
  delay(1000);               // 延时1000毫秒（1秒）
  
  digitalWrite(13, LOW);    // 输出低电平，LED灭
  Serial.println("LED灭");
  delay(1000);
}
```



**🔍 代码知识点详解：**

- `setup()`：程序启动时运行一次，用于初始化
- `loop()`：程序主循环，重复执行
- `pinMode(pin, mode)`：设置引脚模式（INPUT/OUTPUT）
- `digitalWrite(pin, value)`：输出数字信号（HIGH/LOW）
- `delay(ms)`：延时函数，单位毫秒
- `Serial.begin(rate)`：初始化串口通信
- `Serial.println(text)`：向串口发送数据并换行

**实践3：串口监视器使用（10分钟）**

1. 打开串口监视器（工具→串口监视器或Ctrl+Shift+M）
2. 观察程序输出的信息
3. 修改delay时间，观察变化
4. 尝试修改串口输出内容

#### **第四部分：知识总结与扩展（15分钟）**

**知识总结（5分钟）**

- Arduino Uno板各部件功能
- IDE基本操作流程
- 程序的基本结构

**扩展活动：创意闪烁（10分钟）**

cpp

```
// 扩展练习：让LED发出SOS求救信号
// SOS摩斯码：... --- ...（三短三长三短）

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  // 三个短闪（代表S）
  for(int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    delay(200);  // 短闪200ms
    digitalWrite(13, LOW);
    delay(200);
  }
  
  delay(300);  // 字母间间隔
  
  // 三个长闪（代表O）
  for(int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    delay(600);  // 长闪600ms
    digitalWrite(13, LOW);
    delay(200);
  }
  
  delay(300);
  
  // 再来三个短闪（代表S）
  for(int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }
  
  delay(2000);  // 消息间长间隔
}
```



**家庭作业：**

1. 画出Arduino Uno板的简图并标注主要部件
2. 设计自己的闪烁模式（心跳、摩斯码名字等）

------

## 基础课2：电路入门与LED控制

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

**验证代码：**

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

------

## LED灯和灯带编程

### 🎯 今日学习目标

1. 理解PWM（脉宽调制）原理
2. 掌握单LED呼吸灯实现
3. 学会控制RGB LED显示不同颜色
4. 掌握WS2812B灯带的基本控制
5. 创建至少5种灯光效果

### ⏰ 课程时间分配（90分钟）

#### **第一部分：课程导入（10分钟）**

- **灯光秀展示（5分钟）**：展示各种LED创意项目
- **原理提问（5分钟）**：
  - "如何让LED逐渐变亮而不是突然亮？"
  - "如何用3个LED显示所有颜色？"
  - "灯带上的LED如何单独控制？"

#### **第二部分：理论知识讲解（20分钟）**

**1. PWM脉宽调制原理（10分钟）**

![deepseek_mermaid_20251218_d93b0a](D:\下载\deepseek_mermaid_20251218_d93b0a.png)



**2. RGB颜色模型（5分钟）**

- 三原色原理：红(Red)、绿(Green)、蓝(Blue)
- 颜色混合：256×256×256 = 16,777,216种颜色
- RGB LED类型：共阴 vs 共阳

**3. WS2812B智能灯带（5分钟）**

- 单线控制
- 每个LED可单独寻址
- 内置芯片，无需额外元件
- 需要FastLED或NeoPixel库

#### **第三部分：动手实践（40分钟）**

**实践1：单LED呼吸灯（10分钟）**

**电路连接：**

- LED正极 → 220Ω电阻 → 引脚9（必须是PWM引脚：3,5,6,9,10,11）
- LED负极 → GND

**代码实现：**

cpp

```
/*
 * 项目：呼吸灯
 * 功能：模拟呼吸效果，LED缓慢变亮再变暗
 * 知识点：PWM、for循环、analogWrite()
 */

int ledPin = 9;  // 必须使用PWM引脚
int brightness = 0;  // 当前亮度值
int fadeAmount = 5;  // 每次变化的亮度值

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("呼吸灯效果开始");
}

void loop() {
  // 输出PWM信号控制亮度
  analogWrite(ledPin, brightness);
  
  // 串口显示当前亮度值
  Serial.print("当前亮度: ");
  Serial.println(brightness);
  
  // 改变亮度值
  brightness = brightness + fadeAmount;
  
  // 反转变化方向
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;  // 改变变化方向
  }
  
  // 控制呼吸速度
  delay(30);
}
```



**🔍 代码知识点详解：**

- `analogWrite(pin, value)`：输出PWM信号，value范围0-255
- `for循环`：`for(初始化; 条件; 增量) { 代码 }`
- `if条件判断`：`if(条件) { 代码 }`
- 变量运算：`brightness = brightness + fadeAmount;`

**实践2：RGB LED调色板（15分钟）**

**电路连接（共阴RGB LED）：**

text

```
RGB LED：
  最长脚（共阴） → GND
  红色引脚 → 220Ω → 引脚9
  绿色引脚 → 220Ω → 引脚10
  蓝色引脚 → 220Ω → 引脚11
```



**代码实现：**

cpp

```
/*
 * 项目：RGB调色板
 * 功能：展示不同颜色，可自定义颜色
 */

// 定义RGB引脚
int redPin = 9;
int greenPin = 10;
int bluePin = 11;

// 自定义颜色函数
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("RGB调色板启动");
}

void loop() {
  // 彩虹颜色序列
  Serial.println("红色");
  setColor(255, 0, 0);  // 红色
  delay(1000);
  
  Serial.println("绿色");
  setColor(0, 255, 0);  // 绿色
  delay(1000);
  
  Serial.println("蓝色");
  setColor(0, 0, 255);  // 蓝色
  delay(1000);
  
  Serial.println("黄色");
  setColor(255, 255, 0);  // 黄色
  delay(1000);
  
  Serial.println("紫色");
  setColor(255, 0, 255);  // 紫色
  delay(1000);
  
  Serial.println("青色");
  setColor(0, 255, 255);  // 青色
  delay(1000);
  
  Serial.println("白色");
  setColor(255, 255, 255);  // 白色
  delay(1000);
  
  Serial.println("彩虹渐变");
  // 彩虹渐变效果
  for(int i = 0; i < 256; i++) {
    setColor(255-i, i, 0);  // 红→黄
    delay(10);
  }
  for(int i = 0; i < 256; i++) {
    setColor(0, 255-i, i);  // 黄→绿
    delay(10);
  }
  for(int i = 0; i < 256; i++) {
    setColor(i, 0, 255-i);  // 绿→紫
    delay(10);
  }
}
```



**实践3：WS2812B灯带控制（15分钟）**

**安装FastLED库：**

1. 工具 → 管理库
2. 搜索"FastLED"
3. 点击安装

**电路连接：**

text

```
WS2812B灯带：
  VCC → 5V（建议外部电源）
  GND → GND（必须与Arduino共地）
  DIN → 引脚6（数据引脚）
```



**代码实现：**

cpp

```
/*
 * 项目：智能灯带控制
 * 需要安装FastLED库
 */

#include <FastLED.h>

// 灯带设置
#define NUM_LEDS 10      // LED数量（根据实际修改）
#define DATA_PIN 6       // 数据引脚
#define BRIGHTNESS 100   // 初始亮度（0-255）

// 创建LED数组
CRGB leds[NUM_LEDS];

void setup() {
  // 初始化灯带
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  Serial.begin(9600);
  Serial.println("智能灯带初始化完成");
}

void loop() {
  // 效果1：彩虹流动
  rainbowFlow();
  
  // 效果2：呼吸效果
  breathingEffect();
  
  // 效果3：流水灯
  runningLights();
  
  // 效果4：颜色擦除
  colorWipe();
  
  // 效果5：烟花效果
  fireworkEffect();
}

// 效果1：彩虹流动
void rainbowFlow() {
  static uint8_t hue = 0;
  Serial.println("效果：彩虹流动");
  
  for(int i = 0; i < 50; i++) {  // 运行50次
    // 填充彩虹颜色
    fill_rainbow(leds, NUM_LEDS, hue, 7);
    FastLED.show();
    hue += 2;  // 改变颜色偏移
    delay(50);
  }
}

// 效果2：呼吸效果
void breathingEffect() {
  Serial.println("效果：呼吸灯");
  
  for(int breath = 0; breath < 256; breath++) {
    fill_solid(leds, NUM_LEDS, CRGB(breath, 0, breath));  // 紫色呼吸
    FastLED.show();
    delay(10);
  }
  
  for(int breath = 255; breath >= 0; breath--) {
    fill_solid(leds, NUM_LEDS, CRGB(breath, 0, breath));
    FastLED.show();
    delay(10);
  }
}

// 效果3：流水灯
void runningLights() {
  Serial.println("效果：流水灯");
  
  // 正向流动
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Black;
  }
  
  // 反向流动
  for(int i = NUM_LEDS-1; i >= 0; i--) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Black;
  }
}

// 效果4：颜色擦除
void colorWipe() {
  Serial.println("效果：颜色擦除");
  
  CRGB colors[] = {CRGB::Red, CRGB::Green, CRGB::Blue};
  
  for(int colorIndex = 0; colorIndex < 3; colorIndex++) {
    // 填充颜色
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = colors[colorIndex];
      FastLED.show();
      delay(100);
    }
    
    // 清除颜色
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(100);
    }
  }
}

// 效果5：烟花效果
void fireworkEffect() {
  Serial.println("效果：烟花");
  
  for(int i = 0; i < 3; i++) {  // 3个烟花
    // 烟花上升
    for(int pos = NUM_LEDS/2; pos >= 0; pos--) {
      leds[pos] = CRGB::White;
      FastLED.show();
      delay(50);
      leds[pos] = CRGB::Black;
    }
    
    // 烟花爆炸
    int center = NUM_LEDS/2;
    for(int radius = 0; radius < min(center, NUM_LEDS-center); radius++) {
      if(center - radius >= 0) leds[center - radius] = CRGB::Red;
      if(center + radius < NUM_LEDS) leds[center + radius] = CRGB::Yellow;
      FastLED.show();
      delay(100);
    }
    
    // 烟花消散
    fadeToBlackBy(leds, NUM_LEDS, 50);
    FastLED.show();
    delay(200);
  }
}
```



**🔍 FastLED库知识点：**

- `#include <FastLED.h>`：包含FastLED库
- `CRGB`：颜色数据类型，如`CRGB::Red`
- `fill_solid(array, count, color)`：填充相同颜色
- `fill_rainbow(array, count, initialHue, deltaHue)`：填充彩虹色
- `FastLED.show()`：更新LED显示
- `fadeToBlackBy(array, count, fadeBy)`：淡出效果

#### **第四部分：知识总结与扩展（15分钟）**

**知识总结（5分钟）**

- PWM原理及应用
- RGB颜色混合原理
- 智能灯带控制方法
- FastLED库基本函数

**扩展活动：音乐可视化灯带（10分钟）**

cpp

```
/*
 * 扩展：音乐节奏灯
 * 模拟根据音乐节奏变化的灯光
 */

#include <FastLED.h>

#define NUM_LEDS 16
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

// 模拟音乐节奏的数组
int beats[] = {1, 2, 3, 2, 1, 4, 3, 2, 1, 5, 4, 3, 2, 1};
int beatCount = 14;
int currentBeat = 0;

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  Serial.begin(9600);
}

void loop() {
  // 获取当前节拍强度
  int intensity = beats[currentBeat];
  
  Serial.print("节拍强度: ");
  Serial.println(intensity);
  
  // 根据强度显示不同效果
  switch(intensity) {
    case 1:  // 弱拍 - 蓝色呼吸
      breathingColor(CRGB::Blue);
      break;
    case 2:  // 中拍 - 绿色波浪
      colorWave(CRGB::Green);
      break;
    case 3:  // 强拍 - 红色脉冲
      colorPulse(CRGB::Red);
      break;
    case 4:  // 超强拍 - 彩虹闪烁
      rainbowFlash();
      break;
    case 5:  // 最强拍 - 全彩爆发
      colorExplosion();
      break;
  }
  
  // 移动到下一个节拍
  currentBeat = (currentBeat + 1) % beatCount;
  delay(500);  // 节拍间隔
}

void breathingColor(CRGB color) {
  for(int i = 0; i < 100; i++) {
    fill_solid(leds, NUM_LEDS, color);
    FastLED.setBrightness(i);
    FastLED.show();
    delay(20);
  }
  for(int i = 100; i > 0; i--) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(20);
  }
}

void colorWave(CRGB color) {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Black;
  }
}

void colorPulse(CRGB color) {
  for(int i = 0; i < 3; i++) {
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
    delay(100);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(100);
  }
}

void rainbowFlash() {
  fill_rainbow(leds, NUM_LEDS, 0, 7);
  FastLED.show();
  delay(200);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void colorExplosion() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(random8(), 255, 255);  // 随机颜色
  }
  FastLED.show();
  delay(300);
  fadeToBlackBy(leds, NUM_LEDS, 255);
  FastLED.show();
}
```



**家庭作业：**

1. 设计一个生日派对灯光场景
2. 研究如何用灯带显示文字或图案
3. 尝试创建自己的灯光效果函数

------

## 舵机控制课程

### 🎯 今日学习目标

1. 了解舵机的工作原理和类型
2. 掌握180度舵机的角度控制
3. 学会控制360度舵机的速度和方向
4. 掌握多舵机协同控制
5. 实现摇杆控制舵机

### ⏰ 课程时间分配（90分钟）

#### **第一部分：课程导入（10分钟）**

- **舵机应用展示（5分钟）**：机器人手臂、机器人小车、相机云台
- **原理提问（5分钟）**：
  - "舵机如何知道转到什么角度？"
  - "180度和360度舵机有什么区别？"
  - "如何控制舵机的速度？"

#### **第二部分：理论知识讲解（20分钟）**

**1. 舵机工作原理（10分钟）**

<img src="D:\下载\deepseek_mermaid_20251218_c6668b.png" alt="deepseek_mermaid_20251218_c6668b" style="zoom: 25%;" />



**2. 舵机类型对比（5分钟）**

text

```
舵机类型对比表：
┌──────────┬────────────┬────────────┬──────────────┐
│ 类型     │ 旋转范围   │ 控制方式   │ 主要应用     │
├──────────┼────────────┼────────────┼──────────────┤
│ 180度舵机│ 0-180度    │ 角度控制   │ 机械臂、云台 │
├──────────┼────────────┼────────────┼──────────────┤
│ 270度舵机│ 0-270度    │ 角度控制   │ 特殊机械结构 │
├──────────┼────────────┼────────────┼──────────────┤
│ 360度舵机│ 连续旋转   │ 速度控制   │ 小车、风扇   │
└──────────┴────────────┴────────────┴──────────────┘
```



**3. PWM控制原理（5分钟）**

- 脉冲宽度调制
- 控制脉冲：周期20ms，宽度0.5-2.5ms
- 角度计算公式：角度 = (脉宽 - 500) × 180 / 2000

#### **第三部分：动手实践（40分钟）**

**实践1：180度舵机基础控制（15分钟）**

**电路连接：**

text

```
180度舵机：
  棕色/黑色线 → GND
  红色线 → 5V
  橙色/黄色线 → 引脚9（信号线）
```



**代码实现：**

cpp

```
/*
 * 项目：180度舵机控制
 * 功能：控制舵机在不同角度间运动
 * 需要安装Servo库（通常已内置）
 */

#include <Servo.h>  // 包含舵机库

Servo myServo;      // 创建舵机对象
int servoPin = 9;   // 舵机信号引脚
int angle = 0;      // 当前角度

void setup() {
  myServo.attach(servoPin);  // 将舵机连接到指定引脚
  Serial.begin(9600);
  Serial.println("180度舵机控制程序启动");
  Serial.println("请输入角度值(0-180):");
}

void loop() {
  // 检查串口是否有输入
  if (Serial.available() > 0) {
    int inputAngle = Serial.parseInt();  // 读取整数输入
    
    // 限制角度在0-180度之间
    if (inputAngle >= 0 && inputAngle <= 180) {
      angle = inputAngle;
      myServo.write(angle);  // 设置舵机角度
      
      Serial.print("舵机转到: ");
      Serial.print(angle);
      Serial.println(" 度");
    } else {
      Serial.println("角度必须在0-180之间");
    }
    
    // 清除串口缓冲区
    while(Serial.available() > 0) {
      Serial.read();
    }
  }
  
  // 演示自动扫描
  static bool autoMode = true;
  static unsigned long lastChange = 0;
  
  if(autoMode && millis() - lastChange > 20) {
    angle = (angle + 1) % 181;  // 0-180循环
    myServo.write(angle);
    lastChange = millis();
    
    if(angle % 30 == 0) {  // 每30度显示一次
      Serial.print("自动扫描: ");
      Serial.print(angle);
      Serial.println(" 度");
    }
  }
}
```



**🔍 Servo库知识点：**

- `#include <Servo.h>`：包含舵机库
- `Servo myServo`：创建舵机对象
- `myServo.attach(pin)`：关联舵机到引脚
- `myServo.write(angle)`：设置舵机角度(0-180)
- `myServo.read()`：读取当前角度
- `Serial.parseInt()`：从串口读取整数

**实践2：360度舵机控制（15分钟）**

**电路连接：** 同180度舵机

**代码实现：**

cpp

```
/*
 * 项目：360度舵机控制
 * 功能：控制舵机速度和方向
 * 注意：360度舵机write(90)停止，其他值控制速度
 */

#include <Servo.h>

Servo myServo;
int servoPin = 9;

// 速度等级：0停止，1-5递增速度，负数为反向
int speedLevel = 0;

void setup() {
  myServo.attach(servoPin);
  Serial.begin(9600);
  
  Serial.println("360度舵机控制程序");
  Serial.println("控制命令:");
  Serial.println("f - 前进");
  Serial.println("b - 后退");
  Serial.println("s - 停止");
  Serial.println("1-5 - 速度等级");
  Serial.println("+ - 加速");
  Serial.println("- - 减速");
}

void loop() {
  if(Serial.available()) {
    char command = Serial.read();
    
    switch(command) {
      case 'f':  // 前进
        speedLevel = min(speedLevel + 1, 5);
        setSpeed();
        break;
        
      case 'b':  // 后退
        speedLevel = max(speedLevel - 1, -5);
        setSpeed();
        break;
        
      case 's':  // 停止
        speedLevel = 0;
        setSpeed();
        break;
        
      case '1': case '2': case '3': case '4': case '5':
        speedLevel = command - '0';  // 字符转数字
        setSpeed();
        break;
        
      case '+':  // 加速
        if(speedLevel > 0) speedLevel = min(speedLevel + 1, 5);
        else if(speedLevel < 0) speedLevel = max(speedLevel - 1, -5);
        setSpeed();
        break;
        
      case '-':  // 减速
        if(speedLevel > 0) speedLevel = max(speedLevel - 1, 1);
        else if(speedLevel < 0) speedLevel = min(speedLevel + 1, -1);
        setSpeed();
        break;
        
      default:
        break;
    }
  }
}

void setSpeed() {
  int servoValue;
  
  if(speedLevel == 0) {
    servoValue = 90;  // 停止
  } else if(speedLevel > 0) {
    // 前进：91-180，值越大速度越快
    servoValue = map(speedLevel, 1, 5, 100, 180);
  } else {
    // 后退：0-89，值越小速度越快
    servoValue = map(speedLevel, -1, -5, 80, 0);
  }
  
  myServo.write(servoValue);
  
  Serial.print("速度等级: ");
  Serial.print(speedLevel);
  Serial.print(" 舵机值: ");
  Serial.println(servoValue);
}
```



**实践3：摇杆控制舵机（10分钟）**

**电路连接：**

text

```
摇杆模块：
  VRx（X轴） → A0
  VRy（Y轴） → A1
  SW（按键） → 引脚2（可选）
  VCC → 5V
  GND → GND
```



**代码实现：**

cpp

```
/*
 * 项目：摇杆控制舵机
 * 功能：用摇杆控制180度舵机角度
 */

#include <Servo.h>

Servo myServo;
int servoPin = 9;

// 摇杆引脚
int xPin = A0;  // X轴
int yPin = A1;  // Y轴
int swPin = 2;  // 按键（按下时归中）

int xValue, yValue;
int angle = 90;  // 初始角度

void setup() {
  myServo.attach(servoPin);
  pinMode(swPin, INPUT_PULLUP);  // 按键使用内部上拉
  
  Serial.begin(9600);
  Serial.println("摇杆舵机控制");
  Serial.println("Y轴控制角度，按键复位到90度");
  
  myServo.write(angle);  // 初始位置
}

void loop() {
  // 读取摇杆值
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  
  // 根据Y轴值计算角度（Y轴上下控制角度）
  // 模拟值0-1023映射到角度0-180
  angle = map(yValue, 0, 1023, 0, 180);
  
  // 限制角度范围
  angle = constrain(angle, 0, 180);
  
  // 控制舵机
  myServo.write(angle);
  
  // 按键按下时复位到90度
  if(digitalRead(swPin) == LOW) {
    angle = 90;
    myServo.write(angle);
    delay(300);  // 防抖
  }
  
  // 串口输出信息
  static unsigned long lastPrint = 0;
  if(millis() - lastPrint > 200) {
    Serial.print("X: ");
    Serial.print(xValue);
    Serial.print("  Y: ");
    Serial.print(yValue);
    Serial.print("  角度: ");
    Serial.println(angle);
    lastPrint = millis();
  }
  
  delay(20);  // 控制循环速度
}
```



**🔍 新知识点：**

- `analogRead(pin)`：读取模拟值(0-1023)
- `map(value, fromLow, fromHigh, toLow, toHigh)`：数值映射
- `constrain(value, min, max)`：限制数值范围
- `digitalRead(pin)`：读取数字引脚状态

#### **第四部分：知识总结与扩展（15分钟）**

**知识总结（5分钟）**

- 舵机工作原理和类型
- PWM控制原理
- Servo库使用方法
- 摇杆控制实现

**扩展活动：机械臂模拟（10分钟）**

cpp

```
/*
 * 扩展：双舵机机械臂模拟
 * 用两个舵机模拟机械臂的两个关节
 */

#include <Servo.h>

// 创建两个舵机对象
Servo baseServo;    // 底座舵机
Servo armServo;     // 机械臂舵机

// 引脚定义
int basePin = 9;
int armPin = 10;

// 目标角度
int baseAngle = 90;
int armAngle = 90;

// 动作序列
int movements[][2] = {
  {90, 90},   // 初始位置
  {45, 135},  // 位置1
  {135, 45},  // 位置2
  {90, 45},   // 位置3
  {90, 135},  // 位置4
  {90, 90}    // 返回初始
};
int movementCount = 6;
int currentMovement = 0;

void setup() {
  baseServo.attach(basePin);
  armServo.attach(armPin);
  
  Serial.begin(9600);
  Serial.println("双舵机机械臂模拟");
  Serial.println("自动执行预设动作序列");
  
  // 初始位置
  baseServo.write(baseAngle);
  armServo.write(armAngle);
  delay(1000);
}

void loop() {
  // 执行下一个动作
  baseAngle = movements[currentMovement][0];
  armAngle = movements[currentMovement][1];
  
  Serial.print("动作 ");
  Serial.print(currentMovement + 1);
  Serial.print(": 底座=");
  Serial.print(baseAngle);
  Serial.print("度, 机械臂=");
  Serial.print(armAngle);
  Serial.println("度");
  
  // 平滑移动到目标位置
  smoothMove(baseServo, baseAngle);
  smoothMove(armServo, armAngle);
  
  // 保持位置2秒
  delay(2000);
  
  // 下一个动作
  currentMovement = (currentMovement + 1) % movementCount;
}

// 平滑移动函数
void smoothMove(Servo &servo, int targetAngle) {
  int currentAngle = servo.read();
  
  if(currentAngle < targetAngle) {
    for(int angle = currentAngle; angle <= targetAngle; angle++) {
      servo.write(angle);
      delay(15);  // 控制移动速度
    }
  } else {
    for(int angle = currentAngle; angle >= targetAngle; angle--) {
      servo.write(angle);
      delay(15);
    }
  }
}
```



**家庭作业：**

1. 设计一个三舵机机械臂控制程序
2. 研究舵机电流需求，了解外部供电的必要性
3. 尝试用舵机制作一个简单的绘图仪

------

## MiniPlayer MP3模块

### 🎯 今日学习目标

1. 认识DFPlayer Mini MP3模块
2. 学会连接和配置MP3模块
3. 掌握MP3模块的基本控制函数
4. 实现音乐播放、暂停、切歌、音量控制
5. 制作一个简单的音乐播放器

### ⏰ 课程时间分配（90分钟）

#### **第一部分：课程导入（10分钟）**

- **音乐播放器展示（5分钟）**：展示各种MP3播放项目
- **技术提问（5分钟）**：
  - "Arduino如何播放音乐？"
  - "如何存储大量音频文件？"
  - "如何控制播放顺序和音量？"

#### **第二部分：理论知识讲解（20分钟）**

**1. DFPlayer Mini模块介绍（10分钟）**

<img src="D:\下载\deepseek_mermaid_20251218_126a50.png" alt="deepseek_mermaid_20251218_126a50" style="zoom: 200%;" />



**2. 串口通信基础（5分钟）**

- TX（发送）和RX（接收）
- 波特率：通信速度
- 软串口：用普通数字引脚模拟串口

**3. TF卡准备要求（5分钟）**

- 格式化为FAT32
- 创建mp3文件夹
- 文件命名：四位数字，如0001.mp3
- 支持文件夹分类

#### **第三部分：动手实践（40分钟）**

**实践1：MP3模块基础连接（10分钟）**

**电路连接：**

text

```
DFPlayer Mini：
  VCC → 5V
  GND → GND
  TX → Arduino引脚10（软串口RX）
  RX → Arduino引脚11（软串口TX）
  SPK1/SPK2 → 喇叭（注意正负极）
  
注意：Arduino与MP3模块的TX-RX要交叉连接
```



**TF卡准备：**

1. 格式化TF卡为FAT32
2. 创建"mp3"文件夹
3. 准备3-5个MP3文件，重命名为0001.mp3、0002.mp3...
4. 将TF卡插入模块

**实践2：基础播放程序（15分钟）**

**库安装：**

1. 工具 → 管理库
2. 搜索"DFRobotDFPlayerMini"
3. 安装最新版本

**代码实现：**

cpp

```
/*
 * 项目：基础MP3播放器
 * 功能：播放TF卡中的音乐文件
 * 需要DFRobotDFPlayerMini库
 */

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// 创建软串口对象（RX, TX）
SoftwareSerial mp3Serial(10, 11);
// 创建MP3播放器对象
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  // 初始化串口
  mp3Serial.begin(9600);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println("DFPlayer Mini MP3播放器初始化...");
  Serial.println("==============================");
  
  // 初始化MP3模块
  if (!myDFPlayer.begin(mp3Serial)) {
    Serial.println("初始化失败！");
    Serial.println("请检查：");
    Serial.println("1. 接线是否正确（TX-RX交叉）");
    Serial.println("2. SD卡是否插入");
    Serial.println("3. 音量是否设置为0");
    Serial.println("4. 文件命名是否正确");
    while(true);  // 停止程序
  }
  
  Serial.println("DFPlayer Mini初始化成功！");
  Serial.println();
  
  // 设置音量（0-30）
  myDFPlayer.volume(20);  // 建议20左右
  Serial.println("音量设置为20");
  
  // 设置EQ模式（0-5）
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  Serial.println("EQ模式：普通");
  
  // 播放第一首歌
  myDFPlayer.play(1);
  Serial.println("开始播放第1首歌曲");
  Serial.println();
  
  // 显示控制指令
  printInstructions();
}

void loop() {
  // 串口控制
  if (Serial.available()) {
    char command = Serial.read();
    handleCommand(command);
  }
}

void handleCommand(char command) {
  switch(command) {
    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9':
      playTrack(command - '0');
      break;
      
    case 'p':  // 播放/暂停
      myDFPlayer.pause();
      Serial.println("播放/暂停");
      break;
      
    case 's':  // 停止
      myDFPlayer.stop();
      Serial.println("停止播放");
      break;
      
    case 'n':  // 下一首
      myDFPlayer.next();
      Serial.println("下一首");
      break;
      
    case 'b':  // 上一首
      myDFPlayer.previous();
      Serial.println("上一首");
      break;
      
    case '+':  // 音量加
      myDFPlayer.volumeUp();
      Serial.println("音量增加");
      break;
      
    case '-':  // 音量减
      myDFPlayer.volumeDown();
      Serial.println("音量减少");
      break;
      
    case 'l':  // 循环播放
      myDFPlayer.enableLoop();
      Serial.println("单曲循环");
      break;
      
    case 'a':  // 循环所有
      myDFPlayer.enableLoopAll();
      Serial.println("全部循环");
      break;
      
    case 'r':  // 随机播放
      myDFPlayer.randomAll();
      Serial.println("随机播放");
      break;
      
    case 'i':  // 打印信息
      printInstructions();
      break;
      
    case ' ':  // 空格显示状态
      Serial.print("当前音量: ");
      Serial.println(myDFPlayer.readVolume());
      Serial.print("当前EQ: ");
      Serial.println(myDFPlayer.readEQ());
      Serial.print("当前文件: ");
      Serial.println(myDFPlayer.readCurrentFileNumber());
      break;
  }
}

void playTrack(int track) {
  myDFPlayer.play(track);
  Serial.print("播放第");
  Serial.print(track);
  Serial.println("首歌曲");
}

void printInstructions() {
  Serial.println("可用控制命令：");
  Serial.println(" 1-9 : 播放对应歌曲");
  Serial.println(" p   : 播放/暂停");
  Serial.println(" s   : 停止");
  Serial.println(" n   : 下一首");
  Serial.println(" b   : 上一首");
  Serial.println(" +   : 音量增加");
  Serial.println(" -   : 音量减少");
  Serial.println(" l   : 单曲循环");
  Serial.println(" a   : 全部循环");
  Serial.println(" r   : 随机播放");
  Serial.println(" 空格: 显示状态");
  Serial.println(" i   : 显示本帮助");
  Serial.println();
}
```



**🔍 DFPlayer库知识点：**

- `begin(serial)`：初始化MP3模块
- `volume(0-30)`：设置音量
- `play(track)`：播放指定曲目
- `pause()`：暂停/播放切换
- `stop()`：停止播放
- `next()`/`previous()`：下一首/上一首
- `volumeUp()`/`volumeDown()`：音量调节
- `enableLoop()`：单曲循环
- `enableLoopAll()`：全部循环
- `randomAll()`：随机播放

**实践3：按键控制播放器（15分钟）**

**电路连接（添加按键）：**

text

```
按键连接（一端接GND，一端接引脚，使用内部上拉）：
  播放/暂停 → 引脚2
  下一首 → 引脚3
  上一首 → 引脚4
  音量加 → 引脚5
  音量减 → 引脚6
```



**代码实现：**

cpp

```
/*
 * 项目：按键控制MP3播放器
 * 功能：用物理按键控制音乐播放
 */

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mp3Serial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

// 按键引脚定义
#define BTN_PLAY 2
#define BTN_NEXT 3
#define BTN_PREV 4
#define BTN_VOL_UP 5
#define BTN_VOL_DOWN 6

// 按键状态变量
int lastPlayState = HIGH;
int lastNextState = HIGH;
int lastPrevState = HIGH;
int lastVolUpState = HIGH;
int lastVolDownState = HIGH;

// 播放状态
bool isPlaying = true;

void setup() {
  // 设置按键引脚（使用内部上拉电阻）
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  pinMode(BTN_VOL_UP, INPUT_PULLUP);
  pinMode(BTN_VOL_DOWN, INPUT_PULLUP);
  
  // 初始化串口
  mp3Serial.begin(9600);
  Serial.begin(115200);
  
  Serial.println("按键MP3播放器初始化中...");
  
  // 初始化MP3模块
  if (!myDFPlayer.begin(mp3Serial)) {
    Serial.println("MP3模块初始化失败！");
    while(true);
  }
  
  // 初始设置
  myDFPlayer.volume(20);
  myDFPlayer.play(1);
  
  Serial.println("按键MP3播放器就绪！");
  Serial.println("使用按键控制播放：");
  Serial.println("播放/暂停：引脚2");
  Serial.println("下一首：引脚3");
  Serial.println("上一首：引脚4");
  Serial.println("音量加：引脚5");
  Serial.println("音量减：引脚6");
}

void loop() {
  // 读取所有按键状态
  int playState = digitalRead(BTN_PLAY);
  int nextState = digitalRead(BTN_NEXT);
  int prevState = digitalRead(BTN_PREV);
  int volUpState = digitalRead(BTN_VOL_UP);
  int volDownState = digitalRead(BTN_VOL_DOWN);
  
  // 播放/暂停按键处理
  if (playState == LOW && lastPlayState == HIGH) {
    delay(50);  // 消抖
    if (digitalRead(BTN_PLAY) == LOW) {
      if (isPlaying) {
        myDFPlayer.pause();
        Serial.println("音乐暂停");
        isPlaying = false;
      } else {
        myDFPlayer.start();  // 继续播放
        Serial.println("音乐播放");
        isPlaying = true;
      }
    }
  }
  lastPlayState = playState;
  
  // 下一首按键处理
  if (nextState == LOW && lastNextState == HIGH) {
    delay(50);
    if (digitalRead(BTN_NEXT) == LOW) {
      myDFPlayer.next();
      Serial.println("下一首");
      isPlaying = true;
    }
  }
  lastNextState = nextState;
  
  // 上一首按键处理
  if (prevState == LOW && lastPrevState == HIGH) {
    delay(50);
    if (digitalRead(BTN_PREV) == LOW) {
      myDFPlayer.previous();
      Serial.println("上一首");
      isPlaying = true;
    }
  }
  lastPrevState = prevState;
  
  // 音量增加处理
  if (volUpState == LOW && lastVolUpState == HIGH) {
    delay(50);
    if (digitalRead(BTN_VOL_UP) == LOW) {
      myDFPlayer.volumeUp();
      Serial.print("音量增加，当前：");
      Serial.println(myDFPlayer.readVolume());
    }
  }
  lastVolUpState = volUpState;
  
  // 音量减少处理
  if (volDownState == LOW && lastVolDownState == HIGH) {
    delay(50);
    if (digitalRead(BTN_VOL_DOWN) == LOW) {
      myDFPlayer.volumeDown();
      Serial.print("音量减少，当前：");
      Serial.println(myDFPlayer.readVolume());
    }
  }
  lastVolDownState = volDownState;
  
  delay(10);  // 主循环延迟
}
```



#### **第四部分：知识总结与扩展（15分钟）**

**知识总结（5分钟）**

- DFPlayer Mini模块连接方法
- 软串口通信原理
- MP3文件命名规范
- 基本控制函数使用

**扩展活动：音乐盒项目（10分钟）**

cpp

```
/*
 * 扩展：智能音乐盒
 * 功能：根据时间或传感器自动播放不同音乐
 */

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mp3Serial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

// 光敏电阻引脚
int lightSensorPin = A0;
// 按键引脚
int modeButtonPin = 2;

// 播放模式
enum PlayMode { MORNING, DAY, EVENING, NIGHT };
PlayMode currentMode = DAY;
int lastButtonState = HIGH;

// 不同模式对应的播放列表
int morningTracks[] = {1, 2, 3};  // 轻松音乐
int dayTracks[] = {4, 5, 6};      // 活跃音乐
int eveningTracks[] = {7, 8};     // 舒缓音乐
int nightTracks[] = {9, 10};      // 安静音乐

int currentTrackIndex = 0;

void setup() {
  pinMode(lightSensorPin, INPUT);
  pinMode(modeButtonPin, INPUT_PULLUP);
  
  mp3Serial.begin(9600);
  Serial.begin(115200);
  
  Serial.println("智能音乐盒启动...");
  
  if (!myDFPlayer.begin(mp3Serial)) {
    Serial.println("MP3模块初始化失败！");
    while(true);
  }
  
  myDFPlayer.volume(15);
  determineMode();  // 初始确定模式
  playCurrentTrack();
}

void loop() {
  // 检查模式切换按钮
  int buttonState = digitalRead(modeButtonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50);
    if (digitalRead(modeButtonPin) == LOW) {
      switchMode();
    }
  }
  lastButtonState = buttonState;
  
  // 每30秒检查一次光线，调整模式
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 30000) {
    determineMode();
    lastCheck = millis();
  }
  
  // 检查当前歌曲是否播放完毕
  // 注意：实际项目中可能需要使用BUSY引脚
  static unsigned long lastPlayTime = 0;
  if (millis() - lastPlayTime > 180000) {  // 假设每首歌3分钟
    nextTrack();
    lastPlayTime = millis();
  }
  
  delay(100);
}

void determineMode() {
  int lightValue = analogRead(lightSensorPin);
  Serial.print("光线值: ");
  Serial.println(lightValue);
  
  if (lightValue < 200) {
    currentMode = NIGHT;
    Serial.println("模式：夜晚");
  } else if (lightValue < 400) {
    currentMode = EVENING;
    Serial.println("模式：傍晚");
  } else if (lightValue < 700) {
    currentMode = DAY;
    Serial.println("模式：白天");
  } else {
    currentMode = MORNING;
    Serial.println("模式：早晨");
  }
}

void switchMode() {
  currentMode = (PlayMode)((currentMode + 1) % 4);
  currentTrackIndex = 0;
  
  switch(currentMode) {
    case MORNING:
      Serial.println("切换到早晨模式");
      break;
    case DAY:
      Serial.println("切换到白天模式");
      break;
    case EVENING:
      Serial.println("切换到傍晚模式");
      break;
    case NIGHT:
      Serial.println("切换到夜晚模式");
      break;
  }
  
  playCurrentTrack();
}

void playCurrentTrack() {
  int track = getCurrentTrack();
  if (track > 0) {
    myDFPlayer.play(track);
    Serial.print("播放第");
    Serial.print(track);
    Serial.println("首歌曲");
  }
}

void nextTrack() {
  currentTrackIndex++;
  playCurrentTrack();
}

int getCurrentTrack() {
  int* tracks;
  int trackCount;
  
  switch(currentMode) {
    case MORNING:
      tracks = morningTracks;
      trackCount = sizeof(morningTracks) / sizeof(morningTracks[0]);
      break;
    case DAY:
      tracks = dayTracks;
      trackCount = sizeof(dayTracks) / sizeof(dayTracks[0]);
      break;
    case EVENING:
      tracks = eveningTracks;
      trackCount = sizeof(eveningTracks) / sizeof(eveningTracks[0]);
      break;
    case NIGHT:
      tracks = nightTracks;
      trackCount = sizeof(nightTracks) / sizeof(nightTracks[0]);
      break;
  }
  
  if (currentTrackIndex >= trackCount) {
    currentTrackIndex = 0;
  }
  
  return tracks[currentTrackIndex];
}
```



**家庭作业：**

1. 制作一个生日贺卡音乐盒
2. 研究如何用MP3模块播放语音提示
3. 设计一个根据温度播放不同音乐的智能播放器

------

## 霍尔传感器及应用

### 🎯 今日学习目标

1. 了解霍尔效应原理
2. 学会使用霍尔传感器检测磁场
3. 掌握数字型和模拟型霍尔传感器的区别
4. 实现霍尔传感器控制LED、舵机、音乐播放器
5. 完成综合项目：磁控音乐盒

### ⏰ 课程时间分配（90分钟）

#### **第一部分：课程导入（10分钟）**

- **霍尔传感器应用展示（5分钟）**：手机翻盖检测、电动车测速、门窗报警
- **原理提问（5分钟）**：
  - "不用接触如何检测磁铁？"
  - "霍尔传感器有哪些类型？"
  - "如何用磁铁控制电子设备？"

#### **第二部分：理论知识讲解（20分钟）**

**1. 霍尔效应原理（10分钟）**

![deepseek_mermaid_20251218_53a197 (1)](D:\下载\deepseek_mermaid_20251218_53a197 (1).png)



**2. 霍尔传感器类型（5分钟）**

text

```
霍尔传感器类型：
├── 数字型（开关型）
│   ├── 特点：输出高低电平
│   ├── 应用：位置检测、计数
│   └── 型号：44E、A3144
│
├── 模拟型（线性型）
│   ├── 特点：输出电压随磁场变化
│   ├── 应用：距离检测、电流测量
│   └── 型号：49E、UGN3503
│
└── 锁存型
    ├── 特点：保持状态直到反向磁场
    ├── 应用：电机测速、旋转编码
    └── 型号：SS41
```



**3. 应用场景介绍（5分钟）**

- 转速测量：计算磁铁通过次数
- 位置检测：门开关、盖子开关
- 电流测量：非接触式电流检测
- 游戏控制器：磁力控制

#### **第三部分：动手实践（40分钟）**

**实践1：数字霍尔传感器基础使用（10分钟）**

**电路连接：**

text

```
数字霍尔传感器（如44E）：
  VCC → 5V
  GND → GND
  OUT → 引脚2（支持中断）
  
LED指示灯：引脚13（用于视觉反馈）
```



**代码实现：**

cpp

```
/*
 * 项目：数字霍尔传感器测试
 * 功能：检测磁铁靠近，统计次数
 * 使用中断提高响应速度
 */

// 引脚定义
int hallPin = 2;      // 霍尔传感器输出
int ledPin = 13;      // 状态指示灯

// 变量
volatile int magnetCount = 0;  // 磁铁检测次数（volatile确保多线程安全）
unsigned long lastDisplay = 0;

void setup() {
  pinMode(hallPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // 设置中断：引脚2状态变化时触发中断函数
  // FALLING：下降沿触发（磁铁靠近，输出从HIGH变LOW）
  attachInterrupt(digitalPinToInterrupt(hallPin), magnetDetected, FALLING);
  
  Serial.begin(9600);
  Serial.println("数字霍尔传感器测试");
  Serial.println("请用磁铁靠近传感器");
  Serial.println("=================================");
}

void loop() {
  // 每2秒显示一次计数
  if (millis() - lastDisplay > 2000) {
    Serial.print("磁铁检测次数: ");
    Serial.println(magnetCount);
    
    // 计算并显示频率（如果大于2次）
    if (magnetCount >= 2) {
      unsigned long currentTime = millis();
      static unsigned long lastTime = 0;
      static int lastCount = 0;
      
      float frequency = (magnetCount - lastCount) * 1000.0 / (currentTime - lastTime);
      Serial.print("检测频率: ");
      Serial.print(frequency, 2);
      Serial.println(" Hz");
      
      lastTime = currentTime;
      lastCount = magnetCount;
    }
    
    lastDisplay = millis();
  }
  
  // LED闪烁表示程序运行中
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 500) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    lastBlink = millis();
  }
}

// 中断服务函数
void magnetDetected() {
  magnetCount++;
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(10000);  // 10ms LED亮，视觉反馈
  digitalWrite(ledPin, LOW);
  
  Serial.println("检测到磁铁！");
}
```



**🔍 中断知识点：**

- `attachInterrupt(interrupt, function, mode)`：设置中断
- `digitalPinToInterrupt(pin)`：将引脚转换为中断号
- 中断模式：
  - `LOW`：低电平触发
  - `CHANGE`：变化触发
  - `RISING`：上升沿触发
  - `FALLING`：下降沿触发
- `volatile`：告诉编译器变量可能被中断修改
- 中断服务函数应尽量简短

**实践2：模拟霍尔传感器使用（15分钟）**

**电路连接：**

text

```
模拟霍尔传感器（如49E）：
  VCC → 5V
  GND → GND
  OUT → A0（模拟输入）
```



**代码实现：**

cpp

```
/*
 * 项目：模拟霍尔传感器测试
 * 功能：检测磁场强度和极性
 * 模拟传感器输出与磁场强度成正比的电压
 */

int hallPin = A0;      // 模拟霍尔传感器
int redLed = 9;        // 红色LED - 南磁极
int greenLed = 10;     // 绿色LED - 北磁极
int blueLed = 11;      // 蓝色LED - 无磁场

int sensorValue = 0;   // 原始传感器值
int baseline = 512;    // 无磁场时的基准值（2.5V）
int threshold = 50;    // 检测阈值

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("模拟霍尔传感器测试");
  Serial.println("用磁铁南北极分别靠近传感器");
  Serial.println("=================================");
  
  // 校准：读取无磁场时的基准值
  calibrate();
}

void loop() {
  // 读取传感器值
  sensorValue = analogRead(hallPin);
  
  // 计算与基准的差值
  int difference = sensorValue - baseline;
  
  // 串口输出
  Serial.print("原始值: ");
  Serial.print(sensorValue);
  Serial.print(" | 差值: ");
  Serial.print(difference);
  Serial.print(" | 磁场: ");
  
  // 判断磁场状态并控制LED
  if (abs(difference) < threshold) {
    // 无显著磁场
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, HIGH);
    Serial.println("无");
  } 
  else if (difference > threshold) {
    // 南磁极靠近（输出升高）
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, LOW);
    Serial.println("南磁极");
  } 
  else if (difference < -threshold) {
    // 北磁极靠近（输出降低）
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(blueLed, LOW);
    Serial.println("北磁极");
  }
  
  // 绘制磁场强度条形图
  plotMagnetStrength(difference);
  
  delay(200);  // 控制更新频率
}

void calibrate() {
  Serial.println("校准中...请确保无磁铁靠近");
  Serial.println("3秒后开始校准");
  delay(3000);
  
  long sum = 0;
  for (int i = 0; i < 100; i++) {
    sum += analogRead(hallPin);
    delay(10);
  }
  
  baseline = sum / 100;
  Serial.print("校准完成！基准值: ");
  Serial.println(baseline);
  Serial.println("现在可以用磁铁测试了");
  Serial.println();
}

void plotMagnetStrength(int strength) {
  Serial.print("强度图: ");
  
  // 归一化到-20到20的范围
  int normalized = map(constrain(strength, -200, 200), -200, 200, -20, 20);
  
  if (normalized < 0) {
    // 负值（北磁极）
    for (int i = -20; i < 0; i++) {
      if (i >= normalized) {
        Serial.print("#");
      } else {
        Serial.print(" ");
      }
    }
    Serial.print("|");  // 中心线
    Serial.println();
  } 
  else if (normalized > 0) {
    // 正值（南磁极）
    Serial.print("          |");  // 中心线
    for (int i = 0; i < 20; i++) {
      if (i <= normalized) {
        Serial.print("#");
      } else {
        break;
      }
    }
    Serial.println();
  } 
  else {
    // 零值
    Serial.println("          |          ");
  }
}
```



**实践3：霍尔传感器控制舵机（15分钟）**

**电路连接：**

text

```
数字霍尔传感器：VCC→5V, GND→GND, OUT→引脚2
舵机：信号→引脚9, 电源→外部5V
```



**代码实现：**

cpp

```
/*
 * 项目：磁控舵机门锁
 * 功能：用磁铁控制舵机模拟门锁开关
 */

#include <Servo.h>

Servo doorLock;      // 舵机模拟门锁
int servoPin = 9;
int hallPin = 2;     // 霍尔传感器

// 门锁状态
bool doorLocked = true;
bool lastMagnetState = false;
unsigned long lastActionTime = 0;
const unsigned long debounceTime = 500;  // 防抖时间500ms

void setup() {
  doorLock.attach(servoPin);
  pinMode(hallPin, INPUT_PULLUP);  // 使用内部上拉
  
  Serial.begin(9600);
  Serial.println("磁控舵机门锁系统");
  Serial.println("====================");
  Serial.println("初始状态：门已锁定");
  Serial.println("用磁铁钥匙开锁/上锁");
  
  // 初始位置：锁定状态（0度）
  doorLock.write(0);
  delay(1000);
}

void loop() {
  // 读取霍尔传感器状态（低电平表示磁铁靠近）
  bool magnetDetected = (digitalRead(hallPin) == LOW);
  
  // 检测状态变化
  if (magnetDetected != lastMagnetState) {
    lastActionTime = millis();  // 记录状态变化时间
  }
  
  // 防抖处理：状态稳定一段时间后才执行动作
  if ((millis() - lastActionTime > debounceTime) && 
      (magnetDetected != doorLocked)) {
    
    // 切换门锁状态
    doorLocked = !doorLocked;
    
    if (doorLocked) {
      lockDoor();
    } else {
      unlockDoor();
    }
    
    lastActionTime = millis();  // 防止重复触发
  }
  
  lastMagnetState = magnetDetected;
  
  // 状态指示灯
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > (doorLocked ? 1000 : 200)) {
    Serial.print(doorLocked ? "已锁定" : "未锁定");
    Serial.print(" | 传感器: ");
    Serial.println(magnetDetected ? "检测到磁铁" : "无磁铁");
    lastBlink = millis();
  }
  
  delay(50);
}

void unlockDoor() {
  Serial.println(">>> 正在开锁...");
  
  // 平滑开锁动作（0度 → 90度）
  for (int angle = 0; angle <= 90; angle += 2) {
    doorLock.write(angle);
    delay(30);
  }
  
  Serial.println(">>> 门已解锁");
  Serial.println("提示：移开磁铁并再次靠近可重新上锁");
}

void lockDoor() {
  Serial.println(">>> 正在上锁...");
  
  // 平滑上锁动作（90度 → 0度）
  for (int angle = 90; angle >= 0; angle -= 2) {
    doorLock.write(angle);
    delay(30);
  }
  
  Serial.println(">>> 门已锁定");
}
```



#### **第四部分：知识总结与扩展（15分钟）**

**知识总结（5分钟）**

- 霍尔效应原理
- 数字型和模拟型霍尔传感器区别
- 中断使用方法
- 防抖处理技巧

**扩展活动：综合项目 - 磁控音乐盒（10分钟）**

cpp

```
/*
 * 综合项目：磁控智能音乐盒
 * 功能：用磁铁控制音乐播放和灯光效果
 */

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <FastLED.h>

// 引脚定义
#define HALL_PIN 2        // 数字霍尔传感器
#define MP3_RX 10         // MP3模块RX
#define MP3_TX 11         // MP3模块TX
#define LED_PIN 6         // WS2812B灯带
#define NUM_LEDS 16       // LED数量

// 对象创建
SoftwareSerial mp3Serial(MP3_RX, MP3_TX);
DFRobotDFPlayerMini myDFPlayer;
CRGB leds[NUM_LEDS];

// 状态变量
enum SystemMode { MUSIC_MODE, LIGHT_MODE, PARTY_MODE };
SystemMode currentMode = MUSIC_MODE;

volatile bool magnetDetected = false;
int magnetCount = 0;
unsigned long lastMagnetTime = 0;
const unsigned long MODE_CHANGE_INTERVAL = 3000;  // 3秒内检测两次切换模式

void setup() {
  // 初始化串口
  Serial.begin(115200);
  mp3Serial.begin(9600);
  
  Serial.println("磁控智能音乐盒启动...");
  Serial.println("========================");
  
  // 初始化霍尔传感器中断
  pinMode(HALL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), handleMagnet, FALLING);
  
  // 初始化MP3模块
  if (!myDFPlayer.begin(mp3Serial)) {
    Serial.println("MP3模块初始化失败！");
  } else {
    myDFPlayer.volume(20);
    myDFPlayer.play(1);
    Serial.println("MP3模块就绪，开始播放音乐");
  }
  
  // 初始化LED灯带
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  Serial.println("LED灯带初始化完成");
  
  // 显示初始模式
  printModeInfo();
}

void loop() {
  // 处理磁铁检测
  if (magnetDetected) {
    handleMagnetAction();
    magnetDetected = false;
  }
  
  // 根据模式执行不同效果
  switch(currentMode) {
    case MUSIC_MODE:
      musicModeEffects();
      break;
    case LIGHT_MODE:
      lightModeEffects();
      break;
    case PARTY_MODE:
      partyModeEffects();
      break;
  }
  
  FastLED.show();
  delay(20);
}

// 中断服务函数
void handleMagnet() {
  unsigned long currentTime = millis();
  
  // 防抖处理
  if (currentTime - lastMagnetTime > 300) {
    magnetDetected = true;
    lastMagnetTime = currentTime;
  }
}

void handleMagnetAction() {
  static unsigned long lastActionTime = 0;
  unsigned long currentTime = millis();
  
  // 检测是否为快速双击（3秒内两次）
  if (currentTime - lastActionTime < MODE_CHANGE_INTERVAL) {
    // 双击：切换模式
    switchMode();
  } else {
    // 单击：当前模式内动作
    switch(currentMode) {
      case MUSIC_MODE:
        // 音乐模式下：切换歌曲
        myDFPlayer.next();
        Serial.println("切换下一首歌曲");
        break;
      case LIGHT_MODE:
        // 灯光模式下：切换颜色
        Serial.println("切换灯光颜色");
        break;
      case PARTY_MODE:
        // 派对模式下：切换效果
        Serial.println("切换派对效果");
        break;
    }
  }
  
  lastActionTime = currentTime;
}

void switchMode() {
  currentMode = (SystemMode)((currentMode + 1) % 3);
  printModeInfo();
  
  // 模式切换的特殊效果
  switch(currentMode) {
    case MUSIC_MODE:
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
      if (myDFPlayer.available()) {
        myDFPlayer.start();  // 继续播放音乐
      }
      break;
    case LIGHT_MODE:
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      if (myDFPlayer.available()) {
        myDFPlayer.pause();  // 暂停音乐
      }
      break;
    case PARTY_MODE:
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      if (myDFPlayer.available()) {
        myDFPlayer.volume(25);  // 增大音量
      }
      break;
  }
  
  FastLED.show();
  delay(500);
}

void printModeInfo() {
  Serial.println();
  Serial.print("切换到模式: ");
  switch(currentMode) {
    case MUSIC_MODE:
      Serial.println("音乐模式");
      Serial.println("单击：切换歌曲");
      Serial.println("双击：切换模式");
      break;
    case LIGHT_MODE:
      Serial.println("灯光模式");
      Serial.println("单击：切换颜色");
      Serial.println("双击：切换模式");
      break;
    case PARTY_MODE:
      Serial.println("派对模式");
      Serial.println("单击：切换效果");
      Serial.println("双击：切换模式");
      break;
  }
  Serial.println();
}

void musicModeEffects() {
  // 音乐节奏可视化
  static uint8_t hue = 0;
  static int beat = 0;
  
  // 模拟音乐节拍
  if (millis() % 300 < 150) {  // 每300ms切换一次
    beat = 150;
  } else {
    beat = 50;
  }
  
  // 创建波形效果
  for (int i = 0; i < NUM_LEDS; i++) {
    int wave = beatsin8(10 + i * 2, 50, beat, 0, i * 10);
    leds[i] = CHSV(hue + i * 5, 255, wave);
  }
  
  hue++;
}

void lightModeEffects() {
  // 呼吸灯效果
  static uint8_t brightness = 0;
  static int8_t direction = 1;
  
  brightness += direction;
  if (brightness == 0 || brightness == 150) {
    direction = -direction;
  }
  
  // 根据磁铁计数选择颜色
  CRGB color;
  switch (magnetCount % 5) {
    case 0: color = CRGB::Red; break;
    case 1: color = CRGB::Green; break;
    case 2: color = CRGB::Blue; break;
    case 3: color = CRGB::Yellow; break;
    case 4: color = CRGB::Purple; break;
  }
  
  fill_solid(leds, NUM_LEDS, color);
  FastLED.setBrightness(brightness);
}

void partyModeEffects() {
  // 派对灯光效果
  static uint8_t startIndex = 0;
  
  // 彩虹跑马灯
  fill_rainbow(leds, NUM_LEDS, startIndex, 7);
  
  // 随机闪烁效果
  if (random8() < 30) {  // 30%的几率
    int pos = random16(NUM_LEDS);
    leds[pos] = CRGB::White;
  }
  
  startIndex += 2;
}

// 磁铁计数中断（用于统计）
void IRAM_ATTR countMagnet() {
  static unsigned long lastCountTime = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastCountTime > 300) {  // 防抖
    magnetCount++;
    lastCountTime = currentTime;
  }
}
```



**家庭作业：**

1. 设计一个磁力计，显示磁场强度和方向
2. 制作一个磁铁驱动的计数器
3. 研究如何用霍尔传感器测量电机转速

------

## 教学工具和资源

### 每节课必备材料：

1. **硬件清单**：
   - Arduino Uno开发板 ×1
   - USB数据线 ×1
   - 面包板 ×1
   - 杜邦线（公对公、公对母）若干
   - LED灯（红、绿、蓝、RGB）若干
   - 电阻包（220Ω、1kΩ、10kΩ）
   - 相应传感器和模块
2. **软件工具**：
   - Arduino IDE（提前安装）
   - 串口监视器使用指南
   - 常见错误代码表

### 课堂管理建议：

1. **分组学习**：2-3人一组，培养协作能力
2. **进度检查**：每完成一个实践进行检查
3. **问题记录**：鼓励学生记录遇到的问题和解决方法
4. **作品展示**：每节课最后留时间展示成果

### 安全注意事项：

1. 通电前必须检查电路
2. 使用合适阻值的电阻保护LED
3. 舵机等大电流设备使用外部电源
4. 避免短路电源引脚

### 评估方式：

1. **课堂参与**：积极提问和回答问题
2. **实践完成度**：完成规定实验内容
3. **创意扩展**：在基础要求上添加创意功能
4. **代码理解**：能够解释代码工作原理

------

**教学提示：**

- 根据学生年龄调整讲解深度
- 鼓励学生尝试和犯错，从错误中学习
- 每节课预留5-10分钟答疑时间
- 建立作品展示墙，展示学生优秀作品

