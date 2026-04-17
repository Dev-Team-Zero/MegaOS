#include "heap.h"

static uintptr_t heap_base = HEAP_START;
static uintptr_t heap_end = HEAP_END;
static block_header_t* heap_head = NULL;

/**
 * @brief Initializes the kernel heap by setting up the initial block and allocating pages for the heap.
 */
void init_heap(){
    init_pages();
    for(size_t i = 0;i < MAX_HEAP_PAGES;i++) {
        alloc_page();
    }
    heap_head = (block_header_t*)heap_base;
    heap_head->size = HEAP_SIZE - sizeof(block_header_t);
    heap_head->free = 1;
    heap_head->next = NULL;
}

/**
 * @brief Allocates a block of memory of the specified size from the kernel heap.
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block, or NULL if allocation fails.
 */
void* kmalloc(size_t size) {
    if(size == 0) return NULL;
    block_header_t* current = heap_head;
    while(current){
        if(current->free && current->size >= size){
            if(current->size >= size + sizeof(block_header_t) + 1) {
                block_header_t* new_block = (block_header_t*)((uintptr_t) current + sizeof(block_header_t) + size);
                new_block->size = current->size - size - sizeof(block_header_t);
                new_block->free = 1;
                new_block->next = current->next;
                current->size = size;
                current->next = new_block;
            }
            current->free = 0;
            return (void*)((uintptr_t)current + sizeof(block_header_t));
        }
        current = current->next;
    } 
    return NULL;
}

/**
 * @brief Frees a previously allocated block of memory in the kernel heap.
 * @param ptr A pointer to the memory block to free.
 */
void kfree(void* ptr){
    if(!ptr) return;
    block_header_t* header = (block_header_t*)((uintptr_t)ptr - sizeof(block_header_t));
    if(header->free){
        terminal_write_string("Warning: Double free in kfree!\n");
        return;
    }
    header->free = 1;

    while(header->next && header->next->free){
        header->size += sizeof(block_header_t) + header->next->size;
        header->next = header->next->next;
    }

    block_header_t* current = heap_head;
    while(current && (current->next != header)){
        current = current->next;
    }
    if(current && current->free){
        current->size += sizeof(block_header_t) + header->size;
        current->next = header->next;
    }
}