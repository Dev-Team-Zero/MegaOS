#include "vga.h"
#include "interupts.h"

void kernel_main(){
    terminal_initialize();
    terminal_write_string("Workss\b\n");
    terminal_write_hex(67);
    PIC_remap(0x20, 0x28);
}