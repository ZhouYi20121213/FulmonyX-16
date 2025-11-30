/* kernel.c - 带基础图形支持的16位内核 */
#define __STDC_HOSTED__ 0 // 禁用标准库依赖
#include <kernel/graphics.h>
/* C内核入口函数 */
void kernel_main() {
    print_string("=== FulmonyX-16 Graphics Demo ===");
    print_string("\nSwitching to graphics mode...");
    set_video_mode(0x13);
    while (1); // 停留在图形模式
}
