#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>

// RC522引脚定义
#define RC522_SCK  6
#define RC522_MOSI 4
#define RC522_MISO 5
#define RC522_SDA  7
#define RC522_RST  3

// DFPlayer串口定义
#define DFPLAYER_RX 8
#define DFPLAYER_TX 9

// LED灯带定义
#define LED_DATA   10
#define NUM_LEDS   30

// MIFARE Ultralight 配置
#define START_PAGE 4      // 从第4页开始存储数据（避免使用前4页的厂商数据）
#define DATA_LENGTH 16    // 数据长度

// 呼吸灯参数
#define BREATHE_MIN 20    // 最小亮度
#define BREATHE_MAX 150   // 最大亮度
#define BREATHE_SPEED 60   // 呼吸速度

// 创建对象
MFRC522 mfrc522(RC522_SDA, RC522_RST);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_DATA, NEO_GRB + NEO_KHZ800);
HardwareSerial DFPlayerSerial(1);
DFRobotDFPlayerMini myDFPlayer;

// 卡片配置 - 现在不需要预定义颜色
struct CardConfig {
  const char* cardData;
  int trackNumber;
  const char* name;
};

CardConfig cardConfigs[] = {
  {"1", 1, "卡片1"},
  {"2", 2, "卡片2"},
  {"3", 3, "卡片3"},
  {"4", 4, "卡片4"},  
  {"5", 5, "卡片5"},
  {"6", 6, "卡片6"},
  {"7", 7, "卡片7"},
  {"8", 8, "卡片8"},
  {"9", 9, "卡片9"},
  {"10", 10, "卡片10"},
  {"11", 11, "卡片11"},
  {"12", 12, "卡片12"},  
  {"13", 13, "卡片13"},
  {"14", 14, "卡片14"},
  {"15", 15, "卡片15"},
  {"16", 16, "卡片16"}, 
  {"17", 17, "卡片17"},       
  // 新增第4张卡片
  // 可以继续添加更多卡片，不需要指定颜色
};

const int UNAUTHORIZED_TRACK = 5;  // 未授权卡片播放的曲目

bool dfplayerReady = false;

// 呼吸灯状态变量
int breatheBrightness = BREATHE_MIN;
bool breatheDirection = true; // true = 变亮, false = 变暗

// 新增：音乐播放状态变量
bool isMusicPlaying = false;
unsigned long lastMusicCheckTime = 0;
const unsigned long MUSIC_CHECK_INTERVAL = 500; // 每500ms检查一次音乐状态

// 新增：当前灯光效果颜色
uint32_t currentColor1 = 0;
uint32_t currentColor2 = 0;
bool isUnauthorizedMode = false;

void setup() {
  Serial.begin(115200);
  Serial.println("系统启动 - 支持MIFARE Ultralight卡片");
  
  // 初始化随机种子
  randomSeed(analogRead(0));
  
  // 初始化SPI
  SPI.begin(RC522_SCK, RC522_MISO, RC522_MOSI, RC522_SDA);
  
  // 初始化RC522
  mfrc522.PCD_Init();
  delay(4);
  
  // 显示RC522版本
  Serial.print("RC522版本: ");
  byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.println(version, HEX);
  
  // 初始化LED
  strip.begin();
  strip.show();
  
  // 初始化DFPlayer
  DFPlayerSerial.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);
  delay(2000);
  
  if (!myDFPlayer.begin(DFPlayerSerial)) {
    Serial.println("DFPlayer初始化失败！");
  } else {
    Serial.println("DFPlayer初始化成功！");
    dfplayerReady = true;
    myDFPlayer.volume(20);
  }
  
  startupAnimation();
  Serial.println("系统就绪，等待刷卡...");
}

void loop() {
  // 重新初始化RC522以防止卡死
  static unsigned long lastInit = 0;
  if (millis() - lastInit > 1000) {
    mfrc522.PCD_Init();
    lastInit = millis();
  }
  
  // 检查音乐播放状态并更新灯光效果
  checkMusicAndUpdateLights();
  
  // 检查卡片
  if (!mfrc522.PICC_IsNewCardPresent()) {
    delay(100);
    return;
  }
  
  Serial.println("检测到卡片!");
  
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("读取UID失败");
    delay(100);
    return;
  }
  
  handleCardDetected();
  delay(1000); // 防重复检测
}

// 新增：检查音乐状态并更新灯光
void checkMusicAndUpdateLights() {
  if (millis() - lastMusicCheckTime < MUSIC_CHECK_INTERVAL) {
    return;
  }
  lastMusicCheckTime = millis();
  
  // 如果有音乐在播放，更新灯光效果
  if (isMusicPlaying) {
    updateBreatheBrightness();
    
    if (isUnauthorizedMode) {
      // 未授权模式：红色呼吸效果
      uint32_t redColor = strip.Color(breatheBrightness, 0, 0);
      for(int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, redColor);
      }
    } else {
      // 正常模式：双色呼吸效果
      breatheTwoColorEffect(currentColor1, currentColor2);
    }
    strip.show();
  } else {
    // 音乐停止，关闭所有灯光
    if (strip.getPixelColor(0) != 0) { // 避免频繁设置
      for(int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, 0);
      }
      strip.show();
    }
  }
}

void handleCardDetected() {
  Serial.println("=== 开始处理卡片 ===");
  
  // 显示UID
  Serial.print("UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  // 显示卡片类型
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("卡片类型: ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  
  // 检查卡片类型
  if (piccType != MFRC522::PICC_TYPE_MIFARE_UL) {
    Serial.println("⚠️  这不是MIFARE Ultralight卡片，但尝试读取...");
  }
  
  // 读取数据
  String cardData = readUltralightData();
  
  if (cardData.length() > 0) {
    Serial.print("读取数据: \"");
    Serial.print(cardData);
    Serial.println("\"");
    
    int cardIndex = findCardConfig(cardData);
    if (cardIndex >= 0) {
      CardConfig config = cardConfigs[cardIndex];
      Serial.print("✅ 识别: ");
      Serial.println(config.name);
      
      // 为这张卡片生成随机颜色
      currentColor1 = generateRandomColor();
      currentColor2 = generateRandomColor();
      isUnauthorizedMode = false;
      
      playCardAction(config.trackNumber, config.name);
    } else {
      Serial.println("❌ 未授权卡片 - 数据不匹配");
      playUnauthorizedAction();
    }
  } else {
    Serial.println("❌ 无法读取数据或数据为空");
    playUnauthorizedAction();
  }
  
  mfrc522.PICC_HaltA();
  Serial.println("=== 处理完成 ===\n");
}

String readUltralightData() {
  String data = "";
  
  Serial.println("读取MIFARE Ultralight数据...");
  
  // MIFARE Ultralight 不需要认证，直接读取页面
  byte startPage = START_PAGE;
  byte pagesToRead = 4; // 读取4页（16字节）
  
  for (byte page = startPage; page < startPage + pagesToRead; page++) {
    // 读取页面
    byte buffer[18];
    byte size = sizeof(buffer);
    
    MFRC522::StatusCode status = mfrc522.MIFARE_Read(page, buffer, &size);
    
    if (status != MFRC522::STATUS_OK) {
      Serial.print("读取页面 ");
      Serial.print(page);
      Serial.print(" 失败: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      continue;
    }
    
    Serial.print("页面 ");
    Serial.print(page);
    Serial.print(": ");
    
    // 处理页面数据（每页4字节有效数据）
    for (byte i = 0; i < 4; i++) {
      if (buffer[i] != 0x00) { // 只处理非空字节
        data += (char)buffer[i];
        Serial.print("0x");
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
      } else {
        // 遇到空字节，停止读取
        Serial.println();
        return data;
      }
    }
    Serial.println();
  }
  
  return data;
}

int findCardConfig(String cardData) {
  cardData.trim(); // 去除可能的空白字符
  
  for (int i = 0; i < sizeof(cardConfigs) / sizeof(cardConfigs[0]); i++) {
    if (cardData.equals(cardConfigs[i].cardData)) {
      return i;
    }
  }
  return -1;
}

// 生成随机颜色
uint32_t generateRandomColor() {
  // 生成鲜艳但不刺眼的颜色
  int r = random(50, 200);
  int g = random(50, 200);
  int b = random(50, 200);
  
  // 确保颜色不会太暗
  if (r + g + b < 150) {
    int boost = random(50, 100);
    r = min(255, r + boost);
    g = min(255, g + boost);
    b = min(255, b + boost);
  }
  
  Serial.print("生成颜色: R=");
  Serial.print(r);
  Serial.print(" G=");
  Serial.print(g);
  Serial.print(" B=");
  Serial.println(b);
  
  return strip.Color(r, g, b);
}

// 修改：简化播放动作，只启动音乐
void playCardAction(int trackNumber, const char* cardName) {
  Serial.print("播放曲目: ");
  Serial.println(trackNumber);
  
  // 启动音乐播放
  if (dfplayerReady) {
    myDFPlayer.play(trackNumber);
    isMusicPlaying = true;
  }
}

// 更新呼吸亮度
void updateBreatheBrightness() {
  if (breatheDirection) {
    breatheBrightness += BREATHE_SPEED;
    if (breatheBrightness >= BREATHE_MAX) {
      breatheBrightness = BREATHE_MAX;
      breatheDirection = false;
    }
  } else {
    breatheBrightness -= BREATHE_SPEED;
    if (breatheBrightness <= BREATHE_MIN) {
      breatheBrightness = BREATHE_MIN;
      breatheDirection = true;
    }
  }
}

// 双色呼吸效果
void breatheTwoColorEffect(uint32_t color1, uint32_t color2) {
  // 分解颜色1
  uint8_t r1 = (color1 >> 16) & 0xFF;
  uint8_t g1 = (color1 >> 8) & 0xFF;
  uint8_t b1 = color1 & 0xFF;
  
  // 分解颜色2
  uint8_t r2 = (color2 >> 16) & 0xFF;
  uint8_t g2 = (color2 >> 8) & 0xFF;
  uint8_t b2 = color2 & 0xFF;
  
  // 计算当前混合比例（基于呼吸亮度）
  float mixRatio = (breatheBrightness - BREATHE_MIN) / (float)(BREATHE_MAX - BREATHE_MIN);
  
  // 为每个LED设置交替的颜色混合
  for(int i = 0; i < NUM_LEDS; i++) {
    float ledMixRatio;
    
    if (i % 2 == 0) {
      // 偶数LED：颜色1为主，颜色2为辅
      ledMixRatio = mixRatio;
    } else {
      // 奇数LED：颜色2为主，颜色1为辅
      ledMixRatio = 1.0 - mixRatio;
    }
    
    // 混合颜色
    uint8_t r = (uint8_t)(r1 * ledMixRatio + r2 * (1.0 - ledMixRatio));
    uint8_t g = (uint8_t)(g1 * ledMixRatio + g2 * (1.0 - ledMixRatio));
    uint8_t b = (uint8_t)(b1 * ledMixRatio + b2 * (1.0 - ledMixRatio));
    
    // 应用亮度
    float brightnessFactor = breatheBrightness / 255.0;
    r = (uint8_t)(r * brightnessFactor);
    g = (uint8_t)(g * brightnessFactor);
    b = (uint8_t)(b * brightnessFactor);
    
    strip.setPixelColor(i, r, g, b);
  }
}

// 修改：简化未授权动作
void playUnauthorizedAction() {
  Serial.println("播放未授权提示");
  
  // 播放错误音效
  if (dfplayerReady) {
    myDFPlayer.play(UNAUTHORIZED_TRACK);
    isMusicPlaying = true;
    isUnauthorizedMode = true;
  }
}

void startupAnimation() {
  Serial.println("启动动画...");
  
  // 彩虹呼吸效果
  for(int cycles = 0; cycles < 3; cycles++) {
    for(int hue = 0; hue < 256; hue += 5) {
      updateBreatheBrightness();
      
      for(int i = 0; i < NUM_LEDS; i++) {
        // 每个LED有不同的颜色偏移
        int ledHue = (hue + i * 256 / NUM_LEDS) % 256;
        uint32_t color = wheel(ledHue);
        
        // 应用呼吸亮度
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;
        
        float brightnessFactor = breatheBrightness / 255.0;
        r = (uint8_t)(r * brightnessFactor);
        g = (uint8_t)(g * brightnessFactor);
        b = (uint8_t)(b * brightnessFactor);
        
        strip.setPixelColor(i, r, g, b);
      }
      strip.show();
      delay(30);
    }
  }
  
  // 启动动画结束后关闭灯光
  for(int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

// 彩虹色轮函数
uint32_t wheel(byte wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return strip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return strip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return strip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// 新增：DFPlayer回调函数（如果需要检测播放结束）
// 注意：这需要连接DFPlayer的BUSY引脚到Arduino并设置中断
void checkPlaybackFinished() {
  // 这里可以添加检测音乐播放结束的逻辑
  // 如果检测到播放结束，设置 isMusicPlaying = false;
}