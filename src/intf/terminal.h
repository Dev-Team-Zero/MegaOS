#pragma once

#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 50
#define MAX_CMD_LEN 128
#define MAX_NAMED_POINTERS 32

typedef struct {
    const char* name;
    void* data;
} Page;


void update_cursor();
void scroll_screen();
void clear_screen();
void handle_command(const char* cmd);
void process_key(char key);
char scancode_to_ascii(uint8_t scancode);
void start_symbol();

void aloc_page_name(const char* name);
void write_to_page(const char* name, void* data);
void remove_after_space(char* str);