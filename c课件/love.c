#include <stdio.h>
#include <windows.h>

int main() {
    float x, y, a;
    // 设置控制台颜色为红色文字（Windows系统）
    system("color 04");

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
}
