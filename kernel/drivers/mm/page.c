#include "page.h"

static uint8_t page_bitmap[MAX_PAGES / 8];

static inline void set_bit(size_t bit) {
    page_bitmap[bit / 8] |= (1 << (bit % 8));
}

static inline void clear_bit(size_t bit) {
    page_bitmap[bit / 8] &= (1 << (bit % 8));
}

static inline uint8_t test_bit(size_t bit) {
    return (page_bitmap[bit / 8] >> (bit % 8)) & 1;
}

void init_pages() {
    for(size_t i = 0;i < MAX_PAGES / 8;i++) {
        page_bitmap[i] = 0;
    }

    for(size_t i = 0;i < RESERVED_PAGES;i++) {
        set_bit(i);
    }
}

void* alloc_page(){
    for(size_t i = 0;i < MAX_PAGES;i++) {
        if(!test_bit(i)){
            set_bit(i);
            return (void*)(i*PAGE_SIZE);
        }
    }
    return NULL;
}

void free_page(void* page) {
    size_t index = (size_t)page / PAGE_SIZE;

    if(((size_t)page % 8 ) != 0) {
        terminal_write_string("Warning: free_page called with unaligned page addres!\n");
        return;
    }
    if(index < MAX_PAGES) {
        clear_bit(index);
    }
}