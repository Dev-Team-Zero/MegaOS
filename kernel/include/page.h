#pragma once
#include <stdint.h>
#include <stddef.h>
#include "heap.h"
#include "vga.h"

#define PAGE_SIZE 4096
#define RESERVED_PAGES 256
#define MAX_PAGES (MAX_MEMORY / PAGE_SIZE)

void init_pages();
void* alloc_page();
void free_page(void* page);