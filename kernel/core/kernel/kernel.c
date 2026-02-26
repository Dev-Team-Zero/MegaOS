#include "vga.h"

void kernel_main(){
    terminal_initialize();
    terminal_write_string("Workss\b\n");
    
    // uint64_t val = 0;
    //  __asm__ volatile (
    //     "movb $0x01, %%ah;"  // Suffix 'b' pentru byte, '$' pentru constante
    //     "int $0x16;"         // Aceasta va cauza un Crash/Reboot în Long Mode!
    //     "movzbq %%al, %0;"   // movzbq = Move cu Zero-Extend de la Byte la Quadword (64-bit)
    //     : "=r" (val)         // Ieșire
    //     :                    // Intrări
    //     : "rax"              // Am marcat rax ca modificat (pentru că ah și al fac parte din el)
    // );

    terminal_write_hex(val);
}