ifeq ($(OS),Windows_NT)
  # Пути к инструментам в windows в моём workspace
  CC = ..\..\gcc-arm-none-eabi-6-2017-q1-update-win32\bin\arm-none-eabi-gcc
  LD = ..\..\gcc-arm-none-eabi-6-2017-q1-update-win32\bin\arm-none-eabi-gcc
  CP = ..\..\gcc-arm-none-eabi-6-2017-q1-update-win32\bin\arm-none-eabi-objcopy
  IF = ..\..\gcc-arm-none-eabi-6-2017-q1-update-win32\bin\arm-none-eabi-size
else
  CC = arm-none-eabi-gcc
  LD = arm-none-eabi-gcc
  CP = arm-none-eabi-objcopy
  IF = arm-none-eabi-size
endif

# Скрипт для линкера
LDSCRIPT = -T stm32F.ld

# путь к инклудам
INC += -I inc

# Генерим список объектных файлов
OBJ = $(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))
OBJ += $(patsubst src/%.s,obj/%.o,$(wildcard src/*.s))

OBJ += $(patsubst src/%.c,obj/%.o,$(wildcard src/**/*.c))
OBJ += $(patsubst src/%.s,obj/%.o,$(wildcard src/**/*.s))

OBJ += $(patsubst src/%.c,obj/%.o,$(wildcard src/**/**/*.c))
OBJ += $(patsubst src/%.s,obj/%.o,$(wildcard src/**/**/*.s))

# Настройки компилятора
CFLAGS += -mcpu=cortex-m3 # архитектура
CFLAGS += -mlittle-endian 
CFLAGS += -mthumb # система комманд
#CFLAGS += -Os # оптимизация
CFLAGS += -g
CFLAGS += $(INC)
CFLAGS += -D USE_STDPERIPH_DRIVER -D STM32F10X_MD

# Настройки линкера
LDFLAGS += -mcpu=cortex-m3
LDFLAGS += -mlittle-endian
LDFLAGS += -mthumb
LDFLAGS += $(LDSCRIPT)
LDFLAGS += -Wl,--gc-section
LDFLAGS += -Wl,-Map=MAP.map -Wl,--print-memory-usage

######################################################################
all: start info
	@echo "END"

start: main.elf
	@echo "OUT"
	@echo "********************************************************"
	$(CP) -Oihex main.elf main.hex 
	@echo "********************************************************"

main.elf: $(OBJ)
	@echo "LINK"
	@echo "********************************************************"
	$(LD) $(LDFLAGS) $(OBJ) -o main.elf
	@echo "********************************************************"

obj/%.o: src/%.c
	@echo "COMPILE C"
	@echo "********************************************************"
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "********************************************************"

obj/%.o: src/%.s
	@echo "COMPILE S"
	@echo "********************************************************"
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "********************************************************"

info:
	@echo "INFO"
	@echo "********************************************************"
	$(IF) main.elf
	@echo "********************************************************"
