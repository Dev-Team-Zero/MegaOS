#include "console.h"

volatile uint16_t* video_memory = (uint16_t*)VGA_MEMORY;

uint8_t cursor_x = 0;
uint8_t cursor_y = 0;
extern uint8_t terminal_color;
char console_command_buffer[MAX_CONSOLE_LEN];
uint8_t command_length = 0;

void update_cursor(){
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0xF);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0xE);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_scroll(){
    for(uint8_t y = 1;y < VGA_HEIGHT;y++){
        for(uint8_t x = 0;x < VGA_WIDTH;x++){
            video_memory[(y - 1) * VGA_WIDTH + x] = video_memory[y * VGA_WIDTH + x];
        }
    }
    for(uint8_t x = 0;x < VGA_WIDTH;x++){
        video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (terminal_color << 8) | ' ';
    }
    if(cursor_y > 0) cursor_y--;
}

void console_clear(){
    terminal_clear();
    cursor_x = 0;
    cursor_y = 0;
    command_length = 0;
    update_cursor();
}

void console_process_key(char key){
    if(key == '\n'){
        console_command_buffer[command_length] = '\0';
        console_command_handler(console_command_buffer);
        command_length = 0;
        terminal_write_string("> ");
    } else if (key == '\b'){
        if (command_length > 0) {
            command_length--;
        }
    } else {
        if(command_length >= MAX_CONSOLE_LEN) return;
        console_command_buffer[command_length++] = key;
    }
}

void console_command_handler(const char* command){
    if(strcmp(command, "test") == 0){
        terminal_write_string("ok\n");
    } else {
        terminal_write_string("Command ");
        terminal_write_string(command);
        terminal_write_string(" was not found.\n");
    }
}