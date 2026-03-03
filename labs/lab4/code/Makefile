# ── Metadata ──────────────────────────────────────────────────────────
SPACE := $(null) $(null)
APP_NAME = $(notdir $(subst $(SPACE),-,$(CURDIR)))

# Build target (set by VS Code task via BUILD_TARGET=N on the command line):
#   0 = simulator (default)
#   1 = hardware
BUILD_TARGET ?= 0

# ── Directories ───────────────────────────────────────────────────────
BUILD_DIR  = build
OBJ_DIR    = $(BUILD_DIR)/obj
TFTLIB_DIR = tftlib

LINKER_SCRIPT = device/md307-ram.x

# ── Output files ──────────────────────────────────────────────────────
TARGET_EXEC = $(BUILD_DIR)/$(APP_NAME).elf
TARGET_S19  = $(BUILD_DIR)/$(APP_NAME).s19
TARGET_LST  = $(BUILD_DIR)/$(APP_NAME).lst

# ── tftlib archives ───────────────────────────────────────────────────
TFTLIB_SIM = $(BUILD_DIR)/tftlib-sim.a
TFTLIB_HW  = $(BUILD_DIR)/tftlib-hw.a

ifeq ($(BUILD_TARGET),0)
  TFTLIB = $(TFTLIB_SIM)
else
  TFTLIB = $(TFTLIB_HW)
endif


# tftlib source files — each variant contains only what it needs
TFTLIB_SRCS_SIM := $(TFTLIB_DIR)/tft.s
TFTLIB_SRCS_HW  := $(TFTLIB_DIR)/tft-md307.c
TFTLIB_OBJ_SIM  := $(TFTLIB_SRCS_SIM:%=$(OBJ_DIR)/sim/%.o)
TFTLIB_OBJ_HW   := $(TFTLIB_SRCS_HW:%=$(OBJ_DIR)/hw/%.o)

# ── Main program sources (only main.c + startup.s remain in src/) ─────
MAIN_SRCS := src/main.c src/startup.s
MAIN_OBJS := $(MAIN_SRCS:%=$(OBJ_DIR)/%.o)

DEPS := $(MAIN_OBJS:.o=.d) $(TFTLIB_OBJ_SIM:.o=.d) $(TFTLIB_OBJ_HW:.o=.d)

# ── Toolchain ─────────────────────────────────────────────────────────
CC      = "$(GCC_PATH)riscv32-unknown-elf-gcc"
AR      = "$(GCC_PATH)riscv32-unknown-elf-ar"
OBJCOPY = "$(GCC_PATH)riscv32-unknown-elf-objcopy"
OBJDUMP = "$(GCC_PATH)riscv32-unknown-elf-objdump"

# ── Compiler / linker flags ───────────────────────────────────────────
INC_DIRS = $(TFTLIB_DIR)
LIB_DIRS = .

CFLAGS  = -g -O3 -Wall -Wextra -std=c99 -MMD -march=rv32imf_zicsr -mabi=ilp32f \
          $(addprefix -I, $(INC_DIRS))
ASFLAGS = -g -march=rv32imf_zicsr -mabi=ilp32f
LDFLAGS = -T$(LINKER_SCRIPT) -Wl,--no-warn-rwx-segments -march=rv32imf_zicsr \
          -mabi=ilp32f -nostdlib -nostartfiles --specs=nano.specs \
          $(addprefix -L, $(LIB_DIRS))
LDLIBS  = -l:md307-runtime.a -lgcc -lc_nano

# ── Windows / Unix helpers ────────────────────────────────────────────
ifeq ($(OS), Windows_NT)
    MKDIR = powershell New-Item -ItemType Directory -Force -Path
    RM    = powershell Remove-Item -Recurse -Force
else
    MKDIR = mkdir -p
    RM    = rm -rf
endif

# ── Phony targets ─────────────────────────────────────────────────────
.PHONY: all tftlibs clean

# ── Default: build ELF for the selected BUILD_TARGET ─────────────────
all: $(TARGET_EXEC)

$(TARGET_EXEC): $(MAIN_OBJS) $(TFTLIB)
	$(CC) $(MAIN_OBJS) $(TFTLIB) $(LDFLAGS) $(LDLIBS) -o $@
	$(OBJCOPY) -S -O srec $@ $(TARGET_S19)
	$(OBJDUMP) -d -S --dwarf-depth=1 $@ > $(TARGET_LST)

# ── Convenience: rebuild both library variants at once ────────────────
tftlibs: $(TFTLIB_SIM) $(TFTLIB_HW)

# ── tftlib-sim.a  (tft.s only) ───────────────────────────────────────
$(TFTLIB_SIM): $(TFTLIB_OBJ_SIM)
	$(AR) rcs $@ $^

$(OBJ_DIR)/sim/$(TFTLIB_DIR)/%.o: $(TFTLIB_DIR)/%
	$(MKDIR) $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

# ── tftlib-hw.a   (tft-md307.c only) ─────────────────────────────────
$(TFTLIB_HW): $(TFTLIB_OBJ_HW)
	$(AR) rcs $@ $^

$(OBJ_DIR)/hw/$(TFTLIB_DIR)/%.o: $(TFTLIB_DIR)/%
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ── Main object files ─────────────────────────────────────────────────
$(OBJ_DIR)/%.o: %
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)

-include $(DEPS)