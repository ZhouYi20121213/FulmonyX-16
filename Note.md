# FulmonyX-16 OS：一个16位操作系统

## 〇、预备知识

你至少符合以下条件：

1. 了解大致**汇编**语法
2. 掌握**C语言**语法
3. 会用**make** ^(至少看的懂)^
4. 熟练使用**命令行**工具
5. 有^makefile^所示工具链
6. 了解系统调用等知识

## 一、第一个引导扇区 (bootloader)

所有操作系统，你得先被主板引导才可以跑。

主流主板是从 **0x7c00** 处引导的，所以我们要先把我们的程序加载到 **0x7c00** 处。

在汇编中，正好有一个指令可以实现这个功能，那就是 `org [地址]`。

所以我们可以在程序开头写上 `org 0x7c00`。

可是光这样还不够，程序结尾的两个字节必须是 `0x55AA`（原因未知，毕竟是 <s> 歪果仁 </s> 规定的）。

所谓**扇区**，通俗来讲就是一个大小是`512`字节的内存块。

也就是说，**引导扇区**其实只是一个**中转站**，把执行指令的东西中转到你的**内核**（*kernel*）中去。

## 二、内核 (kernel)

### 2.1 内核的基本结构

内核是操作系统的核心部分，负责管理系统资源、提供系统服务。在我们的16位系统中，内核由两部分组成：

1. **汇编桥接代码** (`kernel.asm`) - 设置运行环境
2. **C语言核心** (`kernel.c`) - 主要功能实现

### 2.2 内存布局

``` text
0x00000 - 0x7BFFF: 可用内存区域
0x7C00  - 0x7DFF : 引导扇区 (512字节)
0x7E00  - 0x9FFFF: 可用内存  
0x10000 - 0x1FFFF: 内核加载区域 (64KB)
```

### 2.3 汇编桥接代码

汇编部分主要负责：
- 设置段寄存器
- 初始化栈指针
- 调用C语言主函数
- 提供基本的异常处理

```assembly
; kernel.asm - 内核汇编桥接代码
bits 16
global _start
extern kernel_main

section .text
_start:
    ; 初始化段寄存器
    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x0800

    call kernel_main       ; 调用C内核
    
.halt:
    hlt                    ; 节能等待
    jmp .halt
```

### 2.4 C语言内核核心

C语言部分实现主要功能：
- 屏幕输出管理
- 键盘输入处理
- 系统服务提供

```c
/* kernel.c - C语言内核 */
void kernel_main() {
    print_string("=== FulmonyX-16 OS ===");
    print_string("\nKernel initialized successfully!");
    
    // 主循环
    while (1) {
        // 系统任务处理
    }
}
```

### 2.5 编译流程

``` text
bootloader.asm (NASM) → bootloader.bin
kernel.asm (NASM)     → kernel_asm.o
kernel.c (GCC)        → kernel_c.o
                    ↓ (链接)
kernel.elf → kernel.bin
                    ↓ (组合)
FulmonyX-16.img (可启动镜像)
```


* Tips:代码马上出现在**Github**上了

### 2.6 图形界面
<见代码>