ARMGNU ?= aarch64-linux-gnu

COPS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASMOPS = -Iinclude

BUILD_DIR = build
SRC_DIR = .

all : arm_smp

run : arm_smp
	qemu-system-aarch64 -M raspi3b -nographic -serial null -serial mon:stdio -kernel kernel8.elf

clean :
	rm -rf $(BUILD_DIR) *.img *.bin *.elf

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

arm_smp: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	 $(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o kernel8.elf  $(OBJ_FILES)
	 $(ARMGNU)-objcopy kernel8.elf -O binary kernel8.bin
