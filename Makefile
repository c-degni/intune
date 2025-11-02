# Simple top-level Makefile to automate CMake build and run with dynamic WAV input

APP_NAME := SignalProcessor
BUILD_DIR := build
file ?=             # user-provided argument (e.g., make run file=mixkit-crowd.wav)

# Default target
all: run

# Configure and build using CMake
$(BUILD_DIR)/$(APP_NAME):
	@echo "🔧 Configuring and building..."
	@cmake -S . -B $(BUILD_DIR) >/dev/null
	@cmake --build $(BUILD_DIR) -- -j4

# Run with dynamic WAV argument
run: $(BUILD_DIR)/$(APP_NAME)
ifeq ($(strip $(file)),)
	@echo "❌ Error: please specify a WAV file, e.g. 'make run file=audio.wav'"
	@exit 1
else
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR) -- -j4
	@echo "🚀 Running $(APP_NAME) with $(file)..."
	@$(BUILD_DIR)/$(APP_NAME) $(abspath $(file))
endif

# Clean build artifacts
clean:
	@echo "🧹 Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

.PHONY: all run clean
