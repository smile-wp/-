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
