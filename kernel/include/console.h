#pragma once

#include <stddef.h>
#include <stdint.h>
#include "vga.h"
#include "io.h"
#include "string.h"

#define MAX_CONSOLE_LEN 128

void update_cursor();
void vga_scroll();
void console_clear();
void console_process_key(char key);
void console_command_handler(const char* command);