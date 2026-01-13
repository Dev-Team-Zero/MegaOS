kernel_source_files := $(shell find core/kernel -name *.c)
kernel_object_files := $(patsubst core/kernel/%.c, build/kernel/%.o, $(kernel_source_files))

x86_64_c_source_files := $(shell find drivers/console -name *.c)
x86_64_c_object_files := $(patsubst drivers/console/%.c, build/x86_64/%.o, $(x86_64_c_source_files))

# x86_64_asm_source_files := $(shell find src/impl/x86_64 -name *.asm)
# x86_64_asm_object_files := $(patsubst src/impl/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

x86_64_object_files := $(x86_64_c_object_files) # $(x86_64_asm_object_files)

$(kernel_object_files): build/kernel/%.o : core/kernel/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I include -ffreestanding $(patsubst build/kernel/%.o, core/kernel/%.c, $@) -o $@

$(x86_64_c_object_files): build/x86_64/%.o : drivers/console/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I include -ffreestanding $(patsubst build/x86_64/%.o, drivers/console/%.c, $@) -o $@

# $(x86_64_asm_object_files): build/drivers/%.o : drivers/%.asm
# 	mkdir -p $(dir $@) && \
# 	nasm -f elf64 $(patsubst build/drivers/%.o, drivers/%.asm, $@) -o $@

.PHONY: build-x86_64
build-x86_64: $(kernel_object_files) $(x86_64_object_files)
	mkdir -p dist/x86_64 && \
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T arch/x86_64/boot/linker.ld $(kernel_object_files) $(x86_64_object_files) && \
	cp dist/x86_64/kernel.bin arch/x86_64/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso arch/x86_64/boot