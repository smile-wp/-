#include <Adafruit_NeoPixel.h>

// LED灯带配置
#define LED_PIN     6
#define LED_COUNT   16
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// 触摸传感器配置
#define TOUCH_ADD_PIN   2  // 增加LED的触摸传感器
#define TOUCH_SUB_PIN   3  // 减少LED的触摸传感器

// 变量定义
int ledCount = 0;
bool lastAddState = false;
bool lastSubState = false;

// 颜色定义
uint32_t activeColor = strip.Color(0, 255, 0);   // 绿色
uint32_t inactiveColor = strip.Color(10, 10, 10); // 暗色

void setup() {
  Serial.begin(9600);
  pinMode(TOUCH_ADD_PIN, INPUT);
  pinMode(TOUCH_SUB_PIN, INPUT);
  
  strip.begin();
  strip.show(); // 初始化时关闭所有LED
  strip.setBrightness(100); // 设置亮度(0-255)
  
  Serial.println("双触摸LED计数器已启动");
  Serial.println("触摸传感器1(PIN2): 增加LED");
  Serial.println("触摸传感器2(PIN3): 减少LED");
  Serial.print("最大LED数量: ");
  Serial.println(LED_COUNT);
}

void loop() {
  bool currentAddState = digitalRead(TOUCH_ADD_PIN);
  bool currentSubState = digitalRead(TOUCH_SUB_PIN);
  
  // 检测增加触摸传感器的按下
  if (currentAddState && !lastAddState) {
    increaseLEDs();
    delay(200); // 防抖延时
  }
  
  // 检测减少触摸传感器的按下
  if (currentSubState && !lastSubState) {
    decreaseLEDs();
    delay(200); // 防抖延时
  }
  
  lastAddState = currentAddState;
  lastSubState = currentSubState;
  
  // 可选：添加呼吸灯效果
  // breathingEffect();
}

void increaseLEDs() {
  if (ledCount < LED_COUNT) {
    ledCount++;
    updateLEDs();
    Serial.print("增加LED, 当前数量: ");
    Serial.println(ledCount);
  } else {
    Serial.println("已达到最大LED数量");
    blinkEffect(strip.Color(255, 0, 0)); // 红色闪烁提示
  }
}

void decreaseLEDs() {
  if (ledCount > 0) {
    ledCount--;
    updateLEDs();
    Serial.print("减少LED, 当前数量: ");
    Serial.println(ledCount);
  } else {
    Serial.println("已无LED可关闭");
    blinkEffect(strip.Color(255, 255, 0)); // 黄色闪烁提示
  }
}

void updateLEDs() {
  strip.clear();
  
  for (int i = 0; i < LED_COUNT; i++) {
    if (i < ledCount) {
      strip.setPixelColor(i, activeColor);
    } else {
      strip.setPixelColor(i, inactiveColor);
    }
  }
  
  strip.show();
}

void blinkEffect(uint32_t color) {
  // 闪烁效果提示
  for (int i = 0; i < 2; i++) {
    strip.fill(color);
    strip.show();
    delay(100);
    strip.clear();
    strip.show();
    delay(100);
  }
  // 恢复当前状态
  updateLEDs();
}

// 呼吸灯效果（可选）
void breathingEffect() {
  if (ledCount == 0) return;
  
  for (int brightness = 20; brightness <= 150; brightness += 5) {
    for (int i = 0; i < ledCount; i++) {
      strip.setPixelColor(i, 0, brightness, 0);
    }
    strip.show();
    delay(30);
  }
  
  for (int brightness = 150; brightness >= 20; brightness -= 5) {
    for (int i = 0; i < ledCount; i++) {
      strip.setPixelColor(i, 0, brightness, 0);
    }
    strip.show();
    delay(30);
  }
}

// 彩虹色效果（高级功能）
void rainbowEffect() {
  if (ledCount == 0) return;
  
  for (long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for (int i = 0; i < ledCount; i++) {
      int pixelHue = firstPixelHue + (i * 65536L / ledCount);
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(10);
    
    // 检查是否有触摸输入，有则退出效果
    if (digitalRead(TOUCH_ADD_PIN) || digitalRead(TOUCH_SUB_PIN)) {
      break;
    }
  }
  updateLEDs(); // 恢复正常显示
}