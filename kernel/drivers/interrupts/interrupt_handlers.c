#include "interrupts.h"
#include "idt.h"
#include "vga.h"

void (*interrupt_handlers[IDT_ENTRIES])();

volatile uint64_t ticks = 0;

void time_interrupt_handler(){
    ticks++;
    PIC_send_EOI(0);
}

void keyboard_interrupt_handler(){
    uint8_t status = inb(0x64);
    if(status & 0x01){
        uint8_t scancode = inb(0x60);
        if(scancode < 0x0) return;
        terminal_write_string("Key pressed: ");
        terminal_write_hex(scancode);
        terminal_write_string("\n");
    }

    PIC_send_EOI(1);
}