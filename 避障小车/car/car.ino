// ARDUINO OBSTACLE AVOIDING CAR - 强制转向修复版
#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

// 引脚定义
#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 

// 速度参数 - 调整为更高
#define MAX_SPEED 250      // 最大速度
#define START_SPEED 200    // 启动速度
#define MIN_SPEED 180      // 最低运行速度
#define TURN_SPEED 200     // 转向速度

// 避障参数
#define OBSTACLE_DISTANCE 20  // 障碍物检测距离
#define SAFE_DISTANCE 30      // 安全距离
#define CRITICAL_DISTANCE 10  // 紧急停止距离

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// 电机初始化 - 尝试不同的频率
AF_DCMotor motor1(1, MOTOR12_64KHZ);  // 使用64KHz提高响应
AF_DCMotor motor2(2, MOTOR12_64KHZ);
AF_DCMotor motor3(3, MOTOR34_64KHZ);  // 统一使用64KHz
AF_DCMotor motor4(4, MOTOR34_64KHZ);

Servo myservo;

// 状态变量
boolean goesForward = false;
int distance = 100;
int avoidState = 0;  // 0=正常，1=避障中

void setup() {
  Serial.begin(9600);
  Serial.println("小车初始化...");
  
  // 电源稳定等待
  delay(3000);
  
  // 舵机初始化
  myservo.attach(10);  
  myservo.write(90);
  delay(500);
  
  // 强制电机测试
  Serial.println("=== 电机强制测试 ===");
  testMotors();
  
  // 初始距离测量
  distance = readPing();
  Serial.print("初始距离: ");
  Serial.println(distance);
  
  Serial.println("初始化完成！");
  Serial.println("=====================");
}

// 更彻底的电机测试
void testMotors() {
  Serial.println("1. 前进测试");
  setAllMotors(FORWARD, 200);
  delay(500);
  setAllMotors(RELEASE, 0);
  delay(300);
  
  Serial.println("2. 后退测试");
  setAllMotors(BACKWARD, 200);
  delay(500);
  setAllMotors(RELEASE, 0);
  delay(300);
  
  Serial.println("3. 右转测试");
  // 右转：左边前进，右边后退
  motor1.run(FORWARD);
  motor1.setSpeed(200);
  motor2.run(FORWARD);
  motor2.setSpeed(200);
  motor3.run(BACKWARD);
  motor3.setSpeed(200);
  motor4.run(BACKWARD);
  motor4.setSpeed(200);
  delay(500);
  moveStop();
  delay(300);
  
  Serial.println("4. 左转测试");
  // 左转：右边前进，左边后退
  motor1.run(BACKWARD);
  motor1.setSpeed(200);
  motor2.run(BACKWARD);
  motor2.setSpeed(200);
  motor3.run(FORWARD);
  motor3.setSpeed(200);
  motor4.run(FORWARD);
  motor4.setSpeed(200);
  delay(500);
  moveStop();
  delay(500);
  
  Serial.println("电机测试完成！");
}

void loop() {
  distance = readPing();
  
  Serial.print("距离: ");
  Serial.print(distance);
  Serial.print("cm | 状态: ");
  
  // 如果正在避障，先完成避障动作
  if (avoidState == 1) {
    Serial.println("避障中...");
    delay(100);
    return;
  }
  
  // 紧急停止
  if(distance <= CRITICAL_DISTANCE) {
    Serial.println("紧急停止！");
    emergencyStop();
    avoidObstacle();
  }
  // 检测到障碍物
  else if(distance <= OBSTACLE_DISTANCE) {
    Serial.println("障碍物！");
    avoidObstacle();
  }
  // 正常前进
  else {
    Serial.println("前进");
    moveForward();
  }
  
  delay(50);
}

// 避障函数 - 简化版，强制完成避障
void avoidObstacle() {
  avoidState = 1;  // 标记为避障状态
  
  // 立即停止
  moveStop();
  delay(200);
  
  // 简短后退
  Serial.println("后退...");
  moveBackward();
  delay(300);
  moveStop();
  delay(200);
  
  // 查看左右
  Serial.println("查看右边...");
  int distanceR = lookRight();
  Serial.print("右边距离: ");
  Serial.println(distanceR);
  
  delay(200);
  
  Serial.println("查看左边...");
  int distanceL = lookLeft();
  Serial.print("左边距离: ");
  Serial.println(distanceL);
  
  delay(200);
  
  // 决策：选择更远的一侧
  if (distanceR > distanceL) {
    Serial.println(">>> 执行右转 <<<");
    executeTurnRight();
  } else {
    Serial.println(">>> 执行左转 <<<");
    executeTurnLeft();
  }
  
  // 避障后前进一小段
  Serial.println("避障完成，继续前进");
  moveForward();
  delay(500);
  
  avoidState = 0;  // 清除避障状态
}

// 执行右转 - 强制完成
void executeTurnRight() {
  Serial.println("开始右转...");
  
  // 设置转向速度
  motor1.setSpeed(TURN_SPEED);
  motor2.setSpeed(TURN_SPEED);
  motor3.setSpeed(TURN_SPEED);
  motor4.setSpeed(TURN_SPEED);
  
  // 右转控制：左侧轮前进，右侧轮后退
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  
  // 保持转向足够长时间
  unsigned long startTime = millis();
  while (millis() - startTime < 600) {  // 强制转向600ms
    // 在转向过程中可以检查是否撞到新障碍物
    int currentDist = readPing();
    if (currentDist < CRITICAL_DISTANCE) {
      Serial.println("转向中遇到障碍，停止转向");
      break;
    }
    delay(50);
  }
  
  moveStop();
  delay(100);
  
  // 转向后稍微前进一点
  setAllMotors(FORWARD, 180);
  delay(300);
  
  goesForward = true;
  Serial.println("右转完成");
}

// 执行左转 - 强制完成
void executeTurnLeft() {
  Serial.println("开始左转...");
  
  // 设置转向速度
  motor1.setSpeed(TURN_SPEED);
  motor2.setSpeed(TURN_SPEED);
  motor3.setSpeed(TURN_SPEED);
  motor4.setSpeed(TURN_SPEED);
  
  // 左转控制：右侧轮前进，左侧轮后退
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
  // 保持转向足够长时间
  unsigned long startTime = millis();
  while (millis() - startTime < 600) {  // 强制转向600ms
    // 在转向过程中可以检查是否撞到新障碍物
    int currentDist = readPing();
    if (currentDist < CRITICAL_DISTANCE) {
      Serial.println("转向中遇到障碍，停止转向");
      break;
    }
    delay(50);
  }
  
  moveStop();
  delay(100);
  
  // 转向后稍微前进一点
  setAllMotors(FORWARD, 180);
  delay(300);
  
  goesForward = true;
  Serial.println("左转完成");
}

// 紧急停止
void emergencyStop() {
  Serial.println("!!! 紧急停止 !!!");
  setAllMotors(BACKWARD, MAX_SPEED);
  delay(200);
  moveStop();
  delay(300);
}

int lookRight() {
  myservo.write(30);
  delay(600);
  int distance = readPing();
  delay(100);
  myservo.write(90);
  delay(200);
  return distance;
}

int lookLeft() {
  myservo.write(150);
  delay(600);
  int distance = readPing();
  delay(100);
  myservo.write(90);
  delay(200);
  return distance;
}

int readPing() { 
  // 简单读取
  delay(50);
  int cm = sonar.ping_cm();
  if(cm == 0) {
    cm = 250;
  }
  return cm;
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
}

void moveStop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  goesForward = false;
}

void moveForward() {
  if(!goesForward) {
    goesForward = true;
    // 高扭矩启动
    setAllMotors(FORWARD, START_SPEED);
    delay(200);
  }
  // 保持前进
  setAllMotors(FORWARD, MIN_SPEED);
}

void moveBackward() {
  goesForward = false;
  setAllMotors(BACKWARD, START_SPEED);
  delay(200);
  setAllMotors(BACKWARD, MIN_SPEED);
}