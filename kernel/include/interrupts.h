#include <stddef.h>
#include <stdint.h>
#include "io.h"

#define IDT_ENTRIES 256

#define MASTER_PIC_COMMAND  0x20
#define MASTER_PIC_DATA     0x21
#define SLAVE_PIC_COMMAND   0xA0
#define SLAVE_PIC_DATA      0xA1
#define PIC_EOI             0x20

#define ICW1_ICW4           0x1
#define ICW1_SINGLE         0x2
#define ICW1_INTERVAL4      0x4
#define ICW1_LEVEL          0x8
#define ICW1_INIT           0x10

#define ICW4_8060           0x1
#define ICW4_AUTO           0x2
#define ICW4_BUF_SLAVE      0x8
#define ICW4_BUF_MASTER     0xC
#define ICW4_SFNM           0x10

#define CASCADE_IRQ         2

void io_wait(void);
void pit_init();
void PIC_remap(uint8_t offset1, uint8_t offset2);
void PIC_send_EOI(uint8_t irq);
void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);
void interrupt_setup();

static const char keyboard_keymap[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', //0x00-0x0E
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  //0x0F-0x1C
    '0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 41, //0x1D-0x29
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, //0x2A-0x35
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, //0x36-0x3F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0x40-0x49
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0x4A-0x53
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0x54-0x5B 
};

char scancode_to_ascii(uint8_t scancode);