/* kernel.c - 带中断处理和图形支持的16位内核 */
#define __STDC_HOSTED__ 0
#include <kernel/graphics.h>
#include <kernel/interrupt.h>
#include <kernel/io.h>

extern void keyboard_isr_wrapper();

// 键盘中断处理函数
void keyboard_handler() {
    // 读取键盘扫描码以清除缓冲区
    uint8_t scancode = inb(0x60);
    (void)scancode;
    
    // 可以在这里添加键盘处理逻辑
    // 例如: 根据扫描码做不同的操作
}

void kernel_main() {
    print_string("=== FulmonyX-16 Graphics Demo ===");
    print_string("\nInitializing interrupt system...");
    
    // 注册键盘中断处理程序
    uint16_t handler_offset;
    uint16_t handler_segment;
    
    // 获取 handler 的地址
    __asm__ volatile(
        "mov %%cs, %0\n"
        "lea keyboard_isr_wrapper, %1"
        : "=r"(handler_segment), "=r"(handler_offset)
    );
    
    // 设置 IVT
    __asm__ volatile(
        "cli\n"                      // 禁用中断
        "push %%ds\n"                // 保存 DS
        "xor %%ax, %%ax\n"
        "mov %%ax, %%ds\n"           // DS = 0
        
        "mov $0x24, %%bx\n"          // INT 0x09 的 IVT 位置 (0x09 * 4)
        "mov %0, (%%bx)\n"           // 设置 offset
        "mov %1, 2(%%bx)\n"          // 设置 segment
        
        "pop %%ds\n"                 // 恢复 DS
        "sti\n"                      // 启用中断
        :
        : "r"(handler_offset), "r"(handler_segment)
        : "ax", "bx"
    );
    
    print_string("\nInterrupt system ready!");
    print_string("\nSwitching to graphics mode...");
    
    // 切换到图形模式
    set_video_mode(0x13);
    
    // 绘制一些图形示例
    // 绘制边框
    draw_rectangle(10, 10, 300, 180, COLOR_WHITE);
    
    // 绘制一些线条
    draw_line(20, 20, 100, 100, COLOR_RED);
    draw_line(100, 20, 20, 100, COLOR_GREEN);
    
    // 绘制圆形
    draw_circle(160, 100, 50, COLOR_CYAN);
    draw_circle(160, 100, 30, COLOR_YELLOW);
    
    // 主循环 - 等待中断
    while (1) {
        __asm__ volatile("hlt");
    }
}
