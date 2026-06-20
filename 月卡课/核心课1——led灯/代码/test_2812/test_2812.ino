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