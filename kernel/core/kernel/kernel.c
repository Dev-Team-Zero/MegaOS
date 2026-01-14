#include "vga.h"

void kernel_main(){
    terminal_initialize();
    terminal_write_string("Works\n");
    while (1){
        asm volatile("hlt");
    }
}