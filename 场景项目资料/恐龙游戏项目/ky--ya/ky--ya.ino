#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

TFT_eSPI tft = TFT_eSPI();

// 设置WiFi接入点信息
const char* ssid = "ESP32-Video-Player";
const char* password = "12345678";

// 创建异步Web服务器对象，监听80端口
AsyncWebServer server(80);

// 当前播放的视频索引
int currentVideo = 0;

// 简单的视频帧数据（示例）
// 实际使用时，你需要将视频转换为像素数组
const uint16_t video1[][4] = {
  {TFT_RED, TFT_RED, TFT_RED, TFT_RED},
  {TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE},
  {TFT_GREEN, TFT_GREEN, TFT_GREEN, TFT_GREEN},
  // 更多帧...
};

const uint16_t video2[][4] = {
  {TFT_YELLOW, TFT_BLACK, TFT_BLACK, TFT_YELLOW},
  {TFT_BLACK, TFT_YELLOW, TFT_YELLOW, TFT_BLACK},
  {TFT_YELLOW, TFT_BLACK, TFT_BLACK, TFT_YELLOW},
  // 更多帧...
};

// 视频列表
const uint16_t* videos[] = {video1[0], video2[0]};
const int videoLengths[] = {sizeof(video1)/sizeof(video1[0]), sizeof(video2)/sizeof(video2[0])};

void setup() {
  Serial.begin(115200);
  
  // 初始化屏幕
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
  // 创建WiFi接入点
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  
  // 显示连接信息
  tft.drawString("Connect to WiFi:", 10, 10, 2);
  tft.drawString(ssid, 10, 30, 2);
  tft.drawString("Password: " + String(password), 10, 50, 2);
  tft.drawString("IP: " + IP.toString(), 10, 70, 2);
  tft.drawString("Open: http://" + IP.toString(), 10, 90, 2);
  
  // 设置Web服务器路由
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>body {font-family: Arial; text-align: center;}";
    html += ".button {background-color: #4CAF50; border: none; color: white; padding: 15px 32px;";
    html += "text-align: center; text-decoration: none; display: inline-block; font-size: 16px;";
    html += "margin: 4px 2px; cursor: pointer;}</style></head><body>";
    html += "<h1>ESP32 Video Player Control</h1>";
    html += "<p><a href='/video/1'><button class='button'>Play Video 1</button></a></p>";
    html += "<p><a href='/video/2'><button class='button'>Play Video 2</button></a></p>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });
  
  server.on("/video/1", HTTP_GET, [](AsyncWebServerRequest *request){
    currentVideo = 0;
    request->send(200, "text/plain", "Playing Video 1");
  });
  
  server.on("/video/2", HTTP_GET, [](AsyncWebServerRequest *request){
    currentVideo = 1;
    request->send(200, "text/plain", "Playing Video 2");
  });
  
  // 启动服务器
  server.begin();
}

void loop() {
  // 播放当前选中的视频
  playVideo(currentVideo);
}

void playVideo(int videoIndex) {
  const uint16_t* video = videos[videoIndex];
  int length = videoLengths[videoIndex];
  
  for(int i = 0; i < length; i++) {
    // 检查是否有新的网络请求
    // 这里简化处理，实际应用中可能需要更复杂的中断处理
    
    // 绘制视频帧（这里简化处理，实际需要根据视频分辨率绘制）
    tft.fillRect(0, 0, 240, 240, video[i*4]);
    tft.fillRect(80, 80, 80, 80, video[i*4+1]);
    
    // 模拟视频播放速度
    delay(100);
  }
}