#include "interupts.h"

void PIC_send_EOI(uint8_t irq){
    if(irq >= 8) outb(SLAVE_PIC_COMMAND, PIC_EOI);
    outb(MASTER_PIC_COMMAND, PIC_EOI);
}