#include "vga.h"
#include "interrupts.h"
#include "heap.h"

void kernel_main(){
    terminal_initialize();
    terminal_write_string("Workss\b\n");
    terminal_write_hex(67);
    interrupt_setup();
    init_heap();
    kmalloc(4096);
    terminal_write_string("um\n");
}