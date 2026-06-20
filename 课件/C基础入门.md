# C基础入门

## 1 C初识

### 1.1  第一个C程序

编写一个C程序总共分为4个步骤

* 创建项目
* 创建文件
* 编写代码
* 运行程序

### 1.2 注释

**作用**：在代码中加一些说明和解释，方便自己或其他程序员程序员阅读代码

**两种格式**

1. **单行注释**：`// 描述信息` 
   - 通常放在一行代码的上方，或者一条语句的末尾，==对该行代码说明==
2. **多行注释**： `/* 描述信息 */`
   - 通常放在一段代码的上方，==对该段代码做整体说明==

> 提示：编译器在编译代码时，会忽略注释的内容

### 1.3变量

**作用**：给一段指定的内存空间起名，方便操作这段内存

**语法**：`数据类型 变量名 = 初始值;`

**示例：**

```c
#include <stdio.h>
using namespace std;

int main() {
//变量的定义
//语法：数据类型  变量名 = 初始值

int a = 10;
print a;

system("pause");

return 0;
    }
```


### 1.4 标识符命名规则

**作用**：C规定给标识符（变量、常量）命名时，有一套自己的规则

* 标识符不能是关键字
* 标识符只能由字母、数字、下划线组成
* 第一个字符必须为字母或下划线
* 标识符中字母区分大小写

> 建议：给标识符命名时，争取做到见名知意的效果，方便自己和他人的阅读

### 1.5数据类型

C规定在创建一个变量或者常量时，必须要指定出相应的数据类型，否则无法给变量分配内存

#### 1.5.1 整型

**作用**：整型变量表示的是==整数类型==的数据

C中能够表示整型的类型有以下几种方式，**区别在于所占内存空间不同**：



| **数据类型**        | **占用空间**                                    | 取值范围         |
| ------------------- | ----------------------------------------------- | ---------------- |
| short(短整型)       | 2字节                                           | (-2^15 ~ 2^15-1) |
| int(整型)           | 4字节                                           | (-2^31 ~ 2^31-1) |
| long(长整形)        | Windows为4字节，Linux为4字节(32位)，8字节(64位) | (-2^31 ~ 2^31-1) |
| long long(长长整形) | 8字节                                           | (-2^63 ~ 2^63-1) |

**整型结论**：==short < int <= long <= long long==

#### 1.5.2实型（浮点型）

**作用**：用于==表示小数==

浮点型变量分为两种：

1. 单精度float 
2. 双精度double

两者的**区别**在于表示的有效数字范围不同。

| **数据类型** | **占用空间** | **有效数字范围** |
| ------------ | ------------ | ---------------- |
| float        | 4字节        | 7位有效数字      |
| double       | 8字节        | 15～16位有效数字 |

#### 1.5.3字符型

**作用：**字符型变量用于显示单个字符

**语法：**`char ch = 'a';`



> 注意1：在显示字符型变量时，用单引号将字符括起来，不要用双引号

> 注意2：单引号内只能有一个字符，不可以是字符串



C和C++中字符型变量只占用==1个字节==。

字符型变量并不是把字符本身放到内存中存储，而是将对应的ASCII编码放入到存储单元

ASCII码表格：

| **ASCII**值 | **控制字符** | **ASCII**值 | **字符** | **ASCII**值 | **字符** | **ASCII**值 | **字符** |
| ----------- | ------------ | ----------- | -------- | ----------- | -------- | ----------- | -------- |
| 0           | NUT          | 32          | (space)  | 64          | @        | 96          | 、       |
| 1           | SOH          | 33          | !        | 65          | A        | 97          | a        |
| 2           | STX          | 34          | "        | 66          | B        | 98          | b        |
| 3           | ETX          | 35          | #        | 67          | C        | 99          | c        |
| 4           | EOT          | 36          | $        | 68          | D        | 100         | d        |
| 5           | ENQ          | 37          | %        | 69          | E        | 101         | e        |
| 6           | ACK          | 38          | &        | 70          | F        | 102         | f        |
| 7           | BEL          | 39          | ,        | 71          | G        | 103         | g        |
| 8           | BS           | 40          | (        | 72          | H        | 104         | h        |
| 9           | HT           | 41          | )        | 73          | I        | 105         | i        |
| 10          | LF           | 42          | *        | 74          | J        | 106         | j        |
| 11          | VT           | 43          | +        | 75          | K        | 107         | k        |
| 12          | FF           | 44          | ,        | 76          | L        | 108         | l        |
| 13          | CR           | 45          | -        | 77          | M        | 109         | m        |
| 14          | SO           | 46          | .        | 78          | N        | 110         | n        |
| 15          | SI           | 47          | /        | 79          | O        | 111         | o        |
| 16          | DLE          | 48          | 0        | 80          | P        | 112         | p        |
| 17          | DCI          | 49          | 1        | 81          | Q        | 113         | q        |
| 18          | DC2          | 50          | 2        | 82          | R        | 114         | r        |
| 19          | DC3          | 51          | 3        | 83          | S        | 115         | s        |
| 20          | DC4          | 52          | 4        | 84          | T        | 116         | t        |
| 21          | NAK          | 53          | 5        | 85          | U        | 117         | u        |
| 22          | SYN          | 54          | 6        | 86          | V        | 118         | v        |
| 23          | TB           | 55          | 7        | 87          | W        | 119         | w        |
| 24          | CAN          | 56          | 8        | 88          | X        | 120         | x        |
| 25          | EM           | 57          | 9        | 89          | Y        | 121         | y        |
| 26          | SUB          | 58          | :        | 90          | Z        | 122         | z        |
| 27          | ESC          | 59          | ;        | 91          | [        | 123         | {        |
| 28          | FS           | 60          | <        | 92          | /        | 124         | \|       |
| 29          | GS           | 61          | =        | 93          | ]        | 125         | }        |
| 30          | RS           | 62          | >        | 94          | ^        | 126         | `        |
| 31          | US           | 63          | ?        | 95          | _        | 127         | DEL      |

#### 1.5.4转义字符

**作用：**用于表示一些==不能显示出来的ASCII字符==

现阶段我们常用的转义字符有：` \n  \\  \t

| **转义字符** | **含义**                                | **ASCII**码值（十进制） |
| ------------ | --------------------------------------- | ----------------------- |
| \a           | 警报                                    | 007                     |
| \b           | 退格(BS) ，将当前位置移到前一列         | 008                     |
| \f           | 换页(FF)，将当前位置移到下页开头        | 012                     |
| **\n**       | **换行(LF) ，将当前位置移到下一行开头** | **010**                 |
| \r           | 回车(CR) ，将当前位置移到本行开头       | 013                     |
| **\t**       | **水平制表(HT)  （跳到下一个TAB位置）** | **009**                 |
| \v           | 垂直制表(VT)                            | 011                     |
| **\\\\**     | **代表一个反斜线字符"\"**               | **092**                 |
| \'           | 代表一个单引号（撇号）字符              | 039                     |
| \"           | 代表一个双引号字符                      | 034                     |
| \?           | 代表一个问号                            | 063                     |
| \0           | 数字0                                   | 000                     |
| \ddd         | 8进制转义字符，d范围0~7                 | 3位8进制                |
| \xhh         | 16进制转义字符，h范围0~9，a~f，A~F      | 3位16进制               |

#### 1.5.5 字符串型

**作用**：用于表示一串字符

**两种风格**

1. **C风格字符串**： `char 变量名[] = "字符串值"`

   示例：

```c
int main() {
char str1[] = "hello world";
cout << str1 << endl;

system("pause");

return 0;
}
```
注意：C风格的字符串要用双引号括起来

#### 1.5.6 布尔类型 bool

**作用：**布尔数据类型代表真或假的值 

bool类型只有两个值：

* true  --- 真（本质是1）
* false --- 假（本质是0）

**bool类型占==1个字节==大小**

示例：

## 2 程序代码

### 2.1hello word！

```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
```


![image-20250716174946324](C:\Users\13092\AppData\Roaming\Typora\typora-user-images\image-20250716174946324.png)

### 2.2爱心的实现

~~~c
#include <stdio.h>
#include <windows.h>

int main() {
    float x, y, a;
    // 设置控制台颜色为红色文字（Windows系统）
    system("color 04");

```
for (y = 1.5f; y > -1.5f; y -= 0.1f) {
    for (x = -1.5f; x < 1.5f; x += 0.05f) {
        a = x * x + y * y - 1;
        // 根据不等式判断点是否在爱心内部
        putchar(a * a * a - x * x * y * y * y <= 0.0f ? '*' : ' ');
    }
    putchar('\n');
}

// 暂停程序，等待用户按键
getchar();
return 0;
```

}
~~~


![image-20250716175632797](C:\Users\13092\AppData\Roaming\Typora\typora-user-images\image-20250716175632797.png)







### 2.3 弗拉迪维茨枪



```c
#include <windows.h>
#include <conio.h>

// 设置控制台光标位置
void setCursorPos(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 设置文本颜色（0-15）
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 弗拉迪克枪械字符图形
void drawVladik() {
    // 枪管部分
    setCursorPos(20, 5);
    setColor(8); // 灰色
    printf("▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
// 枪身主体
setCursorPos(15, 6);
setColor(6); // 棕色
printf("████████████████████");

// 弹匣
setCursorPos(18, 7);
setColor(4); // 红色
printf("║║║║║║║║");

// 握把
setCursorPos(22, 8);
setColor(2); // 绿色
printf("▄▄▄▄");

// 瞄准镜
setCursorPos(25, 4);
setColor(9); // 蓝色
printf("(○)");

// 枪械名称
setCursorPos(23, 10);
setColor(14); // 黄色
printf("VLADIK");
}

// 动态效果：开火动画
void fireAnimation() {
    for (int i = 0; i < 3; i++) {
        setCursorPos(50, 6);
        setColor(12); // 亮红色
        printf("★");
        Sleep(100);

        setCursorPos(50, 6);
        printf("  ");
        Sleep(50);
    }

}
int main() {
    // 隐藏光标
    CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    while(!_kbhit()) {
        system("cls"); // 清屏
        drawVladik();
        fireAnimation();
        Sleep(500); // 刷新间隔
    }
    return 0;

}
```




![image-20250717123936727](C:\Users\13092\AppData\Roaming\Typora\typora-user-images\image-20250717123936727.png)

### 2.4三只小猪称体重

![img](file:///C:\Users\13092\AppData\Local\Temp\ksohtml21272\wps2.jpg)

```c
#include <stdio.h>

int main() {
    // 1. 输入三只小猪的体重
    float pig1, pig2, pig3;
    printf("请输入三只小猪的体重（单位：千克）\n");
    printf("第一只小猪体重：");
    scanf("%f", &pig1);
    printf("第二只小猪体重：");
    scanf("%f", &pig2);
    printf("第三只小猪体重：");
    scanf("%f", &pig3);

    // 2. 比较体重并找出最大值
    float max_weight = pig1;  // 假设第一只最重
    int heaviest_pig = 1;

    if(pig2 > max_weight) {
        max_weight = pig2;
        heaviest_pig = 2;
    }

    if(pig3 > max_weight) {
        max_weight = pig3;
        heaviest_pig = 3;
    }

    // 3. 显示比较结果
    printf("\n===== 体重比较结果 =====\n");
    printf("第一只小猪：%.2fkg\n", pig1);
    printf("第二只小猪：%.2fkg\n", pig2);
    printf("第三只小猪：%.2fkg\n", pig3);
    printf("------------------------\n");
    printf("最重的是第%d只小猪，体重为：%.2fkg\n", heaviest_pig, max_weight);

    // 4. 额外比较：找出最轻的小猪
    float min_weight = pig1;
    int lightest_pig = 1;

    if(pig2 < min_weight) {
        min_weight = pig2;
        lightest_pig = 2;
    }

    if(pig3 < min_weight) {
        min_weight = pig3;
        lightest_pig = 3;
    }
    printf("最轻的是第%d只小猪，体重为：%.2fkg\n", lightest_pig, min_weight);

    return 0;
}

```

![image-20250717131301718](C:\Users\13092\AppData\Roaming\Typora\typora-user-images\image-20250717131301718.png)

### 2.5三角形

```
#include<stdio.h>

int main()
{
    int i,j,n;
    while(scanf("%d",&n)!=EOF)//输入想要的边长长度
    {
        for(i=0;i<n;i++)//有几列
        {
            for(j=n;j>i;j--)//几行
            {
                printf("* ");
            }printf("\n");
        }
    }
    return 0;
}

```

![image-20250718140038323](C:\Users\13092\AppData\Roaming\Typora\typora-user-images\image-20250718140038323.png)

### 2.6金字塔

```c
#include<stdio.h>

int main()
{
    int n;
    int i,j,k;
    while(scanf("%d",&n)!=EOF)
        for(i=1;i<=n;i++)//确定需要几行
        {
            for(j=n-i;j>=1;j--)
            {
                printf(" ");//每行先输出几个空格
            }
            for(k=1;k<=i;k++)//每行输出几个*
            {
                printf("* ");
                
            }printf("\n");
        }
    
    return 0;
}

```

### 2.7圣诞树

```
#include <stdio.h>

int main(){
    
    int n;
    scanf("%d" , &n);
    for(int i = 1 ; i <= n ; i ++)
    { 
        for(int j = 0 ; j < 3 ; j++)
        {
            for(int k = 1 ; k + i <= n ; k ++)
                printf("   ");
            for(int l = i ; l > 0 ; l--)
            {
                switch(j)
                {
                    case 0: printf("  *   "); break;
                    case 1: printf(" * *  "); break;
                    case 2: printf("* * * "); break;    
                    default: break;    
                }
            }
            printf("\n");
        }
    }
    for(int i = 1 ; i <= n ; i ++)
    {
        for(int k = 1 ; k < n ; k ++)
                printf("   ");
        printf("  *\n");
    }    
}

```

