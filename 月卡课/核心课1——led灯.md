# 核心课1——led灯

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

```c++
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