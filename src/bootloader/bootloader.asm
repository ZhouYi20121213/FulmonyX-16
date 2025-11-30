; bootloader.asm - 引导加载程序 (NASM语法)
bits 16
org 0x7c00

; 初始化段寄存器和栈
mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00

; 打印加载内核信息
mov si, load_msg
call print_string

; 从磁盘读取内核到内存（0x1000:0000）
mov ah, 0x02                ; 功能号：读磁盘
mov al, 10                  ; 读取10个扇区（增加容量）
mov ch, 0                   ; 柱面0
mov cl, 2                   ; 扇区2（引导程序在扇区1）
mov dh, 0                   ; 磁头0
mov dl, 0x80                ; 设备号：第一块硬盘
mov bx, 0x1000              ; 目标地址：es:bx = 0x1000:0000
mov es, bx
xor bx, bx                  ; bx=0
int 0x13

; 检查读盘是否成功
jc disk_error

; 跳转到内核入口（0x1000:0000）
jmp 0x1000:0x0000

; 读盘失败处理
disk_error:
    mov si, error_msg
    call print_string
    jmp $

; 字符串打印函数
print_string:
    mov ah, 0x0E
.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
.done:
    ret

; 字符串数据
load_msg db 'Loading FulmonyX-16 kernel...', 0x0D, 0x0A, 0
error_msg db 'Disk read error!', 0x0D, 0x0A, 0

; 填充到512字节，并添加引导签名
times 510 - ($ - $$) db 0
dw 0xAA55 ; = db 0x55,0xAA