global irq_stub
global keyboard_stub
global exception_stub_0
extern time_interrupt_handler
extern keyboard_interrupt_handler
extern exception_handler
global exception_stub_13
global exception_stub_14
global exception_stub_generic

irq_stub:
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rbx, rsp
    and rsp, -16
    call time_interrupt_handler
    mov rsp, rbx

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax
    iretq

keyboard_stub:
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
    push r12
    push r13
    push r14
    push r15

    mov rbx, rsp
    and rsp, -16

    call keyboard_interrupt_handler

    mov rsp, rbx

    pop r15
    pop r14
    pop r13
    pop r12
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
        
exception_stub_0:
    push 0
    lea rsi, [rel div_error_msg]
    mov rdi, 0
    call exception_handler

exception_stub_13:
    push 0
    lea rsi, [rel GPF_msg]
    mov rdi, 0xD
    call exception_handler

exception_stub_14:
    push 0
    lea rsi, [rel page_fault_msg]
    mov rdi, 0xE
    call exception_handler

exception_stub_generic:
    push 0 
    lea rsi, [rel generic_msg]
    mov rdi, 19
    call exception_handler

div_error_msg: db "Division by zero", 0
GPF_msg: db "GPF", 0
page_fault_msg: db "page fault", 0
generic_msg: db "uknown fault", 0