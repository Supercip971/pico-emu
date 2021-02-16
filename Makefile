DIR_GUARD=mkdir -p $(@D)
BUILD_PATH := ./build/
PROJECT_DIR := ./src/

CFILES := $(shell find $(PROJECT_DIR) -type f -name \*.c)
HFILES := $(shell find $(PROJECT_DIR) -type f -name \*.h)

OBJFILES := $(patsubst %.c, $(BUILD_PATH)/%.o, $(CFILES))
OUTPUT := $(BUILD_PATH)/picemu
SANITIZER := \
	-fsanitize=address \
	-fsanitize=undefined 
GCC_FLAGS := \
	-g \
	-O2 \
  -pthread \
	-I$(PROJECT_DIR) \
	-msse \
	-std=c17 \
	-m64 \
	-Wall \
	-Werror

$(BUILD_PATH)/%.o: %.c $(CFILES) $(HFILES)
	@$(DIR_GUARD)
	@echo " [ C ] $<"
	@gcc -c $(GCC_FLAGS) $(SANITIZER) -o $@ $<

$(OUTPUT): $(OBJFILES)
	@$(DIR_GUARD)
	@echo " [ picemu ] $(OUTPUT) "
	@gcc $(OBJFILES) $(SANITIZER) -pthread -o $(OUTPUT)


clean: 
	rm $(OBJFILES)
	rm $(OUTPUT)


all: $(OUTPUT)

run: $(OUTPUT)
	$(OUTPUT)
