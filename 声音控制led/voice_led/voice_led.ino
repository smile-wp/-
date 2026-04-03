#include <FastLED.h>  // 用于控制WS2812B灯带

// 硬件引脚定义
#define LED_PIN     5
#define NUM_LEDS    300  // 根据你的灯带LED数量修改
#define SOUND_PIN   A0

// 创建LED数组
CRGB leds[NUM_LEDS];

// 声音阈值和颜色映射
#define QUIET_THRESHOLD   160   // 安静环境阈值[citation:2]
#define MEDIUM_THRESHOLD  183   // 中等声音阈值[citation:2]
// 超过MEDIUM_THRESHOLD为高音量

void setup() {
  Serial.begin(9600);  // 启动串口通信，便于调试
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);  // 初始化灯带
  FastLED.setBrightness(100);  // 设置亮度(0-255)，初始别太亮保护眼睛
}

void loop() {
  int soundValue = analogRead(SOUND_PIN);  // 读取声音传感器数值
  Serial.println(soundValue);              // 打印数值到串口监视器，有助于确定阈值

  // 根据声音强度决定颜色
  if (soundValue > MEDIUM_THRESHOLD) {
    // 高音量：红色 (你可以修改为任何颜色)
    fill_solid(leds, NUM_LEDS, CRGB(255, 255, 0));
  } else if (soundValue > QUIET_THRESHOLD) {
    // 中等音量：蓝色 (你可以修改为任何颜色)
    fill_solid(leds, NUM_LEDS, CRGB(0, 0, 255));
  } else {
    // 安静环境：熄灭
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }

  FastLED.show();  // 更新灯带显示
  delay(20);       // 短暂延迟，稳定循环
}