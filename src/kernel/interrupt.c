#include <kernel/interrupt.h>

// 获取中断向量表的指针
// 注意: IVT 在物理地址 0x00000,需要临时切换 DS 段
ivt_entry_t* get_ivt() {
    // 在实模式下,IVT 位于 0x0000:0x0000
    // 我们需要使用段地址 0 来访问它
    return (ivt_entry_t*)0x0000;  // 这会被解释为 DS:0x0000
}

void register_interrupt_handler(uint8_t interrupt_number, void (*handler)()) {
    uint16_t old_ds;
    uint16_t current_cs;
    ivt_entry_t* ivt;
    
    // 获取当前的 CS (在改变 DS 之前)
    __asm__ volatile("mov %%cs, %0" : "=r"(current_cs));
    
    // 禁用中断以保证原子性 (在改变 DS 之前)
    disable_interrupts();
    
    // 保存当前 DS
    __asm__ volatile("mov %%ds, %0" : "=r"(old_ds));
    
    // 设置 DS = 0 以访问 IVT
    __asm__ volatile("mov %0, %%ds" : : "r"((uint16_t)0));
    
    // 现在可以安全访问 IVT
    ivt = (ivt_entry_t*)0x0000;
    
    ivt[interrupt_number].offset = (uint16_t)((uintptr_t)handler);
    ivt[interrupt_number].segment = current_cs;
    
    // 恢复原来的 DS (在启用中断之前)
    __asm__ volatile("mov %0, %%ds" : : "r"(old_ds));
    
    // 重新启用中断
    enable_interrupts();
}

void enable_interrupts() {
    __asm__ volatile("sti");
}

void disable_interrupts() {
    __asm__ volatile("cli");
}
