#include "interupts.h"

void interupt_setup(){
    PIC_remap(0x20, 0x28);

    IRQ_clear_mask(0);
    asm volatile("sti");
}