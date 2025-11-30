; interrupt_asm.asm
bits 16

global keyboard_isr_wrapper
extern keyboard_handler

section .text

; 键盘中断处理程序包装器
keyboard_isr_wrapper:
    pusha           ; 保存通用寄存器
    push ds
    push es
    
    ; 设置内核数据段 (假设内核数据段在 0x1000)
    ; 注意：这里必须与 kernel.asm 中的设置一致
    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    
    call keyboard_handler
    
    ; 发送 EOI (End of Interrupt) 给 PIC
    mov al, 0x20
    out 0x20, al
    
    pop es
    pop ds
    popa            ; 恢复通用寄存器
    iret            ; 中断返回
