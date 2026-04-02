#include "vga.h"
#include "interrupts.h"

void kernel_main(){
    terminal_initialize();
    terminal_write_string("Workss\b\n");
    terminal_write_hex(67);
    interrupt_setup();
    __asm__ volatile("sti");
}