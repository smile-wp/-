// ARDUINO OBSTACLE AVOIDING CAR - 智能优化版
#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

// 引脚定义
#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 

// 速度参数
#define MAX_SPEED 190
#define START_SPEED 180
#define MIN_SPEED 150
#define TURN_SPEED 170  // 转向速度

// 避障参数
#define OBSTACLE_DISTANCE 20  // 障碍物检测距离（厘米）
#define SAFE_DISTANCE 30      // 安全距离
#define CRITICAL_DISTANCE 10  // 紧急停止距离

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// 电机初始化
AF_DCMotor motor1(1, MOTOR12_8KHZ);
AF_DCMotor motor2(2, MOTOR12_8KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

Servo myservo;

// 状态变量
boolean goesForward = false;
int distance = 100;
int speedSet = 0;
int avoidManeuver = 0;  // 避障策略状态

void setup() {
  // 初始化串口调试（可选）
  Serial.begin(9600);
  
  // 电源稳定等待
  delay(2000);
  
  // 舵机初始化
  myservo.attach(10);  
  myservo.write(90);  // 改为90度居中位置
  delay(1000);
  
  // 电机预测试
  testMotors();
  
  // 初始距离测量
  for(int i = 0; i < 3; i++) {
    distance = readPing();
    delay(100);
  }
  
  Serial.println("小车初始化完成！");
}

void testMotors() {
  Serial.println("电机测试中...");
  
  // 前转测试
  setAllMotors(FORWARD, 150);
  delay(200);
  setAllMotors(RELEASE, 0);
  delay(100);
  
  // 后转测试
  setAllMotors(BACKWARD, 150);
  delay(200);
  setAllMotors(RELEASE, 0);
  delay(200);
  
  Serial.println("电机测试完成");
}

void loop() {
  // 持续读取前方距离
  distance = readPing();
  
  Serial.print("前方距离: ");
  Serial.print(distance);
  Serial.print(" cm | 状态: ");
  
  // 紧急情况：太近了！
  if(distance <= CRITICAL_DISTANCE) {
    Serial.println("紧急停止！");
    emergencyStop();
    delay(200);
    moveBackward();
    delay(500);
    avoidObstacle();
  }
  // 一般障碍物
  else if(distance <= OBSTACLE_DISTANCE) {
    Serial.println("检测到障碍物，准备避障");
    moveStop();
    delay(100);
    avoidObstacle();
  }
  // 安全距离，正常前进
  else {
    Serial.println("正常前进");
    moveForward();
  }
  
  delay(50);  // 主循环延时
}

// 智能避障决策
void avoidObstacle() {
  moveStop();
  delay(200);
  
  // 先稍微后退
  moveBackward();
  delay(300);
  moveStop();
  delay(200);
  
  // 查看右侧距离
  int distanceR = lookRight();
  Serial.print("右侧距离: ");
  Serial.println(distanceR);
  
  // 查看左侧距离
  int distanceL = lookLeft();
  Serial.print("左侧距离: ");
  Serial.println(distanceL);
  
  delay(300);
  
  // 智能决策逻辑
  if(distanceR >= SAFE_DISTANCE && distanceL >= SAFE_DISTANCE) {
    // 两边都安全，选择距离更远的一侧
    if(distanceR > distanceL) {
      Serial.println("两边都安全，选择右侧（距离更远）");
      turnRight();
    } else {
      Serial.println("两边都安全，选择左侧（距离更远）");
      turnLeft();
    }
  }
  else if(distanceR >= SAFE_DISTANCE) {
    // 只有右侧安全
    Serial.println("右侧安全，向右转");
    turnRight();
  }
  else if(distanceL >= SAFE_DISTANCE) {
    // 只有左侧安全
    Serial.println("左侧安全，向左转");
    turnLeft();
  }
  else {
    // 两边都不安全，180度掉头
    Serial.println("两边都不安全，掉头");
    turnAround();
  }
  
  // 避障后继续前进
  delay(300);
  moveForward();
}

// 紧急停止
void emergencyStop() {
  setAllMotors(BACKWARD, MAX_SPEED);
  delay(100);
  moveStop();
}

// 掉头（180度）
void turnAround() {
  Serial.println("执行180度掉头");
  
  // 向右转180度（调整时间以达到180度）
  motor1.setSpeed(TURN_SPEED);
  motor2.setSpeed(TURN_SPEED);
  motor3.setSpeed(TURN_SPEED);
  motor4.setSpeed(TURN_SPEED);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  
  delay(1000);  // 调整这个时间以获得180度转弯
  
  moveStop();
  goesForward = true;
}

int lookRight() {
  myservo.write(30);  // 向右看（0-90度）
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(90);  // 回到中间
  delay(200);
  return distance;
}

int lookLeft() {
  myservo.write(150);  // 向左看（90-180度）
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(90);  // 回到中间
  delay(200);
  return distance;
}

int readPing() { 
  // 多次读取取平均值，提高准确性
  int total = 0;
  int validReadings = 0;
  
  for(int i = 0; i < 3; i++) {
    delay(40);
    int cm = sonar.ping_cm();
    
    // 过滤无效读数（0或太大）
    if(cm > 0 && cm < MAX_DISTANCE) {
      total += cm;
      validReadings++;
    }
  }
  
  if(validReadings > 0) {
    return total / validReadings;
  } else {
    return MAX_DISTANCE;  // 没有有效读数，返回最大距离
  }
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
  setAllMotors(RELEASE, 0);
  goesForward = false;
}

void moveForward() {
  if(!goesForward) {
    goesForward = true;
    
    // 高扭矩启动
    setAllMotors(FORWARD, START_SPEED);
    delay(150);
    
    // 调整到运行速度
    for (speedSet = START_SPEED; speedSet >= MIN_SPEED; speedSet -= 5) {
      setAllMotors(FORWARD, speedSet);
      delay(10);
    }
  }
  
  // 保持前进
  setAllMotors(FORWARD, MIN_SPEED);
}

void moveBackward() {
  goesForward = false;
  
  // 后退
  setAllMotors(BACKWARD, START_SPEED);
  delay(200);
  
  // 调整后退速度
  setAllMotors(BACKWARD, MIN_SPEED);
}

void turnRight() {
  Serial.println("向右转");
  
  motor1.setSpeed(TURN_SPEED);
  motor2.setSpeed(TURN_SPEED);
  motor3.setSpeed(TURN_SPEED);
  motor4.setSpeed(TURN_SPEED);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  
  delay(400);  // 转弯时间，根据实际情况调整
  
  moveStop();
  goesForward = true;
}

void turnLeft() {
  Serial.println("向左转");
  
  motor1.setSpeed(TURN_SPEED);
  motor2.setSpeed(TURN_SPEED);
  motor3.setSpeed(TURN_SPEED);
  motor4.setSpeed(TURN_SPEED);
  
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
  delay(400);  // 转弯时间，根据实际情况调整
  
  moveStop();
  goesForward = true;
}