#include "interupts.h"

void PIC_send_EOI(uint8_t irq){
    if(irq >= 8) outb(SLAVE_PIC_COMMAND, PIC_EOI);
    outb(MASTER_PIC_COMMAND, PIC_EOI);
}

void io_wait(void){
    outb(0x80, 0);
}

void PIC_remap(int offset1, int offset2){
    outb(MASTER_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(SLAVE_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(MASTER_PIC_DATA, offset1);
    io_wait();
    outb(SLAVE_PIC_DATA, offset2);
    io_wait();
    outb(MASTER_PIC_DATA, 1 << CASCADE_IRQ);
    io_wait();
    outb(SLAVE_PIC_DATA, CASCADE_IRQ);
    io_wait();
    outb(SLAVE_PIC_DATA, ICW4_8060);
    io_wait();
    outb(MASTER_PIC_DATA, 0);
    outb(SLAVE_PIC_DATA, 0);
}

void IRQ_set_mask(uint8_t IRQline){
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = MASTER_PIC_DATA;
    } else {
        port = SLAVE_PIC_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}

void IRQ_clear_mask(uint8_t IRQline){
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = MASTER_PIC_DATA;
    } else {
        port = SLAVE_PIC_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}