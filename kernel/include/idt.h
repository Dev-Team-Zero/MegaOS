#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct IDT_ENTRY {
   uint16_t offset_1;       
   uint16_t selector;        
   uint8_t  ist;            
   uint8_t  type_attributes; 
   uint16_t offset_2;        
   uint32_t offset_3;       
   uint32_t zero;        
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

void set_idt_gate(uint8_t vector, uint64_t handler);
void init_idt();


#endif