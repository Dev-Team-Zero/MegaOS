#include "print.h"
#include "memory.h"
#include "idt.h"
#include "pit.h"
#include "interrupts.h"
#include "terminal.h"
#include "ata_pio_read.h"
#include "file.h"
#include "io.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("Initializing memory manager...\n");
    init_memory_manager(1024 * 1024 * 512); //512MB
    print_str("Memory manager initialized.\n");

    print_str("Initializing heap...\n");
    init_heap();
    print_str("Heap initialized.\n");
    void* page = alloc_page();
    if (!page) {
        print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
        print_str("Failed to allocate page\n");
        return;
    }
    print_str("Page allocated successfully\n");

    void* ptr = kmalloc(512 * 1024); //512KB
    if (!ptr) {
        print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
        print_str("Failed to allocate 512 kilobytes from heap.\n");
        return;
    }
    print_str("Allocated 512 kilobytes successfully.\n");

    print_str("Checking heap integrity...\n");
    check_heap_integrity();

    print_str("Initializing IDT...\n");

    init_idt();         
    init_interrupts();  
    init_pit(100);      

    
    print_str("Kernel setup complete.\n");
    start_symbol();  

    while (1) {
        __asm__ volatile ("hlt"); 
    }
}
