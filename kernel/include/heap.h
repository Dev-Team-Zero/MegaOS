#pragma once
#include <stdint.h>
#include <stddef.h>
#include "page.h"

#define HEAP_START 0x1000000
#define HEAP_SIZE 0x1000000
#define HEAP_END (HEAP_START + HEAP_SIZE)
#define MAX_HEAP_PAGES (HEAP_SIZE / PAGE_SIZE)

#define MAX_MEMORY 1024 * 1024 * 1024 //1GB

typedef struct block_header {
    size_t size;
    uint8_t free;
    struct block_header* next;
} block_header_t;

void init_heap();