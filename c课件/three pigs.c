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
