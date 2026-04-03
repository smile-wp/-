#include <Wire.h>

#define MAX30102_ADDRESS 0x57

// 寄存器地址
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

const int GREEN_LED_PIN = 8;
const int RED_LED_PIN = 9;

// 采样变量
unsigned long lastSampleTime = 0;
const int SAMPLE_INTERVAL = 10;
int irValue = 0;
int previousIR = 0;

// 信号强度调试
int maxEverIR = 0;
int minEverIR = 100000;
unsigned long startTime = 0;

// Processing通信变量 - 新增
unsigned long lastDataSendTime = 0;
const int DATA_SEND_INTERVAL = 50; // 每50ms发送一次数据到Processing
int simulatedHeartRate = 70;       // 模拟心率值
int simulatedSPO2 = 98;            // 模拟血氧值
bool fingerDetected = false;       // 手指检测状态
bool beatDetected = false;         // 心跳检测状态

void setup() {
  Serial.begin(9600); // 初始化串口通信 - 新增
  Serial.println("HEART_RATE_DATA_START"); // Processing识别标记 - 新增
  
  Serial.println("MAX30102 信号强度调试");
  Serial.println("====================");

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  // 指示灯
  for (int i = 0; i < 2; i++) {
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(200);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    delay(200);
  }

  Wire.begin();
  
  // 尝试不同的配置
  testDifferentConfigurations();

  startTime = millis();
  Serial.println("开始信号强度测试...");
  Serial.println("请尝试：");
  Serial.println("1. 不同手指位置");
  Serial.println("2. 不同按压力度");
  Serial.println("3. 不同手指（食指/中指）");
  Serial.println("====================");
}

void testDifferentConfigurations() {
  Serial.println("测试传感器配置...");
  
  // 配置1：默认设置
  configureSensor(0x1F, 0x1F, 0x27); // 31mA, 100Hz
  delay(1000);
  readAndPrintIR("配置1");
  
  // 配置2：最大电流
  configureSensor(0xFF, 0xFF, 0x27); // 最大电流, 100Hz
  delay(1000);
  readAndPrintIR("配置2");
  
  // 配置3：高灵敏度模式
  configureSensor(0xFF, 0xFF, 0x07); // 最大电流, 50Hz
  delay(1000);
  readAndPrintIR("配置3");
  
  Serial.println("配置测试完成");
  Serial.println("====================");
}

void configureSensor(byte led1Current, byte led2Current, byte spo2Config) {
  writeRegister(REG_MODE_CONFIG, 0x40);
  delay(50);
  
  writeRegister(REG_INTR_ENABLE_1, 0xC0);
  writeRegister(REG_FIFO_WR_PTR, 0x00);
  writeRegister(REG_OVF_COUNTER, 0x00);
  writeRegister(REG_FIFO_RD_PTR, 0x00);
  
  writeRegister(REG_FIFO_CONFIG, 0x4F);
  writeRegister(REG_MODE_CONFIG, 0x03);
  writeRegister(REG_SPO2_CONFIG, spo2Config);
  
  writeRegister(REG_LED1_PA, led1Current);
  writeRegister(REG_LED2_PA, led2Current);
  
  Serial.print("配置: LED=");
  Serial.print(led1Current, HEX);
  Serial.print(", 模式=");
  Serial.println(spo2Config, HEX);
}

void readAndPrintIR(const char* configName) {
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += readFIFO();
    delay(10);
  }
  int avgIR = sum / 10;
  
  Serial.print(configName);
  Serial.print(": IR=");
  Serial.println(avgIR);
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;
  
  if (currentTime - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = currentTime;
    
    previousIR = irValue;
    irValue = readFIFO();
    
    // 更新手指检测状态 - 新增
    bool newFingerDetected = (irValue > 3000);
    if (newFingerDetected != fingerDetected) {
      fingerDetected = newFingerDetected;
      if (fingerDetected) {
        Serial.println("手指检测成功");
      }
    }
    
    // 更新心跳检测状态 - 新增
    beatDetected = detectSimpleBeat();
    
    // 更新极值记录
    if (irValue > maxEverIR) maxEverIR = irValue;
    if (irValue > 0 && irValue < minEverIR) minEverIR = irValue;
    
    // 实时显示
    if (elapsedTime % 1000 < 20) {
      printSignalStatus();
    }
    
    // 控制LED基于信号强度
    updateLEDsBasedOnSignal();
    
    // 每5秒显示详细报告
    if (elapsedTime % 5000 < 20) {
      printDetailedReport();
    }
  }
  
  // 发送数据到Processing - 新增
  if (currentTime - lastDataSendTime >= DATA_SEND_INTERVAL) {
    lastDataSendTime = currentTime;
    sendDataToProcessing();
  }
  
  // 每30秒重新配置传感器以找到最佳设置
  if (elapsedTime % 30000 < 20) {
    tryOptimalConfiguration();
  }
}

// 发送数据到Processing函数 - 新增
void sendDataToProcessing() {
  // 更新模拟心率数据（基于信号强度）
  if (fingerDetected) {
    // 有手指时生成更真实的心率数据
    simulatedHeartRate = 70 + (irValue % 11) - 5; // 65-75范围内波动
    simulatedSPO2 = 96 + (irValue % 5) - 2;       // 94-98范围内波动
  } else {
    // 无手指时使用固定值
    simulatedHeartRate = 0;
    simulatedSPO2 = 0;
  }
  
  // 发送格式: 心率,血氧,信号强度,心跳状态,手指状态
  Serial.print(simulatedHeartRate);
  Serial.print(",");
  Serial.print(simulatedSPO2);
  Serial.print(",");
  Serial.print(irValue);
  Serial.print(",");
  Serial.print(beatDetected ? "1" : "0");
  Serial.print(",");
  Serial.print(fingerDetected ? "1" : "0");
  Serial.println(); // 换行符
}

void printSignalStatus() {
  Serial.print("IR: ");
  Serial.print(irValue);
  Serial.print(" | 范围: ");
  Serial.print(minEverIR);
  Serial.print("-");
  Serial.print(maxEverIR);
  
  if (irValue < 1000) {
    Serial.println(" | ❌ 信号太弱");
  } else if (irValue < 3000) {
    Serial.println(" | ⚠️  信号较弱");
  } else if (irValue < 10000) {
    Serial.println(" | ✅ 信号良好");
  } else {
    Serial.println(" | 🎉 信号优秀");
  }
}

void updateLEDsBasedOnSignal() {
  // 先关闭所有LED
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  
  if (!fingerDetected) {
    // 无手指：红色慢闪
    if (millis() % 2000 < 200) {
      digitalWrite(RED_LED_PIN, HIGH);
    }
  } else if (irValue < 2000) {
    // 弱信号：红色常亮
    digitalWrite(RED_LED_PIN, HIGH);
  } else if (irValue < 5000) {
    // 中等信号：黄色（红+绿）
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
  } else {
    // 强信号：绿色常亮
    digitalWrite(GREEN_LED_PIN, HIGH);
    
    // 心跳检测时的闪烁效果 - 增强
    if (beatDetected) {
      digitalWrite(GREEN_LED_PIN, LOW);
      delay(60); // 心跳闪烁持续时间
      digitalWrite(GREEN_LED_PIN, HIGH);
    }
  }
}

bool detectSimpleBeat() {
  static int lastValue = 0;
  static unsigned long lastBeatTime = 0;
  int change = irValue - lastValue;
  lastValue = irValue;
  
  // 心跳检测逻辑
  if (fingerDetected && change > 150 && millis() - lastBeatTime > 300) {
    lastBeatTime = millis();
    return true;
  }
  return false;
}

void printDetailedReport() {
  Serial.println("--- 详细信号报告 ---");
  Serial.print("运行时间: ");
  Serial.print((millis() - startTime) / 1000);
  Serial.println(" 秒");
  
  Serial.print("信号范围: ");
  Serial.print(minEverIR);
  Serial.print(" - ");
  Serial.println(maxEverIR);
  
  Serial.print("当前配置: LED=");
  Serial.print("0x");
  Serial.print(readRegister(REG_LED1_PA), HEX);
  Serial.print(", 模式=0x");
  Serial.println(readRegister(REG_SPO2_CONFIG), HEX);
  
  Serial.print("手指状态: ");
  Serial.println(fingerDetected ? "检测中" : "未检测");
  
  Serial.println("建议操作:");
  if (maxEverIR < 2000) {
    Serial.println("1. ✅ 检查手指是否完全覆盖传感器");
    Serial.println("2. ✅ 尝试不同的按压力度");
    Serial.println("3. ✅ 确保环境光线不要太强");
  }
  Serial.println("-------------------");
}

void tryOptimalConfiguration() {
  Serial.println("尝试优化配置...");
  
  // 尝试不同的配置组合
  byte configurations[][3] = {
    {0xFF, 0xFF, 0x27}, // 最大电流，100Hz
    {0x3F, 0x3F, 0x27}, // 中等电流，100Hz
    {0xFF, 0xFF, 0x07}, // 最大电流，50Hz
    {0x7F, 0x7F, 0x17}  // 高电流，200Hz
  };
  
  for (int i = 0; i < 4; i++) {
    configureSensor(configurations[i][0], configurations[i][1], configurations[i][2]);
    delay(500);
    
    int testIR = 0;
    for (int j = 0; j < 5; j++) {
      testIR += readFIFO();
      delay(10);
    }
    testIR /= 5;
    
    Serial.print("配置 ");
    Serial.print(i + 1);
    Serial.print(": IR=");
    Serial.println(testIR);
    
    if (testIR > 5000) {
      Serial.println("🎉 找到良好配置！");
      return;
    }
  }
  
  Serial.println("⚠️  未找到理想配置，使用默认");
  configureSensor(0x1F, 0x1F, 0x27);
}

int readFIFO() {
  Wire.beginTransmission(MAX30102_ADDRESS);
  Wire.write(REG_FIFO_DATA);
  Wire.endTransmission(false);
  
  Wire.requestFrom(MAX30102_ADDRESS, 6);
  
  if (Wire.available() >= 6) {
    byte data[6];
    for (int i = 0; i < 6; i++) {
      data[i] = Wire.read();
    }
    return ((data[0] & 0x03) << 16) | (data[1] << 8) | data[2];
  }
  return 0;
}

byte readRegister(byte reg) {
  Wire.beginTransmission(MAX30102_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission(false);
  
  Wire.requestFrom(MAX30102_ADDRESS, 1);
  if (Wire.available()) {
    return Wire.read();
  }
  return 0;
}

void writeRegister(byte reg, byte value) {
  Wire.beginTransmission(MAX30102_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}