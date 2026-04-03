// 智能避障小车 - L9110s驱动版（支持蓝牙遥控）
#include <NewPing.h>
#include <Servo.h> 

// 引脚定义
#define TRIG_PIN 6
#define ECHO_PIN 7 
#define MAX_DISTANCE 200 

// L9110s驱动引脚定义
#define MOTOR_A_PIN1 2   // 左电机方向1
#define MOTOR_A_PIN2 3   // 左电机方向2
#define MOTOR_B_PIN1 4   // 右电机方向1
#define MOTOR_B_PIN2 5   // 右电机方向2

// 速度参数 (PWM值0-255)
#define MAX_SPEED 200
#define CRUISE_SPEED 180
#define TURN_SPEED 150
#define SLOW_SPEED 100

// 避障参数
#define OBSTACLE_DISTANCE 20  // 障碍物检测距离（厘米）
#define SAFE_DISTANCE 30      // 安全距离
#define CRITICAL_DISTANCE 10  // 紧急停止距离

// 蓝牙控制参数
#define BT_COMMAND_TIMEOUT 1000  // 蓝牙命令超时时间(ms)

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;

// 状态变量
boolean goesForward = false;
int distance = 100;
int avoidState = 0;  // 避障策略状态

// 模式控制
enum Mode {AUTO_MODE, BLUETOOTH_MODE};
Mode currentMode = AUTO_MODE;  // 默认自动模式
unsigned long lastBTCommandTime = 0;  // 最后蓝牙命令时间

void setup() {
  // 初始化串口（用于蓝牙和调试）
  Serial.begin(9600);
  
  // 设置电机控制引脚为输出
  pinMode(MOTOR_A_PIN1, OUTPUT);
  pinMode(MOTOR_A_PIN2, OUTPUT);
  pinMode(MOTOR_B_PIN1, OUTPUT);
  pinMode(MOTOR_B_PIN2, OUTPUT);
  
  // 电源稳定等待
  delay(2000);
  
  // 舵机初始化（舵机信号线接引脚9）
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
  
  Serial.println("智能避障小车初始化完成！");
  Serial.println("当前模式：自动避障模式");
  Serial.println("蓝牙命令：");
  Serial.println("  0 - 切换模式（自动/蓝牙）");
  Serial.println("  1 - 前进    2 - 后退");
  Serial.println("  3 - 停止    4 - 左前");
  Serial.println("  5 - 右前    6 - 左后");
  Serial.println("  7 - 右后    8 - 原地左转");
  Serial.println("  9 - 原地右转");
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
  // 检查蓝牙命令
  checkBluetoothCommand();
  
  // 根据当前模式执行相应操作
  if (currentMode == AUTO_MODE) {
    // 自动避障模式
    runAutoMode();
  } else {
    // 蓝牙遥控模式
    runBluetoothMode();
  }
}

void runAutoMode() {
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

void runBluetoothMode() {
  // 蓝牙模式：等待命令并执行
  // 命令已在checkBluetoothCommand()中处理
  
  // 如果长时间没有收到蓝牙命令，自动停止
  if (millis() - lastBTCommandTime > BT_COMMAND_TIMEOUT) {
    moveStop();
  }
  
  delay(50);  // 减少循环延迟，提高响应速度
}

void checkBluetoothCommand() {
  if (Serial.available() > 0) {
    int incomingByte = Serial.parseInt();
    
    // 记录最后命令时间
    lastBTCommandTime = millis();
    
    // 显示接收到的命令
    Serial.print("接收到命令: ");
    Serial.println(incomingByte);
    
    // 使用switch语句处理命令
    switch(incomingByte) {
      case 0:  // 模式切换
        toggleMode();
        break;
        
      case 1:  // 前进
        moveForward();
        break;
        
      case 2:  // 后退
        moveBackward();
        break;
        
      case 3:  // 停止
        moveStop();
        break;
        
      case 4:  // 左前
        turnLeftForward();
        break;
        
      case 5:  // 右前
        turnRightForward();
        break;
        
      case 6:  // 左后
        turnLeftBack();
        break;
        
      case 7:  // 右后
        turnRightBack();
        break;
        
      case 8:  // 原地左转
        turnLeft();
        break;
        
      case 9:  // 原地右转
        turnRight();
        break;
        
      default:
        Serial.println("未知命令");
    }
    
    // 清空串口缓冲区
    while(Serial.available() > 0) {
      Serial.read();
    }
  }
}

void toggleMode() {
  if (currentMode == AUTO_MODE) {
    currentMode = BLUETOOTH_MODE;
    moveStop();
    myservo.write(90);  // 舵机回中
    Serial.println("切换到蓝牙遥控模式");
  } else {
    currentMode = AUTO_MODE;
    myservo.write(90);  // 舵机回中
    Serial.println("切换到自动避障模式");
  }
}

// 以下是差速转向函数（蓝牙模式专用）
void turnRightForward() {
  Serial.println("右前转向");
  // 右前：左轮全速，右轮半速
  analogWrite(MOTOR_A_PIN1, CRUISE_SPEED);
  analogWrite(MOTOR_A_PIN2, 0);
  analogWrite(MOTOR_B_PIN1, CRUISE_SPEED/2);
  analogWrite(MOTOR_B_PIN2, 0);
  goesForward = true;
}

void turnLeftForward() {
  Serial.println("左前转向");
  // 左前：左轮半速，右轮全速
  analogWrite(MOTOR_A_PIN1, CRUISE_SPEED/2);
  analogWrite(MOTOR_A_PIN2, 0);
  analogWrite(MOTOR_B_PIN1, CRUISE_SPEED);
  analogWrite(MOTOR_B_PIN2, 0);
  goesForward = true;
}

void turnRightBack() {
  Serial.println("右后转向");
  // 右后：左轮全速后退，右轮半速后退
  analogWrite(MOTOR_A_PIN1, 0);
  analogWrite(MOTOR_A_PIN2, CRUISE_SPEED);
  analogWrite(MOTOR_B_PIN1, 0);
  analogWrite(MOTOR_B_PIN2, CRUISE_SPEED/2);
  goesForward = false;
}

void turnLeftBack() {
  Serial.println("左后转向");
  // 左后：左轮半速后退，右轮全速后退
  analogWrite(MOTOR_A_PIN1, 0);
  analogWrite(MOTOR_A_PIN2, CRUISE_SPEED/2);
  analogWrite(MOTOR_B_PIN1, 0);
  analogWrite(MOTOR_B_PIN2, CRUISE_SPEED);
  goesForward = false;
}

// 以下是原有避障相关函数
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
  Serial.println("原地右转");
  
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
  Serial.println("原地左转");
  
  // 左转：右轮前进，左轮后退（原地转弯）
  analogWrite(MOTOR_A_PIN1, 0);
  analogWrite(MOTOR_A_PIN2, TURN_SPEED);  // 左轮后退
  analogWrite(MOTOR_B_PIN1, TURN_SPEED);  // 右轮前进
  analogWrite(MOTOR_B_PIN2, 0);
  
  delay(400);  // 转弯时间，根据实际情况调整
  
  moveStop();
  goesForward = true;
}