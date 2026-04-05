#include "heap.h"

static uintptr_t heap_base = HEAP_START;
static uintptr_t heap_end = HEAP_END;
static block_header_t* heap_head = NULL;

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