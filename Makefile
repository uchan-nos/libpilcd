TARGET = pilcd
OBJS = main.o lcd.o
#CFLAGS = -O0 -Wall -Wextra -mcpu=cortex-a72 --target=aarch32
#CFLAGS = -O2 -Wall -Wextra --target=armv7l-arm-none-eabi -mcpu=cortex-a72
CFLAGS = -O2 -Wall -Wextra
LDFLAGS = -lpigpio

all: $(TARGET)

$(TARGET): $(OBJS) text1.o text2.o
	$(CC) $(LDFLAGS) -o $(TARGET) $^

%.o: %.txt
	objcopy -I binary -O elf32-littlearm -B arm $< $@

.PHONY: clean
clean:
	rm *.o
