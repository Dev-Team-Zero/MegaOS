#pragma once
#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096
#define HEAP_START 0x200000
#define HEAP_SIZE  0x100000
#define GUARD_PATTERN 0xDEADBEEF 
#define GUARD_SIZE sizeof(uintptr_t)
#define MAX_MEMORY (1024 * 1024 * 1024) // 1GB
#define RESERVED_PAGES 256 // First 1MB + some reserved
#define MAX_PAGES (MAX_MEMORY / PAGE_SIZE) // 1GB / 4KB = 262144 pages

void init_memory_manager(size_t mem_size);
void* alloc_page();
void free_page(void* addr);
void init_heap();
void* kmalloc(size_t size);
void kfree(void* ptr);
void check_heap_integrity();
void* alloc_pages(size_t count);
size_t get_total_free_pages();