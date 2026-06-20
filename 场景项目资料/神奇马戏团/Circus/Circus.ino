#include <Servo.h>
#include <FastLED.h>                     // WS2812 控制库

// ===== 引脚定义 =====
const int PIN_LION_SERVO = 10;   // 狮子舵机 (180度)
const int PIN_GEAR_SERVO = 11;   // 齿轮舵机 (360度)
const int PIN_STAGE_SERVO = 9;   // 舞台舵机 (360度)
const int PIN_LIFT_SERVO = 5;    // 升降舵机 (180度)
const int PIN_BG_LED = 6;         // 背景灯带 (WS2812)

// ===== LED 配置 =====
#define NUM_BG_LEDS 30            // 背景灯带 LED 数量
CRGB bgLeds[NUM_BG_LEDS];         // LED 数组

// ===== 背景灯光参数 =====
uint8_t bgHue = 0;                // 当前色调 (0-255)
uint8_t bgBrightness = 128;       // 当前亮度
bool breathingUp = true;           // 呼吸方向：true=变亮，false=变暗
const uint8_t BREATH_STEP = 2;     // 呼吸每步变化量（速度）
const uint8_t HUE_STEP = 1;        // 色调每步变化量（速度）
const uint8_t MIN_BRIGHT = 50;     // 最小亮度
const uint8_t MAX_BRIGHT = 255;    // 最大亮度
unsigned long lastLEDUpdate = 0;
const unsigned long LED_UPDATE_INTERVAL = 20; // 20ms 更新一次

// ===== 狮子模块参数 =====
int lionCurrentAngle = 90;
int lionTargetAngle = 90;
int lionStartAngle = 40;
int lionEndAngle = 140;
int lionSpeedDelay = 15;
bool lionIsJumping = false;
unsigned long lionLastStepTime = 0;

// ===== 齿轮模块参数 =====
const int GEAR_SPEED = 89;        // 顺时针中等速度

// ===== 舞台模块参数 =====
const int STAGE_SPEED = 100;      // 逆时针速度

// ===== 升降模块参数（简化版）=====
int liftCurrentAngle = 0;           // 当前位置
const int liftLowAngle = 0;         // 低角度（关门）
const int liftHighAngle = 80;        // 高角度（开门）
bool liftWaiting = false;           // 等待标志
unsigned long liftWaitStart = 0;    // 等待开始时间
const unsigned long liftChangeDelay = 1000; // 等待时间（毫秒）
bool liftAutoMode = true;           // 自动模式开关

// 舵机对象
Servo lionServo;
Servo gearServo;
Servo stageServo;
Servo liftServo;

// 定期刷新舵机速度的间隔（防止意外停止）
const unsigned long KEEP_ALIVE_INTERVAL = 1000;
unsigned long lastServoRefresh = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("狮子 + 齿轮 + 舞台 + 升降 + 背景灯 模块启动");

  // 舵机连接
  lionServo.attach(PIN_LION_SERVO);
  gearServo.attach(PIN_GEAR_SERVO);
  stageServo.attach(PIN_STAGE_SERVO);
  liftServo.attach(PIN_LIFT_SERVO);

  // 初始化位置
  lionServo.write(lionCurrentAngle);
  liftServo.write(liftCurrentAngle);   // 初始位置 0°

  // 设置齿轮持续旋转
  gearServo.write(GEAR_SPEED);
  Serial.print("齿轮旋转速度设置为: ");
  Serial.println(GEAR_SPEED);

  // 设置舞台持续旋转
  stageServo.write(STAGE_SPEED);
  Serial.print("舞台旋转速度设置为: ");
  Serial.println(STAGE_SPEED);

  Serial.println("齿轮和舞台开始持续旋转");
  Serial.println("升降模块初始化完成，自动模式开启（0°<->80°循环）");

  // 初始化 WS2812 灯带
  FastLED.addLeds<WS2812, PIN_BG_LED, GRB>(bgLeds, NUM_BG_LEDS);
  FastLED.setBrightness(MAX_BRIGHT);
  fill_solid(bgLeds, NUM_BG_LEDS, CRGB::Black);
  FastLED.show();
  Serial.println("背景灯带初始化完成，彩虹呼吸效果启动");
}

void loop() {
  unsigned long currentMillis = millis();

  // 定期刷新齿轮和舞台的速度（保持旋转）
  if (currentMillis - lastServoRefresh >= KEEP_ALIVE_INTERVAL) {
    lastServoRefresh = currentMillis;
    gearServo.write(GEAR_SPEED);
    stageServo.write(STAGE_SPEED);
  }

  // 处理狮子模块的非阻塞移动
  updateLionJump();

  // 处理升降模块的非阻塞移动（简单开关模式）
  updateLiftModule(currentMillis);

  // 处理背景灯带（彩虹呼吸效果）
  updateBackgroundLED(currentMillis);

  // 可选：串口命令控制狮子（需取消注释下方函数）
  // handleSerialCommands();

  // 可选：串口命令控制升降（如需启用，取消下一行注释）
  // handleLiftCommands();
}

// ==================== 狮子模块函数 ====================
void updateLionJump() {
  if (!lionIsJumping) {
    static unsigned long lastAutoJump = 0;
    if (millis() - lastAutoJump > 3000) {
      lastAutoJump = millis();
      startLionJump();
    }
    return;
  }

  if (millis() - lionLastStepTime >= lionSpeedDelay) {
    lionLastStepTime = millis();

    if (lionCurrentAngle < lionTargetAngle) lionCurrentAngle++;
    else if (lionCurrentAngle > lionTargetAngle) lionCurrentAngle--;

    lionServo.write(lionCurrentAngle);

    if (lionCurrentAngle == lionTargetAngle) {
      if (lionTargetAngle == lionEndAngle) {
        lionTargetAngle = lionStartAngle;
        Serial.println("狮子跳过火圈！");
      } else {
        lionIsJumping = false;
        Serial.println("狮子回到起始位置");
      }
    }
  }
}

void startLionJump() {
  if (!lionIsJumping) {
    lionIsJumping = true;
    lionTargetAngle = lionEndAngle;
    Serial.println("狮子开始跳火圈！");
  }
}

void setLionParams(int start, int end, int speed) {
  lionStartAngle = constrain(start, 0, 180);
  lionEndAngle = constrain(end, 0, 180);
  lionSpeedDelay = map(speed, 1, 100, 50, 5);
  Serial.print("狮子参数更新: 起始=");
  Serial.print(lionStartAngle);
  Serial.print(" 结束=");
  Serial.print(lionEndAngle);
  Serial.print(" 速度=");
  Serial.println(speed);
}

// ==================== 升降模块函数（简单开关模式）====================
void updateLiftModule(unsigned long now) {
  if (!liftAutoMode) return; // 手动模式不动作

  if (!liftWaiting) {
    // 切换目标
    if (liftCurrentAngle == liftLowAngle) {
      liftServo.write(liftHighAngle);
      liftCurrentAngle = liftHighAngle;
      Serial.println("升降上升至 80°");
    } else {
      liftServo.write(liftLowAngle);
      liftCurrentAngle = liftLowAngle;
      Serial.println("升降下降至 0°");
    }
    liftWaiting = true;
    liftWaitStart = now;
  } else {
    // 等待时间到则允许下次切换
    if (now - liftWaitStart >= liftChangeDelay) {
      liftWaiting = false;
    }
  }
}

// 切换自动模式
void toggleLiftAuto() {
  liftAutoMode = !liftAutoMode;
  Serial.print("升降自动模式: ");
  Serial.println(liftAutoMode ? "开启" : "关闭");
  if (!liftAutoMode) {
    liftWaiting = false; // 退出等待状态
  }
}

// 手动设置目标角度（0° 或 80°），若输入其他值则约束到最近端点
void setLiftTarget(int target) {
  if (target <= (liftLowAngle + liftHighAngle) / 2) {
    target = liftLowAngle;
  } else {
    target = liftHighAngle;
  }
  liftServo.write(target);
  liftCurrentAngle = target;
  Serial.print("升降手动设置到: ");
  Serial.println(target);
  liftWaiting = true;      // 手动设置后也进入等待，避免立即切换
  liftWaitStart = millis();
}

// ==================== 背景灯光模块（彩虹呼吸）====================
void updateBackgroundLED(unsigned long now) {
  if (now - lastLEDUpdate < LED_UPDATE_INTERVAL) return;
  lastLEDUpdate = now;

  // 呼吸亮度变化
  if (breathingUp) {
    bgBrightness += BREATH_STEP;
    if (bgBrightness >= MAX_BRIGHT) {
      bgBrightness = MAX_BRIGHT;
      breathingUp = false;
    }
  } else {
    bgBrightness -= BREATH_STEP;
    if (bgBrightness <= MIN_BRIGHT) {
      bgBrightness = MIN_BRIGHT;
      breathingUp = true;
    }
  }

  // 颜色缓慢变化
  bgHue += HUE_STEP;

  // 彩虹效果
  fill_rainbow(bgLeds, NUM_BG_LEDS, bgHue, 10);

  // 设置亮度
  FastLED.setBrightness(bgBrightness);
  FastLED.show();
}

// ==================== 串口命令处理（可选）====================
void handleLiftCommands() {
  if (!Serial.available()) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();

  if (cmd.startsWith("lift_to ")) {
    int val = cmd.substring(7).toInt();
    setLiftTarget(val);
  }
  else if (cmd == "lift_auto") {
    toggleLiftAuto();
  }
  else if (cmd == "lift_help") {
    Serial.println("升降命令: lift_to [角度(0或80)], lift_auto");
  }
}

void handleSerialCommands() {
  if (!Serial.available()) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();

  if (cmd.startsWith("start ")) {
    int val = cmd.substring(6).toInt();
    lionStartAngle = constrain(val, 0, 180);
    Serial.print("起始角度=");
    Serial.println(lionStartAngle);
  }
  else if (cmd.startsWith("end ")) {
    int val = cmd.substring(4).toInt();
    lionEndAngle = constrain(val, 0, 180);
    Serial.print("结束角度=");
    Serial.println(lionEndAngle);
  }
  else if (cmd.startsWith("speed ")) {
    int val = cmd.substring(6).toInt();
    val = constrain(val, 1, 100);
    lionSpeedDelay = map(val, 1, 100, 50, 5);
    Serial.print("速度延迟=");
    Serial.println(lionSpeedDelay);
  }
  else if (cmd == "jump") {
    startLionJump();
  }
  else if (cmd == "help") {
    Serial.println("可用命令: start [角度], end [角度], speed [1-100], jump, help");
  }
}