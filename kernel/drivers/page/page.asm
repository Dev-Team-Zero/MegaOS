global loadPage
global enablePage 

section .text
align 16

loadPage:
    push ebp
    mov esp, ebp
    mov [esp+8], eax
    mov eax, cr3
    mov ebp, esp
    pop ebp
    ret

enablePage:
    push ebp
    mov esp, ebp
    mov cr0, eax
    or 0x80000000, eax
    mov eax, cr0
    mov ebp, esp
    pop ebp
    ret