// ARDUINO OBSTACLE AVOIDING CAR - L9110s版 (双TT电机)
#include <NewPing.h>
#include <Servo.h> 
#define TRIG_PIN 6
#define ECHO_PIN 7 
#define MAX_DISTANCE 200 

// L9110s驱动引脚定义 (根据PDF第8页)
#define MOTOR_A_PIN1 2   // MA方向1 (PDF中对应MA > PINS 3 AND 5)
#define MOTOR_A_PIN2 3   // MA方向2
#define MOTOR_B_PIN1 4   // MB方向1 (PDF中对应MB > PINS 6 AND 11)
#define MOTOR_B_PIN2 5  // MB方向2

// 速度参数 (PWM值0-255)
#define MAX_SPEED 200
#define CRUISE_SPEED 180
#define TURN_SPEED 150
#define SLOW_SPEED 100

// 避障参数
#define OBSTACLE_DISTANCE 20  // 障碍物检测距离（厘米）
#define SAFE_DISTANCE 30      // 安全距离
#define CRITICAL_DISTANCE 10  // 紧急停止距离

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;

// 状态变量
boolean goesForward = false;
int distance = 100;
int avoidState = 0;  // 避障策略状态

void setup() {
  // 初始化串口调试
  Serial.begin(9600);
  
  // 设置电机控制引脚为输出
  pinMode(MOTOR_A_PIN1, OUTPUT);
  pinMode(MOTOR_A_PIN2, OUTPUT);
  pinMode(MOTOR_B_PIN1, OUTPUT);
  pinMode(MOTOR_B_PIN2, OUTPUT);
  
  // 电源稳定等待
  delay(2000);
  
  // 舵机初始化 (根据PDF第11页，舵机信号线接引脚13)
  myservo.attach(9);  
  myservo.write(90);  // 初始位置居中
  delay(1000);
  
  // 电机预测试
  testMotors();
  
  // 初始距离测量
  for(int i = 0; i < 3; i++) {
    distance = readPing();
    delay(100);
  }
  
  Serial.println("小车初始化完成！");
  Serial.println("使用L9110s驱动板，双TT电机配置");
}

void testMotors() {
  Serial.println("电机测试中...");
  
  // 前转测试
  moveForward();
  delay(500);
  moveStop();
  delay(300);
  
  // 后转测试
  moveBackward();
  delay(500);
  moveStop();
  delay(300);
  
  // 左转测试
  turnLeft();
  delay(500);
  moveStop();
  delay(300);
  
  // 右转测试
  turnRight();
  delay(500);
  moveStop();
  delay(300);
  
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
  // L9110s: 两个引脚都输出HIGH或LOW实现刹车
  analogWrite(MOTOR_A_PIN1, 255);
  analogWrite(MOTOR_A_PIN2, 255);
  analogWrite(MOTOR_B_PIN1, 255);
  analogWrite(MOTOR_B_PIN2, 255);
  delay(100);
  moveStop();
}

// 掉头（180度）
void turnAround() {
  Serial.println("执行180度掉头");
  
  // 原地旋转：一个电机前进，一个后退
  analogWrite(MOTOR_A_PIN1, TURN_SPEED);
  analogWrite(MOTOR_A_PIN2, 0);
  analogWrite(MOTOR_B_PIN1, 0);
  analogWrite(MOTOR_B_PIN2, TURN_SPEED);
  
  delay(800);  // 调整这个时间以获得180度转弯
  
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

// 电机控制函数 - L9110s专用
void moveStop() {
  // L9110s停止：两个引脚都输出LOW
  analogWrite(MOTOR_A_PIN1, 0);
  analogWrite(MOTOR_A_PIN2, 0);
  analogWrite(MOTOR_B_PIN1, 0);
  analogWrite(MOTOR_B_PIN2, 0);
  goesForward = false;
}

void moveForward() {
  if(!goesForward) {
    goesForward = true;
    // 缓启动
    for(int i = 80; i <= CRUISE_SPEED; i+=20) {
      analogWrite(MOTOR_A_PIN1, i);
      analogWrite(MOTOR_A_PIN2, 0);
      analogWrite(MOTOR_B_PIN1, i);
      analogWrite(MOTOR_B_PIN2, 0);
      delay(50);
    }
  }
  
  // 保持前进
  analogWrite(MOTOR_A_PIN1, CRUISE_SPEED);
  analogWrite(MOTOR_A_PIN2, 0);
  analogWrite(MOTOR_B_PIN1, CRUISE_SPEED);
  analogWrite(MOTOR_B_PIN2, 0);
}

void moveBackward() {
  goesForward = false;
  
  // 缓启动后退
  for(int i = 80; i <= CRUISE_SPEED; i+=20) {
    analogWrite(MOTOR_A_PIN1, 0);
    analogWrite(MOTOR_A_PIN2, i);
    analogWrite(MOTOR_B_PIN1, 0);
    analogWrite(MOTOR_B_PIN2, i);
    delay(50);
  }
  
  // 保持后退
  analogWrite(MOTOR_A_PIN1, 0);
  analogWrite(MOTOR_A_PIN2, CRUISE_SPEED);
  analogWrite(MOTOR_B_PIN1, 0);
  analogWrite(MOTOR_B_PIN2, CRUISE_SPEED);
}

void turnRight() {
  Serial.println("向右转");
  
  // 右转：左轮前进，右轮后退（原地转弯）
  analogWrite(MOTOR_A_PIN1, TURN_SPEED);  // 左轮前进
  analogWrite(MOTOR_A_PIN2, 0);
  analogWrite(MOTOR_B_PIN1, 0);
  analogWrite(MOTOR_B_PIN2, TURN_SPEED);  // 右轮后退
  
  delay(400);  // 转弯时间，根据实际情况调整
  
  moveStop();
  goesForward = true;
}

void turnLeft() {
  Serial.println("向左转");
  
  // 左转：右轮前进，左轮后退（原地转弯）
  analogWrite(MOTOR_A_PIN1, 0);
  analogWrite(MOTOR_A_PIN2, TURN_SPEED);  // 左轮后退
  analogWrite(MOTOR_B_PIN1, TURN_SPEED);  // 右轮前进
  analogWrite(MOTOR_B_PIN2, 0);
  
  delay(400);  // 转弯时间，根据实际情况调整
  
  moveStop();
  goesForward = true;
}