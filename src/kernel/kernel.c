/* kernel.c - 带基础图形支持的16位内核 */
#define __STDC_HOSTED__ 0 // 禁用标准库依赖
#include <kernel/graphics.h>
/* C内核入口函数 */
void kernel_main() {
    print_string("=== FulmonyX-16 Graphics Demo ===");
    print_string("\nSwitching to graphics mode...");
    set_video_mode(0x13);
    //test pattern
    draw_circle(160, 100, 50, 4); // 红色圆圈
    draw_line(0, 0, 319, 199, 2); // 绿色对角线
    draw_rectangle(50, 50, 100, 50, 1); // 蓝色矩形
    while (1); // 停留在图形模式
}
