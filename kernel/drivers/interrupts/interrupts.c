#include "interrupts.h"

extern void time_interrupt_handler();
extern void keyboard_interrupt_handler();
extern void (*interrupt_handlers[IDT_ENTRIES])();
extern void irq_stub();
extern void keyboard_stub();
extern void exception_stub_0();
extern void exception_stub_13();
extern void exception_stub_14();
extern void exception_stub_generic();

/**
 * @brief Sends an End-of-Interrupt (EOI) signal to the Programmable Interrupt Controller (PIC).
 * @param irq The interrupt request line number.
 */
void PIC_send_EOI(uint8_t irq){
    if(irq >= 8) outb(SLAVE_PIC_COMMAND, PIC_EOI);
    outb(MASTER_PIC_COMMAND, PIC_EOI);
}

/**
 * @brief Waits for an I/O operation to complete by sending a dummy byte to an unused port.
 */
void io_wait(void){
    outb(0x80, 0);
}

/**
 * @brief Remaps the Programmable Interrupt Controller (PIC) interrupt vectors.
 * @param offset1 The offset for the master PIC.
 * @param offset2 The offset for the slave PIC.
 */
void PIC_remap(uint8_t offset1, uint8_t offset2){
    terminal_write_string("\nMaster PIC Init.\n");
    outb(MASTER_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    terminal_write_string("Slave PIC Init.\n");
    outb(SLAVE_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    terminal_write_string("Interrupt vector offset.\n");
    outb(MASTER_PIC_DATA, offset1);
    io_wait();
    outb(SLAVE_PIC_DATA, offset2);
    io_wait();
    terminal_write_string("Configuring PIC cascade on IRQ: \n");
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
    
    outb(MASTER_PIC_DATA, 0xFF);
    outb(SLAVE_PIC_DATA, 0xFF);
    io_wait();

    terminal_write_string("PIC remap complete.\n");
}

/**
 * @brief Sets the mask for a specific IRQ line.
 * @param IRQline The IRQ line number.
 */
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

/**
 * @brief Clears the mask for a specific IRQ line.
 * @param IRQline The IRQ line number.
 */
void IRQ_clear_mask(uint8_t IRQline){
    uint16_t port;
    uint8_t value;
    terminal_write_string("unmasking ");
    terminal_write_hex(IRQline);

    if(IRQline < 8) {
        port = MASTER_PIC_DATA;
    } else {
        port = SLAVE_PIC_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    terminal_write_string(" ... ");
    outb(port, value);   
    terminal_write_string(" ... done;\n");     
}

/**
 * @brief Sets up the interrupt handling system.
 */
void interrupt_setup() {
    init_idt();
    PIC_remap(0x20, 0x28);
    
    for (uint8_t i = 0; i < 32; i++) {
        set_idt_gate(i, (uint64_t)exception_stub_generic);
    }
    for (uint8_t i = 0; i < 16; ++i) {
        set_idt_gate(0x20 + i, (uint64_t)irq_stub);
        interrupt_handlers[0x20 + i] = time_interrupt_handler;
    }

    set_idt_gate(0x00, (uint64_t)exception_stub_0);
    set_idt_gate(0x0D, (uint64_t)exception_stub_13);
    set_idt_gate(0x0E, (uint64_t)exception_stub_14);
    terminal_write_string("6\n");

    set_idt_gate(0x21, (uint64_t)keyboard_stub);
    interrupt_handlers[0x21] = keyboard_interrupt_handler;

    pit_init();
    
    __asm__ volatile("sti");
    terminal_write_string("sti survived\n");

    IRQ_clear_mask(0);
    IRQ_clear_mask(1);
}

/**
 * @brief Initializes the Programmable Interval Timer (PIT) to generate timer interrupts at a specific frequency.
 */
void pit_init(){
    uint32_t freq = 100;
    uint16_t divisor = 1193180 / freq;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}