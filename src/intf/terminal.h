#pragma once

#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 50
#define MAX_CMD_LEN 128
#define MAX_NAMED_POINTERS 32

void update_cursor();
void scroll_screen();
void clear_screen();
void handle_command(const char* cmd);
void process_key(char key);
char scancode_to_ascii(uint8_t scancode);
void start_symbol();

void set_named_pointer(const char* name, void* value);
void* get_named_pointer(const char* name);
void remove_after_space(char* str);