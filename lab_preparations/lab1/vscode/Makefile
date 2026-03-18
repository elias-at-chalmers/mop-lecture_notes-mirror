# name of the app (name of working directory)
SPACE := $(null) $(null)
APP_NAME = $(notdir $(subst $(SPACE),-,$(CURDIR)))

# output files
TARGET_EXEC = $(BUILD_DIR)/$(APP_NAME).elf
TARGET_S19 = $(BUILD_DIR)/$(APP_NAME).s19
TARGET_LST = $(BUILD_DIR)/$(APP_NAME).lst

# linker script
LINKER_SCRIPT = device/md307-ram.x

# directories containing source files and libraries
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
SRC_DIRS = src lib/src
INC_DIRS = inc lib/inc
LIB_DIRS = .

# paths to source, object and dependency files
SRCS := $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c) $(wildcard $(d)/*.s $(wildcard $(d)/*.S)))
C_SRCS := $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c))
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
ASM_FILES := $(C_SRCS:%.c=$(BUILD_DIR)/asm/%.s)

# riscv gcc toolchain paths
CC = "$(GCC_PATH)riscv32-unknown-elf-gcc"
OBJCOPY = "$(GCC_PATH)riscv32-unknown-elf-objcopy"
OBJDUMP = "$(GCC_PATH)riscv32-unknown-elf-objdump"


# compiler and linker flags
CFLAGS += -g -Wall -Wextra -std=c99 -MMD -march=rv32imf_zicsr  -mabi=ilp32f $(addprefix -I, $(INC_DIRS))
LDFLAGS += -T$(LINKER_SCRIPT) -Wl,--no-warn-rwx-segments -march=rv32imf_zicsr  -mabi=ilp32f  -nostdlib -nostartfiles --specs=nano.specs
LDFLAGS += $(addprefix -L, $(LIB_DIRS))
ASFLAGS += -g

# compiler and standard libraries
LDLIBS += -l:md307-runtime.a -lgcc -lc_nano

# check if os is windows, imitate UNIX behavior
ifeq ($(OS), Windows_NT)
    MKDIR = powershell mkdir -Force
    RM = powershell rm -Force
else
    MKDIR = mkdir -p
endif


all: $(TARGET_EXEC)

# link object files into executable, generate s-records and debug files
$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@ 
	$(OBJCOPY) -S -O srec $@ $(TARGET_S19)
	$(OBJDUMP) -d -S --dwarf-depth=1 $@ > $(TARGET_LST)

# compile .s and .c-files
$(OBJ_DIR)/%.o: %
	$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# generate assembly files from C sources
$(BUILD_DIR)/asm/%.s: %.c
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -S $< -o $@

# generate compact assembly files without debug symbols
$(BUILD_DIR)/asm-compact/%.s: %.c
	$(MKDIR) $(dir $@)
	$(CC) $(filter-out -g -MMD,$(CFLAGS)) -fno-asynchronous-unwind-tables -S $< -o $@

# target to generate all assembly files
asm: $(ASM_FILES)
	@echo "Assembly files generated in $(BUILD_DIR)/asm/"

# target to generate compact assembly files
asm-compact: $(C_SRCS:%.c=$(BUILD_DIR)/asm-compact/%.s)
	@echo "Compact assembly files generated in $(BUILD_DIR)/asm-compact/"

.PHONY: clean asm asm-compact

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)