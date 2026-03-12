#include "interrupts.h"
#include "idt.h"
#include "vga.h"

extern void time_interrupt_handler();
extern void (*interrupt_handlers[IDT_ENTRIES])();
extern void irq_stub(void);

void PIC_send_EOI(uint8_t irq){
    if(irq >= 8) outb(SLAVE_PIC_COMMAND, PIC_EOI);
    outb(MASTER_PIC_COMMAND, PIC_EOI);
}

void io_wait(void){
    outb(0x80, 0);
}

void PIC_remap(uint8_t offset1, uint8_t offset2){
    terminal_write_string("\nMaster PIC Init.\n");
    outb(MASTER_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    terminal_write_string("Slave PIC Init.\n");
    outb(SLAVE_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    terminal_write_string("Interupt vector offset.\n");
    outb(MASTER_PIC_DATA, offset1);
    io_wait();
    outb(SLAVE_PIC_DATA, offset2);
    io_wait();
    terminal_write_string("Unmasking Slave PIC on IRQ: \n");
    char str[2];
    str[0] = '0' + CASCADE_IRQ;
    str[1] = 0;
    terminal_write_string(str);
    terminal_write_string(".\n");
    outb(MASTER_PIC_DATA, 1 << CASCADE_IRQ);
    io_wait();
    outb(SLAVE_PIC_DATA, CASCADE_IRQ);
    io_wait();
    outb(MASTER_PIC_DATA, ICW4_8060);
    io_wait();
    outb(SLAVE_PIC_DATA, ICW4_8060);
    io_wait();


    outb(MASTER_PIC_DATA, 0xFE);
    outb(SLAVE_PIC_DATA, 0xFF);
    terminal_write_string("PIC remap compleat.\n");
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

void interupt_setup(){
    set_idt_gate(0x20, (uint64_t)irq_stub);
    init_idt();
    PIC_remap(0x20, 0x28);
    interrupt_handlers[0x20] = time_interrupt_handler;
    pit_init();
    IRQ_clear_mask(0);
    __asm__ volatile("sti");
}

void pit_init(){
    uint32_t freq = 100;
    uint16_t divisor = 1193180 / freq;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}