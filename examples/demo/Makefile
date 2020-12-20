TARGET = demo
OBJS = ../../pilcd.o main.o text1.o text2.o
CPPFLAGS = -I../../
CFLAGS = -O2 -Wall -Wextra
LDFLAGS = -lpigpio

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $^

%.o: %.txt
	objcopy -I binary -O elf32-littlearm -B arm $< $@

.PHONY: clean
clean:
	rm *.o
