#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <kernel/types.h>

// 16位实模式下的中断向量表条目结构
typedef struct {
    uint16_t offset;
    uint16_t segment;
} __attribute__((packed)) ivt_entry_t;

// 中断向量表位于内存 0x0000 处
#define IVT_ADDRESS 0x0000

// 注册中断处理程序
void register_interrupt_handler(uint8_t interrupt_number, void (*handler)());

// 启用中断
void enable_interrupts();

// 禁用中断
void disable_interrupts();

#endif
