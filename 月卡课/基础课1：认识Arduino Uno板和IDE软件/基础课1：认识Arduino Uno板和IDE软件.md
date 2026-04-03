# 基础课1：认识Arduino Uno板和IDE软件

### 🎯 今日学习目标

1. 认识Arduino Uno板的主要部件
2. 学会安装和配置Arduino IDE软件
3. 完成第一个程序上传并理解程序结构
4. 学会使用串口监视器调试程序

### ⏰ 课程时间分配（90分钟）

#### **第一部分：课程导入（10分钟）**

- **破冰活动（5分钟）**：展示有趣的Arduino项目视频（nfc唱片机、避障小车）
- **提出问题（5分钟）**：
  - "计算机怎么控制硬件？"
  - "Arduino是什么？能做什么？"
  - "你们想用Arduino创造什么？"

#### **第二部分：理论知识讲解（20分钟）**

**1. Arduino是什么？（5分钟）**

- 开源电子原型平台
- 硬件（各种板子）+ 软件（IDE）+ 社区
- 应用领域：机器人、智能家居、艺术装置等

**2. Arduino Uno板详细讲解（10分钟）**

![deepseek_mermaid_20251218_b9664a](D:\下载\deepseek_mermaid_20251218_b9664a.png)



**3. Arduino IDE安装演示（5分钟）**

- 下载、安装、基本界面介绍
- 工具栏功能：验证、上传、新建、打开、保存

#### **第三部分：动手实践（40分钟）**

**实践1：环境搭建（15分钟）**

1. 连接Arduino到电脑
2. 安装驱动程序（如果需要）
3. 选择开发板和端口
4. 测试连接

**实践2：第一个程序Blink（15分钟）**

cpp

```
/*
 * 项目：眨眼LED
 * 功能：让板载LED每秒闪烁一次
 * 知识点：setup()、loop()、pinMode()、digitalWrite()、delay()
 */

void setup() {
  // 初始化代码，只运行一次
  pinMode(13, OUTPUT);  // 设置13号引脚为输出模式
  Serial.begin(9600);   // 初始化串口通信，波特率9600
  Serial.println("Arduino已启动！");  // 向串口发送消息
}

void loop() {
  // 主循环代码，重复运行
  digitalWrite(13, HIGH);   // 输出高电平，LED亮
  Serial.println("LED亮");   // 串口输出状态
  delay(1000);               // 延时1000毫秒（1秒）
  
  digitalWrite(13, LOW);    // 输出低电平，LED灭
  Serial.println("LED灭");
  delay(1000);
}
```



**🔍 代码知识点详解：**

- `setup()`：程序启动时运行一次，用于初始化
- `loop()`：程序主循环，重复执行
- `pinMode(pin, mode)`：设置引脚模式（INPUT/OUTPUT）
- `digitalWrite(pin, value)`：输出数字信号（HIGH/LOW）
- `delay(ms)`：延时函数，单位毫秒
- `Serial.begin(rate)`：初始化串口通信
- `Serial.println(text)`：向串口发送数据并换行

**实践3：串口监视器使用（10分钟）**

1. 打开串口监视器（工具→串口监视器或Ctrl+Shift+M）
2. 观察程序输出的信息
3. 修改delay时间，观察变化
4. 尝试修改串口输出内容

#### **第四部分：知识总结与扩展（15分钟）**

**知识总结（5分钟）**

- Arduino Uno板各部件功能
- IDE基本操作流程
- 程序的基本结构

**扩展活动：创意闪烁（10分钟）**

cpp

```
// 扩展练习：让LED发出SOS求救信号
// SOS摩斯码：... --- ...（三短三长三短）

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  // 三个短闪（代表S）
  for(int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    delay(200);  // 短闪200ms
    digitalWrite(13, LOW);
    delay(200);
  }
  
  delay(300);  // 字母间间隔
  
  // 三个长闪（代表O）
  for(int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    delay(600);  // 长闪600ms
    digitalWrite(13, LOW);
    delay(200);
  }
  
  delay(300);
  
  // 再来三个短闪（代表S）
  for(int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }
  
  delay(2000);  // 消息间长间隔
}
```



**家庭作业：**

1. 画出Arduino Uno板的简图并标注主要部件
2. 设计自己的闪烁模式（心跳、摩斯码名字等）

------