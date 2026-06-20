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