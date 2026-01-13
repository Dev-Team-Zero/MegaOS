#include "vga.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str){
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void terminal_initialize(void){
    terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t row = 0; row < VGA_HEIGHT; row++) {
		for (size_t col = 0; col < VGA_WIDTH; col++) {
			const size_t index = row * VGA_WIDTH + col;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_set_color(uint8_t color){
    terminal_color = color;
}

void terminal_put_entry_at(char c, uint8_t color, size_t row, size_t col){
    const size_t index = row * VGA_WIDTH + col;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_put_char(char c){
    terminal_put_entry_at(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_write(const char* data, size_t size){
    for (size_t i = 0; i < size; i++) {
        terminal_put_char(data[i]);
    }
}

void terminal_write_string(const char* data){
    terminal_write(data, strlen(data));
}