# ============================================================
# Ienta — Firmware Makefile
# Target: STM32F401RE (Nucleo-F401RE board)
# Toolchain: arm-none-eabi-gcc (ARM GNU Toolchain)
# ============================================================

# ---- Build profile (override on command line: make BUILD=debug) ----
BUILD ?= dev

ifeq ($(BUILD),dev)
  OPT = -Og
else ifeq ($(BUILD),debug)
  OPT = -O0
else ifeq ($(BUILD),release)
  OPT = -O2
else
  $(error Unknown BUILD=$(BUILD). Use dev, debug, or release.)
endif

# ---- Project identity ----
TARGET   = ienta
MCU      = STM32F401xE

# ---- Toolchain ----
PREFIX   = arm-none-eabi-
CC       = $(PREFIX)gcc
AS       = $(PREFIX)gcc -x assembler-with-cpp
LD       = $(PREFIX)gcc
OBJCOPY  = $(PREFIX)objcopy
SIZE     = $(PREFIX)size

# ---- Directories ----
BUILD_DIR  = build
CORE_DIR   = Core
CMSIS_DIR  = Drivers/CMSIS
LINKER_DIR = Linker

# ---- Source files ----
C_SOURCES = \
  $(CORE_DIR)/Src/main.c \
  $(CMSIS_DIR)/Device/ST/STM32F4xx/Source/system_stm32f4xx.c

ASM_SOURCES = \
  $(CMSIS_DIR)/Device/ST/STM32F4xx/Source/startup_stm32f401xe.s

# ---- Include paths ----
C_INCLUDES = \
  -I$(CORE_DIR)/Inc \
  -I$(CMSIS_DIR)/Device/ST/STM32F4xx/Include \
  -I$(CMSIS_DIR)/Include

# ---- Preprocessor defines ----
C_DEFS = -D$(MCU)

# ---- CPU / FPU flags (Cortex-M4F) ----
CPU      = -mcpu=cortex-m4
FPU      = -mfpu=fpv4-sp-d16
FLOAT_ABI = -mfloat-abi=hard
MCU_FLAGS = $(CPU) -mthumb $(FPU) $(FLOAT_ABI)

# ---- Compiler flags ----
CFLAGS  = $(MCU_FLAGS) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Wextra -fdata-sections -ffunction-sections -g3 -gdwarf-2

# ---- Assembler flags ----
ASFLAGS = $(MCU_FLAGS) $(OPT) -Wall -fdata-sections -ffunction-sections -g3 -gdwarf-2

# ---- Linker flags ----
LDSCRIPT = $(LINKER_DIR)/STM32F401RETX_FLASH.ld
LDFLAGS  = $(MCU_FLAGS) -T$(LDSCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs

# ---- Object files (mirror source tree under build/) ----
OBJECTS  = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))

# Tell make where to find the original sources for those flat object names
vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# ============================================================
# Rules
# ============================================================

.PHONY: all clean flash size info

all: $(BUILD_DIR)/$(TARGET).bin size

# Compile C sources
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "  CC      $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Assemble startup file
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	@echo "  AS      $<"
	@$(AS) $(ASFLAGS) -c $< -o $@

# Link everything into the ELF
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@echo "  LD      $@"
	@$(LD) $(OBJECTS) $(LDFLAGS) -o $@

# Convert ELF → raw binary for flashing
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	@echo "  OBJCOPY $@"
	@$(OBJCOPY) -O binary $< $@

# Create the build directory if missing
$(BUILD_DIR):
	@mkdir -p $@

# Print size of each section in the ELF
size: $(BUILD_DIR)/$(TARGET).elf
	@echo ""
	@$(SIZE) $(BUILD_DIR)/$(TARGET).elf

# Flash the binary to the chip via ST-LINK
flash: $(BUILD_DIR)/$(TARGET).bin
	@echo "  FLASH   $<"
	st-flash --reset write $< 0x08000000

# Show build configuration
info:
	@echo "BUILD    = $(BUILD)"
	@echo "OPT      = $(OPT)"
	@echo "MCU      = $(MCU)"
	@echo "TARGET   = $(TARGET)"
	@echo "OBJECTS  = $(OBJECTS)"

# Wipe build artifacts
clean:
	@echo "  CLEAN"
	@rm -rf $(BUILD_DIR)
