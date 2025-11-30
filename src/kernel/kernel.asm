; kernel.asm - 内核汇编桥接代码 (NASM语法)
bits 16
global _start
extern kernel_main

section .text
_start:
    ; 初始化段寄存器（与引导程序跳转的段地址一致）
    mov ax, 0x1000         ; 内核所在的段地址
    mov ds, ax             ; 数据段 = 0x1000
    mov es, ax             ; 附加段 = 0x1000
    mov ss, ax             ; 栈段 = 0x1000
    mov sp, 0x0800         ; 栈指针设为0x1000:0800

    call kernel_main       ; 调用C内核入口函数
    
    ; 内核返回后进入死循环
.halt:
    hlt
    jmp .halt