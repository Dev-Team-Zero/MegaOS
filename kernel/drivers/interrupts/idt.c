#include "interrupts.h"


extern void (*interrupt_handlers[IDT_ENTRIES])();
/** 
 * @brief Loads the Interrupt Descriptor Table (IDT) into the CPU.
 * @param idt_ptr* A pointer to the IDT pointer structure.
 */
extern void load_idt(struct idt_ptr*);
/** 
 * @brief The Interrupt Descriptor Table (IDT).
 */
static struct IDT_ENTRY idt[IDT_ENTRIES];

/**
 * @brief Sets an entry in the Interrupt Descriptor Table (IDT).
 * @param vector The interrupt vector number.
 * @param handler The address of the interrupt handler function.
 */
void set_idt_gate(uint8_t vector, uint64_t handler){
    idt[vector].offset_1 = handler & 0xFFFF;
    idt[vector].selector = 0x08;
    idt[vector].ist = 0;
    idt[vector].type_attributes = 0x8E;
    idt[vector].offset_2 = (handler >> 16) & 0xFFFF;
    idt[vector].offset_3 = (handler >> 32) & 0xFFFFFFFF;
    idt[vector].zero = 0;
}

/**
 * @brief Initializes the IDT by setting up the entries for the timer and keyboard interrupts and loading the IDT.
 */
void init_idt(){
    static struct idt_ptr idtp; 
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint64_t)&idt;
    load_idt(&idtp);  
}