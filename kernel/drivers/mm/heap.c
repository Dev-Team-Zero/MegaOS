#include "heap.h"

static uintptr_t heap_base = HEAP_START;
static uintptr_t heap_end = HEAP_END;

void init_heap(){
    init_pages();
    for(size_t i = 0;i < MAX_HEAP_PAGES;i++) {
        alloc_page();
    }
}