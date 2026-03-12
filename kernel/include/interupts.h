#include <stddef.h>
#include <stdint.h>
#include "io.h"

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
void PIC_remap(uint8_t offset1, uint8_t offset2);
void PIC_send_EOI(uint8_t irq);
void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);
void interupt_setup();