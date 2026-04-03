// ARDUINO OBSTACLE AVOIDING CAR - 高级优化版
#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

// 引脚定义
#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 

// 速度参数 - 动态调整
#define MAX_SPEED 200      // 增加最大速度
#define CRUISE_SPEED 180   // 巡航速度
#define SLOW_SPEED 130     // 减速速度
#define TURN_SPEED_HIGH 180 // 快速转向
#define TURN_SPEED_LOW 150  // 慢速转向

// 避障参数 - 动态调整
#define OBSTACLE_DISTANCE_FAST 25  // 高速时检测距离
#define OBSTACLE_DISTANCE_SLOW 15  // 慢速时检测距离
#define SAFE_DISTANCE 35           // 增加安全距离
#define CRITICAL_DISTANCE 8        // 更短的紧急停止距离

// 扫描参数优化
#define SCAN_ANGLE_RIGHT 45        // 减小扫描角度，加快响应
#define SCAN_ANGLE_LEFT 135
#define SCAN_DELAY 150             // 减少扫描延迟

// 行为参数
#define BACKUP_TIME 200           // 减少后退时间
#define TURN_TIME_FAST 300        // 快速转向时间
#define TURN_TIME_SLOW 450        // 慢速转向时间
#define PROACTIVE_SCAN_INTERVAL 1000 // 主动扫描间隔

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// 电机初始化
AF_DCMotor motor1(1, MOTOR12_8KHZ);
AF_DCMotor motor2(2, MOTOR12_8KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

Servo myservo;

// 状态变量
bool goesForward = false;
bool isTurning = false;
int currentSpeed = CRUISE_SPEED;
int lastDistance = 100;
int lastTurnDirection = 0; // 0=未记录, 1=右转, -1=左转
unsigned long lastScanTime = 0;
unsigned long lastObstacleTime = 0;

// 距离历史记录
int distanceHistory[3] = {100, 100, 100};
int historyIndex = 0;

void setup() {
  Serial.begin(115200);  // 提高串口速率
  
  delay(1000);  // 减少等待时间
  
  // 舵机初始化
  myservo.attach(10);  
  myservo.write(90);
  delay(200);  // 减少等待时间
  
  // 快速电机测试
  testMotors();
  
  // 初始距离测量 - 修改这里：使用 readPingQuick() 替代 readPing()
  for(int i = 0; i < 2; i++) {  // 减少测量次数
    lastDistance = readPingQuick();  // 改为调用 readPingQuick()
    distanceHistory[i] = lastDistance;
    delay(50);
  }
  
  Serial.println("小车初始化完成 - 高级优化版");
}

void testMotors() {
  Serial.println("快速电机测试...");
  
  // 快速测试
  setAllMotors(FORWARD, 150);
  delay(100);
  setAllMotors(RELEASE, 0);
  delay(50);
}

void loop() {
  unsigned long currentTime = millis();
  
  // 更新距离历史
  updateDistanceHistory();
  
  // 计算距离变化趋势
  int trend = calculateDistanceTrend();
  
  // 读取前方距离
  int currentDistance = readPingQuick();
  
  // 动态调整检测距离
  int obstacleDistance = (currentSpeed >= CRUISE_SPEED) ? 
                         OBSTACLE_DISTANCE_FAST : 
                         OBSTACLE_DISTANCE_SLOW;
  
  Serial.print("距离: ");
  Serial.print(currentDistance);
  Serial.print("cm | 速度: ");
  Serial.print(currentSpeed);
  Serial.print(" | 趋势: ");
  Serial.println(trend);
  
  // 如果正在转向，优先完成转向
  if (isTurning) {
    return;
  }
  
  // 紧急情况：非常近的障碍物
  if (currentDistance <= CRITICAL_DISTANCE) {
    emergencyManeuver();
  }
  // 接近障碍物：预测性减速
  else if (currentDistance <= obstacleDistance || trend < -5) {
    if (currentDistance <= obstacleDistance * 0.7) {
      avoidObstacleProactive();
    } else {
      avoidObstacleSmooth();
    }
  }
  // 安全距离：正常行驶
  else {
    moveForwardOptimized(currentDistance);
  }
  
  // 主动扫描（周期性）
  if (currentTime - lastScanTime > PROACTIVE_SCAN_INTERVAL && 
      currentDistance > SAFE_DISTANCE) {
    proactiveScan();
    lastScanTime = currentTime;
  }
  
  delay(30);  // 减少主循环延时
}

void updateDistanceHistory() {
  distanceHistory[historyIndex] = readPingQuick();
  historyIndex = (historyIndex + 1) % 3;
}

int calculateDistanceTrend() {
  int sum = 0;
  for (int i = 0; i < 2; i++) {
    sum += distanceHistory[(historyIndex + i) % 3] - 
           distanceHistory[(historyIndex + i + 1) % 3];
  }
  return sum / 2;
}

void moveForwardOptimized(int distance) {
  // 根据距离动态调整速度
  if (distance > SAFE_DISTANCE * 2) {
    currentSpeed = MAX_SPEED;
  } else if (distance > SAFE_DISTANCE * 1.5) {
    currentSpeed = CRUISE_SPEED;
  } else {
    currentSpeed = SLOW_SPEED;
  }
  
  if (!goesForward) {
    goesForward = true;
    setAllMotors(FORWARD, currentSpeed);
  } else {
    // 平滑调整速度
    setAllMotors(FORWARD, currentSpeed);
  }
}

void avoidObstacleSmooth() {
  Serial.println("平滑避障");
  
  // 减速但不完全停止
  currentSpeed = SLOW_SPEED;
  setAllMotors(FORWARD, currentSpeed);
  delay(100);
  
  // 快速扫描
  int distanceR = lookRightQuick();
  int distanceL = lookLeftQuick();
  
  // 决策逻辑优化
  if (distanceR > SAFE_DISTANCE && distanceL > SAFE_DISTANCE) {
    // 两边都安全，选择更优方向
    if (distanceR - distanceL > 10) {
      turnRightSmooth();
    } else if (distanceL - distanceR > 10) {
      turnLeftSmooth();
    } else {
      // 随机选择或根据历史
      (lastTurnDirection <= 0) ? turnRightSmooth() : turnLeftSmooth();
    }
  } else if (distanceR > SAFE_DISTANCE) {
    turnRightSmooth();
  } else if (distanceL > SAFE_DISTANCE) {
    turnLeftSmooth();
  } else {
    emergencyManeuver();
  }
}

void avoidObstacleProactive() {
  Serial.println("主动避障");
  
  moveStop();
  delay(50);
  
  int distanceR = lookRightQuick();
  int distanceL = lookLeftQuick();
  
  // 快速决策
  int turnDecision = 0; // 0=直行, 1=右转, -1=左转
  
  if (distanceR >= SAFE_DISTANCE || distanceL >= SAFE_DISTANCE) {
    if (distanceR >= distanceL) {
      turnDecision = 1;
    } else {
      turnDecision = -1;
    }
    
    // 执行转向
    if (turnDecision == 1) {
      turnRightQuick();
    } else {
      turnLeftQuick();
    }
    
    lastTurnDirection = turnDecision;
  } else {
    // 后退并大角度转向
    moveBackwardQuick();
    delay(BACKUP_TIME);
    turnAroundQuick();
  }
}

void emergencyManeuver() {
  Serial.println("紧急机动！");
  
  // 急停
  setAllMotors(BACKWARD, MAX_SPEED);
  delay(80);
  moveStop();
  delay(50);
  
  // 快速后退
  moveBackwardQuick();
  delay(BACKUP_TIME);
  
  // 快速扫描并转向
  int distanceR = lookRightQuick();
  int distanceL = lookLeftQuick();
  
  if (distanceR > distanceL && distanceR > CRITICAL_DISTANCE * 2) {
    turnRightQuick();
  } else if (distanceL > CRITICAL_DISTANCE * 2) {
    turnLeftQuick();
  } else {
    turnAroundQuick();
  }
  
  lastObstacleTime = millis();
}

void proactiveScan() {
  int centerDist = readPingQuick();
  
  if (centerDist > SAFE_DISTANCE * 1.5) {
    // 随机扫描左右
    int scanAngle = (random(0, 2) == 0) ? SCAN_ANGLE_RIGHT : SCAN_ANGLE_LEFT;
    myservo.write(scanAngle);
    delay(100);
    int sideDist = readPingQuick();
    myservo.write(90);
    delay(100);
    
    // 如果侧边有更好路径，轻微调整方向
    if (sideDist > centerDist + 20) {
      if (scanAngle == SCAN_ANGLE_RIGHT) {
        slightRight();
      } else {
        slightLeft();
      }
      delay(100);
    }
  }
}

// 快速扫描函数
int lookRightQuick() {
  myservo.write(SCAN_ANGLE_RIGHT);
  delay(SCAN_DELAY);
  int distance = readPingQuick();
  myservo.write(90);
  delay(100);
  return distance;
}

int lookLeftQuick() {
  myservo.write(SCAN_ANGLE_LEFT);
  delay(SCAN_DELAY);
  int distance = readPingQuick();
  myservo.write(90);
  delay(100);
  return distance;
}

// 快速测距
int readPingQuick() {
  int cm = sonar.ping_cm();
  return (cm > 0 && cm < MAX_DISTANCE) ? cm : MAX_DISTANCE;
}

// 平滑转向函数
void turnRightSmooth() {
  isTurning = true;
  Serial.println("平滑右转");
  
  // 差速转向：右侧慢，左侧快
  motor1.setSpeed(TURN_SPEED_LOW);
  motor2.setSpeed(TURN_SPEED_LOW);
  motor3.setSpeed(TURN_SPEED_HIGH);
  motor4.setSpeed(TURN_SPEED_HIGH);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
  delay(TURN_TIME_FAST);
  moveStop();
  isTurning = false;
  lastTurnDirection = 1;
}

void turnLeftSmooth() {
  isTurning = true;
  Serial.println("平滑左转");
  
  motor1.setSpeed(TURN_SPEED_HIGH);
  motor2.setSpeed(TURN_SPEED_HIGH);
  motor3.setSpeed(TURN_SPEED_LOW);
  motor4.setSpeed(TURN_SPEED_LOW);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
  delay(TURN_TIME_FAST);
  moveStop();
  isTurning = false;
  lastTurnDirection = -1;
}

// 快速转向函数
void turnRightQuick() {
  isTurning = true;
  Serial.println("快速右转");
  
  motor1.setSpeed(TURN_SPEED_HIGH);
  motor2.setSpeed(TURN_SPEED_HIGH);
  motor3.setSpeed(TURN_SPEED_HIGH);
  motor4.setSpeed(TURN_SPEED_HIGH);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  
  delay(TURN_TIME_FAST);
  moveStop();
  delay(50);
  isTurning = false;
}

void turnLeftQuick() {
  isTurning = true;
  Serial.println("快速左转");
  
  motor1.setSpeed(TURN_SPEED_HIGH);
  motor2.setSpeed(TURN_SPEED_HIGH);
  motor3.setSpeed(TURN_SPEED_HIGH);
  motor4.setSpeed(TURN_SPEED_HIGH);
  
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
  delay(TURN_TIME_FAST);
  moveStop();
  delay(50);
  isTurning = false;
}

void turnAroundQuick() {
  Serial.println("快速掉头");
  
  // 原地旋转
  motor1.setSpeed(TURN_SPEED_HIGH);
  motor2.setSpeed(TURN_SPEED_HIGH);
  motor3.setSpeed(TURN_SPEED_HIGH);
  motor4.setSpeed(TURN_SPEED_HIGH);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  
  delay(TURN_TIME_SLOW * 1.5);
  moveStop();
  lastTurnDirection = 0;
}

// 轻微调整方向
void slightRight() {
  motor1.setSpeed(CRUISE_SPEED - 40);
  motor2.setSpeed(CRUISE_SPEED - 40);
  motor3.setSpeed(CRUISE_SPEED);
  motor4.setSpeed(CRUISE_SPEED);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void slightLeft() {
  motor1.setSpeed(CRUISE_SPEED);
  motor2.setSpeed(CRUISE_SPEED);
  motor3.setSpeed(CRUISE_SPEED - 40);
  motor4.setSpeed(CRUISE_SPEED - 40);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveBackwardQuick() {
  goesForward = false;
  setAllMotors(BACKWARD, CRUISE_SPEED);
}

void moveStop() {
  setAllMotors(RELEASE, 0);
  goesForward = false;
  currentSpeed = 0;
}

// 设置所有电机
void setAllMotors(int dir, int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
  
  motor1.run(dir);
  motor2.run(dir);
  motor3.run(dir);
  motor4.run(dir);
  
  if (dir == FORWARD) {
    currentSpeed = speed;
  }
}