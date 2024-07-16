# Compiler and linker
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Microcontroller specific settings
MCU = atmega328pb
F_CPU = 16000000UL
BAUD = 9600

# Programmer settings
PROGRAMMER = xplainedmini

# Include directories
INCLUDE_DIRS = -Ilib/EmbeddedCommon/drivers

# Source directories under the root directories
LIB_SRC_DIRS := \
	lib/EmbeddedCommon/drivers/328pb \
	lib/EmbeddedCommon/drivers/mpu6050

PROJECT_SRC_DIRS := \
	src

# Combine all source directories
SRC_DIRS := \
	$(LIB_SRC_DIRS) \
	$(PROJECT_SRC_DIRS)

# Output directory
BUILD_DIR = build

# Source files
SRC_FILES = $(wildcard $(addsuffix /*.c, $(SRC_DIRS)))

# Object files in build directory
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Output files
TARGET_NAME = target
TARGET = $(BUILD_DIR)/${TARGET_NAME}.elf
HEX = $(BUILD_DIR)/${TARGET_NAME}.hex

# Compiler flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Os ${INCLUDE_DIRS}
LDFLAGS = -mmcu=$(MCU)

# Default target
all: $(BUILD_DIR) $(TARGET) $(HEX)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@echo "Creating build folder"

# Link objects to create the final executable
$(TARGET): $(OBJS) | $(BUILD_DIR)
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating executable $@"

# Convert ELF to HEX
$(HEX): $(TARGET)
	@$(OBJCOPY) -O ihex $< $@
	@echo "Generating $@"

# Compile C source files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Flash target to program the microcontroller
flash: $(HEX)
	@echo "Flashing $(HEX) to device"
	@$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -U flash:w:$(HEX)

# Clean build files
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaning..."

.PHONY: all clean flash
