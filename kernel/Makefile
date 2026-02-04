kernel_source_files := $(shell find core/kernel -name *.c)
kernel_object_files := $(patsubst core/kernel/%.c, build/kernel/%.o, $(kernel_source_files))

x86_64_c_source_files := $(shell find drivers -name *.c)
x86_64_c_object_files := $(patsubst drivers/%.c, build/x86_64/%.o, $(x86_64_c_source_files))

x86_64_asm_source_files := $(shell find drivers -name *.asm)
x86_64_asm_object_files := $(patsubst drivers/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

x86_64_boot_asm := arch/x86_64/boot/boot.asm
x86_64_boot_long_mode_asm := arch/x86_64/boot/long_mode.asm
x86_64_boot_object_files := build/x86_64/boot.o build/x86_64/long_mode.o

x86_64_object_files := $(x86_64_boot_object_files) $(x86_64_c_object_files) $(x86_64_asm_object_files)

$(kernel_object_files): build/kernel/%.o : core/kernel/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I include -ffreestanding $(patsubst build/kernel/%.o, core/kernel/%.c, $@) -o $@

$(x86_64_c_object_files): build/x86_64/%.o : drivers/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I include -ffreestanding $(patsubst build/x86_64/%.o, drivers/%.c, $@) -o $@

$(x86_64_asm_object_files): build/x86_64/%.o : drivers/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst build/x86_64/%.o, drivers/%.asm, $@) -o $@

build/x86_64/boot.o: $(x86_64_boot_asm)
	mkdir -p $(dir $@) && \
	nasm -f elf64 $< -o $@

build/x86_64/long_mode.o: $(x86_64_boot_long_mode_asm)
	mkdir -p $(dir $@) && \
	nasm -f elf64 $< -o $@

.PHONY: build-x86_64
build-x86_64: $(kernel_object_files) $(x86_64_object_files)
	mkdir -p dist/x86_64 && \
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T arch/x86_64/boot/linker.ld $(kernel_object_files) $(x86_64_object_files) && \
	cp dist/x86_64/kernel.bin targets/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/iso

.PHONY: run
run: 
	qemu-system-x86_64 -nographic -serial mon:stdio -boot d -cdrom dist/x86_64/kernel.iso -m 512