CROSS = arm-none-eabi

CFLAGS = -O3 -I./include/ -nostdlib -static -ffreestanding -nostartfiles \
  -march=armv6-m -mthumb -Wall -Wextra -Werror

LDFLAGS = -T include/blink.ld -Wl,--gc-sections -Wl,-Map=build/blink.map

OBJ = build/%.o

GENHDR = tools/crc32table.h

.PHONY: all build clean

.INTERMEDIATE: tools/gen_crc32table

all: build/blink.elf build/blink.bin build/blink.uf2

build:
	mkdir -p build

# Tool building
tools/%: tools/%.c
	gcc -O3 $< -o $@

# CRC table header generation
$(GENHDR): tools/gen_crc32table
	$< > $@

$(OBJ): build | $(GENHDR)
	$(CROSS)-gcc $(CFLAGS) -c src/*.c -o $@

build/blink.elf: $(OBJ) tools/inject_crc32
	$(CROSS)-gcc $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ)
	tools/inject_crc32 $@

build/blink.bin: build/blink.elf
	$(CROSS)-objcopy -O binary $< $@

build/blink.uf2: build/blink.bin tools/bin2uf2_pico
	tools/bin2uf2_pico $< $@

clean:
	rm -f -r build
