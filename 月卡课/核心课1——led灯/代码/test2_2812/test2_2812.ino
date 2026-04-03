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