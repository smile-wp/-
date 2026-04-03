### 什么是Arduino？

Arduino就像是一个会思考的小电脑，你可以告诉它做什么，它就会按照你的指令控制各种电子元件。



### 需要准备的物品：

1. Arduino Uno板 x1
2. 面包板 x1
3. LED灯 x3（不同颜色）
4. 电阻（220欧姆）x3
5. 按钮 x1
6. 电位器（旋钮）x1
7. 连接线若干
8. USB数据线 x1

## 案例1：让LED灯闪烁

### 连接方法：

- 将LED的长脚（正极）连接到Arduino的13号引脚
- 将LED的短脚（负极）连接到GND（接地）
- 在LED和13号引脚之间加一个220欧姆电阻

### 代码（在Arduino IDE中输入）：

arduino

```
void setup() {
  pinMode(13, OUTPUT);  // 告诉Arduino，13号引脚是用来输出的
}

void loop() {
  digitalWrite(13, HIGH);  // 让13号引脚输出高电平，LED亮
  delay(1000);             // 等待1000毫秒（1秒）
  digitalWrite(13, LOW);   // 让13号引脚输出低电平，LED灭
  delay(1000);             // 等待1000毫秒（1秒）
}
```



### 解释：

- `setup()`函数只在开始时运行一次
- `loop()`函数会一直重复运行
- `delay(1000)`意思是等待1秒钟

## 案例2：交通信号灯

### 连接方法：

- 红色LED：正极→7号引脚，负极→GND
- 黄色LED：正极→6号引脚，负极→GND
- 绿色LED：正极→5号引脚，负极→GND
- 每个LED都要串联220欧姆电阻

### 代码：

arduino

```
void setup() {
  pinMode(7, OUTPUT);   // 红灯
  pinMode(6, OUTPUT);   // 黄灯
  pinMode(5, OUTPUT);   // 绿灯
}

void loop() {
  // 红灯亮
  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
  delay(3000);          // 等待3秒
  
  // 绿灯亮
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, HIGH);
  delay(3000);          // 等待3秒
  
  // 黄灯亮
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(5, LOW);
  delay(1000);          // 等待1秒
}
```



## 案例3：用按钮控制LED

### 连接方法：

- LED：正极→13号引脚，负极→GND
- 按钮一脚→2号引脚，另一脚→GND
- 在按钮和2号引脚之间连接一个10k欧姆电阻（上拉电阻）

### 代码：

arduino

```
int buttonState = 0;    // 用来存储按钮状态的变量

void setup() {
  pinMode(13, OUTPUT);  // LED引脚
  pinMode(2, INPUT);    // 按钮引脚
}

void loop() {
  buttonState = digitalRead(2);  // 读取按钮状态
  
  if (buttonState == HIGH) {
    digitalWrite(13, HIGH);      // 如果按钮按下，LED亮
  } else {
    digitalWrite(13, LOW);       // 如果按钮没按下，LED灭
  }
}
```



## 案例4：用旋钮控制LED亮度

### 连接方法：

- LED：正极→9号引脚，负极→GND
- 电位器中间引脚→A0引脚
- 电位器另外两脚：一个接5V，一个接GND

### 代码：

arduino

```
int potValue = 0;       // 存储电位器读数的变量
int ledValue = 0;       // 存储LED亮度值的变量

void setup() {
  pinMode(9, OUTPUT);   // LED引脚（必须是带~的引脚）
}

void loop() {
  potValue = analogRead(A0);             // 读取电位器值（0-1023）
  ledValue = map(potValue, 0, 1023, 0, 255);  // 将0-1023转换为0-255
  analogWrite(9, ledValue);              // 设置LED亮度
  delay(10);                             // 短暂延迟
}
```



## 使用步骤：

1. **连接硬件**：按照图示用面包板连接元件
2. **打开Arduino IDE**：在电脑上打开编程软件
3. **编写代码**：输入上面的代码
4. **选择板卡**：工具→板卡→Arduino Uno
5. **选择端口**：工具→端口→选择正确的COM口
6. **上传代码**：点击上传按钮（向右箭头）
7. **观察效果**：看LED是否按预期工作

## 小贴士：

- LED长脚是正极，短脚是负极
- 电阻没有正负极，可以任意方向连接
- 上传代码时，Arduino板上的LED会快速闪烁
- 如果代码上传失败，检查USB连接和端口选择
- 大胆尝试修改代码中的数字，看看会发生什么变化！

## 扩展挑战：

1. 尝试改变闪烁的速度
2. 让两个LED交替闪烁
3. 创建自己的灯光模式
4. 尝试用不同的颜色LED

记住：编程就像搭积木，从简单的开始，慢慢组合成复杂的效果。祝你玩得开心！