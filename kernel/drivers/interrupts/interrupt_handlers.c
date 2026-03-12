#include "interrupts.h"
#include "idt.h"

void (*interrupt_handlers[IDT_ENTRIES])();

volatile uint64_t ticks = 0;

void time_interrupt_handler(){
    ticks++;
    PIC_send_EOI(0);
}