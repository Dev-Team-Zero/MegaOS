#include "page.h"

/**
 * @brief A bitmap to track the allocation status of pages in the kernel heap.
 */
static uint8_t page_bitmap[MAX_PAGES / 8];

/**
 * @brief Sets a specific bit in the page bitmap to indicate that a page is allocated.
 * @param bit The index of the bit to set.
 */
static inline void set_bit(size_t bit) {
    page_bitmap[bit / 8] |= (1 << (bit % 8));
}

/**
 * @brief Clears a specific bit in the page bitmap to indicate that a page is free.
 * @param bit The index of the bit to clear.
 */
static inline void clear_bit(size_t bit) {
    page_bitmap[bit / 8] &= (1 << (bit % 8));
}

/**
 * @brief Tests a specific bit in the page bitmap to check if a page is allocated or free.
 * @param bit The index of the bit to test.
 * @return 1 if the bit is set (allocated), 0 if the bit is clear (free).
 */
static inline uint8_t test_bit(size_t bit) {
    return (page_bitmap[bit / 8] >> (bit % 8)) & 1;
}

/**
 * @brief Initializes the page management system by marking the reserved pages as allocated.
 */
void init_pages() {
    for(size_t i = 0;i < MAX_PAGES / 8;i++) {
        page_bitmap[i] = 0;
    }

    for(size_t i = 0;i < RESERVED_PAGES;i++) {
        set_bit(i);
    }
}

/**
 * @brief Allocates a page of memory and returns a pointer to it.
 * @return A pointer to the allocated page, or NULL if no free pages are available.
 */
void* alloc_page(){
    for(size_t i = 0;i < MAX_PAGES;i++) {
        if(!test_bit(i)){
            set_bit(i);
            return (void*)(i*PAGE_SIZE);
        }
    }
    return NULL;
}

/**
 * @brief Frees a previously allocated page of memory.
 * @param page A pointer to the page to free.
 */
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