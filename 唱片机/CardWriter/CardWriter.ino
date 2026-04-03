#include <SPI.h>
#include <MFRC522.h>

#define RC522_SCK  6
#define RC522_MOSI 4
#define RC522_MISO 5
#define RC522_SDA  7
#define RC522_RST  3

#define START_PAGE 4

MFRC522 mfrc522(RC522_SDA, RC522_RST);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("=== MIFARE Ultralight 卡片写入工具 ===");
  
  SPI.begin(RC522_SCK, RC522_MISO, RC522_MOSI, RC522_SDA);
  mfrc522.PCD_Init();
  delay(4);
  
  Serial.print("RC522版本: ");
  Serial.println(mfrc522.PCD_ReadRegister(mfrc522.VersionReg), HEX);
  
  Serial.println("\n使用方法:");
  Serial.println("1. 将MIFARE Ultralight卡片靠近读卡器");
  Serial.println("2. 在串口输入要写入的数据（如: 1, 2, 3）");
  Serial.println("3. 按回车发送");
  Serial.println("4. 数据将写入卡片页面4开始的位置");
  Serial.println("输入 'scan' 扫描卡片信息和当前数据");
  Serial.println("=====================================\n");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input.equalsIgnoreCase("scan")) {
      scanCard();
    } else if (input.length() > 0) {
      writeUltralightData(input.c_str());
    }
  }
  
  delay(100);
}

void scanCard() {
  Serial.println("等待卡片扫描...");
  
  // 等待卡片
  unsigned long startTime = millis();
  while (!mfrc522.PICC_IsNewCardPresent()) {
    if (millis() - startTime > 5000) {
      Serial.println("超时，未检测到卡片");
      return;
    }
    delay(100);
  }
  
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("读取卡片失败");
    return;
  }
  
  Serial.println("\n=== 卡片信息 ===");
  Serial.print("UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("类型: ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  
  // 读取当前数据
  String currentData = readUltralightData();
  if (currentData.length() > 0) {
    Serial.print("当前数据: \"");
    Serial.print(currentData);
    Serial.println("\"");
  } else {
    Serial.println("当前数据: 空或无法读取");
  }
  
  mfrc522.PICC_HaltA();
  Serial.println("================\n");
}

String readUltralightData() {
  String data = "";
  
  byte startPage = START_PAGE;
  byte pagesToRead = 4;
  
  for (byte page = startPage; page < startPage + pagesToRead; page++) {
    byte buffer[18];
    byte size = sizeof(buffer);
    
    MFRC522::StatusCode status = mfrc522.MIFARE_Read(page, buffer, &size);
    
    if (status != MFRC522::STATUS_OK) {
      continue;
    }
    
    for (byte i = 0; i < 4; i++) {
      if (buffer[i] != 0x00) {
        data += (char)buffer[i];
      } else {
        return data;
      }
    }
  }
  
  return data;
}

void writeUltralightData(const char* data) {
  Serial.println("等待卡片写入...");
  
  // 等待卡片
  unsigned long startTime = millis();
  while (!mfrc522.PICC_IsNewCardPresent()) {
    if (millis() - startTime > 5000) {
      Serial.println("超时，未检测到卡片");
      return;
    }
    delay(100);
  }
  
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("读取卡片失败");
    return;
  }
  
  Serial.print("检测到卡片，UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("卡片类型: ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  
  if (piccType != MFRC522::PICC_TYPE_MIFARE_UL) {
    Serial.println("⚠️  警告：这不是MIFARE Ultralight卡片，写入可能失败！");
  }
  
  // 写入数据到页面
  byte dataLength = strlen(data);
  byte currentPage = START_PAGE;
  byte bytesWritten = 0;
  
  for (byte i = 0; i < dataLength; i += 4) {
    // 准备4字节的页面数据
    byte pageData[4] = {0, 0, 0, 0};
    byte bytesInPage = min(4, dataLength - i);
    
    for (byte j = 0; j < bytesInPage; j++) {
      pageData[j] = data[i + j];
    }
    
    // 写入页面
    MFRC522::StatusCode status = mfrc522.MIFARE_Ultralight_Write(currentPage, pageData, 4);
    
    if (status != MFRC522::STATUS_OK) {
      Serial.print("写入页面 ");
      Serial.print(currentPage);
      Serial.print(" 失败: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      break;
    } else {
      Serial.print("✅ 页面 ");
      Serial.print(currentPage);
      Serial.print(" 写入成功: ");
      for (byte j = 0; j < bytesInPage; j++) {
        Serial.print("0x");
        Serial.print(pageData[j], HEX);
        Serial.print(" ");
      }
      Serial.println();
      bytesWritten += bytesInPage;
    }
    
    currentPage++;
    
    // 安全限制：最多写入8页（从页面4到11）
    if (currentPage >= 12) {
      Serial.println("⚠️  已达到最大页面限制");
      break;
    }
  }
  
  Serial.println("✅ 数据写入完成！");
  
  // 验证写入
  Serial.println("验证写入的数据...");
  String verifyData = readUltralightData();
  if (verifyData.equals(data)) {
    Serial.println("✅ 验证成功！");
  } else {
    Serial.print("❌ 验证失败！期望: \"");
    Serial.print(data);
    Serial.print("\", 实际: \"");
    Serial.print(verifyData);
    Serial.println("\"");
  }
  
  mfrc522.PICC_HaltA();
}