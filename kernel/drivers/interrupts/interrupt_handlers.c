#include "interrupts.h"

void (*interrupt_handlers[IDT_ENTRIES])();

volatile uint64_t ticks = 0;

void time_interrupt_handler(){
    ticks++;
    PIC_send_EOI(0);
}

void keyboard_interrupt_handler(){
    uint8_t status = inb(0x64);
    if(!(status & 0x01)){
        PIC_send_EOI(1);
        return;
    }

    uint8_t scancode = inb(0x60);
    if (scancode >= 0x80){
        PIC_send_EOI(1);
        return;
    }

    char key = scancode_to_ascii(scancode);
    if (!key){
        PIC_send_EOI(1);
        return;
    }

    terminal_put_char(key);
    console_process_key(key);


    PIC_send_EOI(1);
}

char scancode_to_ascii(uint8_t scancode) {
    if (scancode > 0x7F) return 0;
    return keyboard_keymap[scancode];
} 