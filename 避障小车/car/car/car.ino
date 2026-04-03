// ARDUINO OBSTACLE AVOIDING CAR - 修复版
// 针对TT电机启动问题优化

#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 
#define MAX_SPEED 190     // 最大速度
#define START_SPEED 180   // 新增：启动速度（必须足够高）
#define MIN_SPEED 150     // 新增：最低运行速度

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

// 根据错误提示，使用可用的频率常量
// 如果MOTOR12_8KHZ可用，就使用8KHz；否则使用原来的1KHz
// 尝试使用编译器建议的替代方案
AF_DCMotor motor1(1, MOTOR12_8KHZ);  // 改为8KHz
AF_DCMotor motor2(2, MOTOR12_8KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);  // 使用原来的1KHz，或者尝试MOTOR34_64KHZ
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;   

boolean goesForward = false;
int distance = 100;
int speedSet = 0;

void setup() {
  // 电源稳定等待
  delay(2000);
  
  myservo.attach(10);  
  myservo.write(115); 
  delay(2000);
  
  // 电机预测试
  testMotors();
  
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

// 新增：电机预测试函数
void testMotors() {
  // 分别测试每个电机
  motor1.setSpeed(150);
  motor1.run(FORWARD);
  delay(100);
  motor1.run(RELEASE);
  delay(50);
  
  motor2.setSpeed(150);
  motor2.run(FORWARD);
  delay(100);
  motor2.run(RELEASE);
  delay(50);
  
  motor3.setSpeed(150);
  motor3.run(FORWARD);
  delay(100);
  motor3.run(RELEASE);
  delay(50);
  
  motor4.setSpeed(150);
  motor4.run(FORWARD);
  delay(100);
  motor4.run(RELEASE);
  delay(200);
}

void loop() {
  int distanceR = 0;
  int distanceL = 0;
  delay(40);
  
  if(distance <= 15) {
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    delay(200);
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    if(distanceR >= distanceL) {
      turnRight();
      moveStop();
    } else {
      turnLeft();
      moveStop();
    }
  } else {
    moveForward();
  }
  distance = readPing();
}

int lookRight() {
  myservo.write(50); 
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115); 
  return distance;
}

int lookLeft() {
  myservo.write(170); 
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115); 
  return distance;
}

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm == 0) {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
} 

// 修改的moveForward函数
void moveForward() {
  if(!goesForward) {
    goesForward = true;
    
    // 方法1：高扭矩启动（推荐）
    motor1.setSpeed(START_SPEED);
    motor2.setSpeed(START_SPEED);
    motor3.setSpeed(START_SPEED);
    motor4.setSpeed(START_SPEED);
    
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    
    // 保持高扭矩100ms确保启动
    delay(100);
    
    // 然后调整到运行速度
    motor1.setSpeed(MIN_SPEED);
    motor2.setSpeed(MIN_SPEED);
    motor3.setSpeed(MIN_SPEED);
    motor4.setSpeed(MIN_SPEED);
    
    // 可选：缓慢加速到MAX_SPEED
    for (speedSet = MIN_SPEED; speedSet < MAX_SPEED; speedSet += 2) {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet);
      motor3.setSpeed(speedSet);
      motor4.setSpeed(speedSet);
      delay(5);
    }
  } else {
    // 如果已经在前进，保持当前速度
    motor1.setSpeed(MAX_SPEED);
    motor2.setSpeed(MAX_SPEED);
    motor3.setSpeed(MAX_SPEED);
    motor4.setSpeed(MAX_SPEED);
  }
}

// 修改的moveBackward函数
void moveBackward() {
  goesForward = false;
  
  // 高扭矩启动
  motor1.setSpeed(START_SPEED);
  motor2.setSpeed(START_SPEED);
  motor3.setSpeed(START_SPEED);
  motor4.setSpeed(START_SPEED);
  
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  
  // 保持高扭矩100ms
  delay(100);
  
  // 然后加速
  for (speedSet = START_SPEED; speedSet < MAX_SPEED; speedSet += 2) {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}

void turnRight() {
  // 转弯时也使用较高速度
  motor1.setSpeed(MAX_SPEED);
  motor2.setSpeed(MAX_SPEED);
  motor3.setSpeed(MAX_SPEED);
  motor4.setSpeed(MAX_SPEED);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);     
  delay(500);
  
  // 转弯后继续前进
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
  // 重置前进标志
  goesForward = true;
}

void turnLeft() {
  // 转弯时也使用较高速度
  motor1.setSpeed(MAX_SPEED);
  motor2.setSpeed(MAX_SPEED);
  motor3.setSpeed(MAX_SPEED);
  motor4.setSpeed(MAX_SPEED);
  
  motor1.run(BACKWARD);     
  motor2.run(BACKWARD);  
  motor3.run(FORWARD);
  motor4.run(FORWARD);   
  delay(500);
  
  // 转弯后继续前进
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
  // 重置前进标志
  goesForward = true;
}