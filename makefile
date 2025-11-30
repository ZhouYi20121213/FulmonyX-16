# FulmonyX-16 OS Makefile
NASM := nasm
GCC := gcc
LD := ld
OBJCOPY := objcopy
QEMU := qemu-system-x86_64
DD := dd
MKDIR := mkdir -p
RM := rm -f

# 目录结构
SRC_DIR := src
BOOT_DIR := $(SRC_DIR)/bootloader
KERNEL_DIR := $(SRC_DIR)/kernel
OUT_DIR := out

# 源文件
BOOTLOADER_SRC := $(BOOT_DIR)/bootloader.asm
KERNEL_ASM_SRC := $(KERNEL_DIR)/kernel.asm
KERNEL_C_SRC := $(KERNEL_DIR)/kernel.c
LINK_SCRIPT := $(SRC_DIR)/ld-kernel.ld

# 输出文件
BOOTLOADER_BIN := $(OUT_DIR)/bootloader.bin
KERNEL_ASM_OBJ := $(OUT_DIR)/kernel_asm.o
KERNEL_C_OBJ := $(OUT_DIR)/kernel_c.o
KERNEL_ELF := $(OUT_DIR)/kernel.elf
KERNEL_BIN := $(OUT_DIR)/kernel.bin
OS_IMG := FulmonyX-16.iso

# 编译标志
GCC_FLAGS := -m16 -ffreestanding -fno-pie -nostdlib -O0 -Wall -Wextra

.PHONY: all run clean

all: $(OS_IMG)

# 创建输出目录
$(OUT_DIR):
	@$(MKDIR) $(OUT_DIR)

# 编译引导程序
$(BOOTLOADER_BIN): $(BOOTLOADER_SRC) | $(OUT_DIR)
	@echo "Building bootloader..."
	$(NASM) -f bin $< -o $@

# 编译内核汇编代码
$(KERNEL_ASM_OBJ): $(KERNEL_ASM_SRC) | $(OUT_DIR)
	@echo "Building kernel ASM..."
	$(NASM) -f elf32 $< -o $@ -I inc/

# 编译内核C代码
$(KERNEL_C_OBJ): $(KERNEL_C_SRC) | $(OUT_DIR)
	@echo "Building kernel C..."
	$(GCC) $(GCC_FLAGS) -c $< -o $@ -I inc/

# 链接内核
$(KERNEL_ELF): $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(LINK_SCRIPT)
	@echo "Linking kernel..."
	$(LD) -m elf_i386 -T $(LINK_SCRIPT) -o $@ $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ)

# 提取内核二进制
$(KERNEL_BIN): $(KERNEL_ELF)
	@echo "Extracting kernel binary..."
	$(OBJCOPY) -O binary $< $@

# 创建磁盘镜像
$(OS_IMG): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	@echo "Creating disk image..."
	$(DD) if=/dev/zero of=$@ bs=512 count=2880 status=none
	$(DD) if=$(BOOTLOADER_BIN) of=$@ bs=512 count=1 conv=notrunc status=none
	$(DD) if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc status=none
	@echo "OS image created: $@"

# 运行虚拟机
run: $(OS_IMG)
	@echo "Starting QEMU..."
	$(QEMU) -drive file=$<,format=raw -monitor stdio

# 调试模式
debug: $(OS_IMG)
	@echo "Starting QEMU in debug mode..."
	$(QEMU) -drive file=$<,format=raw -d cpu_reset -D debug.log -no-reboot -no-shutdown

# 清理
clean:
	@echo "Cleaning..."
	$(RM) -r $(OUT_DIR)
	$(RM) $(OS_IMG) debug.log

# 文件结构
init:
	@echo "Creating project structure..."
	$(MKDIR) $(BOOT_DIR) $(KERNEL_DIR) $(OUT_DIR)