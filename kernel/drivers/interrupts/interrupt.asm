global irq_stub
extern time_interrupt_handler

section .text
    irq_stub:
        push rbp
        mov rbp, rsp

        push rax
        push rcx
        push rdx
        push rbx
        push rsi
        push rdi
        push r8
        push r9
        push r10
        push r11

        call time_interrupt_handler

        add rsp, 8
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdi
        pop rsi
        pop rbx
        pop rdx
        pop rcx
        pop rax
        pop rbp

        iretq 