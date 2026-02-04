bits 32

%define ALIGN 1<<0
%define MEMINFO 1<<1
%define FLAGS (ALIGN | MEMINFO)
%define MAGIC 0x1BADB002
%define CHECKSUM -(MAGIC + FLAGS)

extern long_mode_start

section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .bss
align 4096
l4: resq 512
l3: resq 512
l2: resq 512
l1: resq 512
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
_start:

    setup_page_tables:
        mov eax, l3
        or eax, 0b11
        mov [l4], eax

        mov eax, l2
        or eax, 0b11
        mov [l3], eax

        mov ecx, 0
    .loop:
        imul eax, ecx, 0x200000
        or eax, 0b10000011 
        mov [l2 + ecx * 8], eax

        inc ecx
        cmp ecx, 512 
        jne .loop 

        


    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov eax, l4
    mov cr3, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    jmp 0x08:long_mode_start
