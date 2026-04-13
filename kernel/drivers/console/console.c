#include "console.h"

volatile uint16_t* video_memory = (uint16_t*)VGA_MEMORY;

size_t cursor_x = 0;
size_t cursor_y = 0;
extern uint8_t terminal_color;
char console_command_buffer[MAX_CONSOLE_LEN];
char buffer[2];
uint8_t command_length = 0;

void update_cursor(){
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0xF);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0xE);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_scroll(){
    for(size_t y = 1;y < VGA_HEIGHT;y++){
        for(uint8_t x = 0;x < VGA_WIDTH;x++){
            video_memory[(y - 1) * VGA_WIDTH + x] = video_memory[y * VGA_WIDTH + x];
        }
    }
    for(size_t x = 0;x < VGA_WIDTH;x++){
        video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (terminal_color << 8) | ' ';
    }
    if(cursor_y > 0) cursor_y--;
}

/**
 * @brief Clears the console/terminal screen and resets cursor.
 */
void console_clear(){
    terminal_clear();
    cursor_x = 0;
    cursor_y = 0;
    command_length = 0;
    update_cursor();
}

void console_process_key(char key){
    if(key == '\n' || key == '\r'){
        console_command_buffer[command_length] = '\0';
        console_command_handler(console_command_buffer);
        command_length = 0;
        print_start_symbol();
    } else if (key == '\b'){
        if (command_length > 0) {
            command_length--;
        }
    } else{
        if(command_length >= MAX_CONSOLE_LEN) return;
        console_command_buffer[command_length++] = key;
    }
}

/**
 * @brief The actual handler for commands. Contains the code to execute all the console/terminal commands.
 * @param command The line which the command and it`s params are.
 */
void console_command_handler(const char* command){
    if(strcmp(command, "test") == 0){
        terminal_write_string("ok\n");
    } else if(strcmp(command, "clear") == 0){
        console_clear();
    } else if(strncmp(command, "color", 4) == 0){
        if(command_length <= 5){
            set_color_info();
            return;
        }
        command+=6;
        uint8_t bg = 0;
        uint8_t fg = 0;
        while(*command != ' '){
            bg = bg * 10 + (*command - '0');
            command++;
        }
        command++;
        while(*command != '\n' && *command != '\0'){
            fg = fg * 10 + (*command - '0');
            command++;
        }
        if((bg > 15) || (fg > 15)){
            set_color_info();
            return;
        }
        terminal_color = fg | bg << 4;
    } else{
        terminal_write_string("Command ");
        terminal_write_string(command);
        terminal_write_string(" was not found.\n");
    }
}

/**
 * @brief Prints conslole/terminal start symbol.
 */
void print_start_symbol(){
    terminal_write_string("> ");
}

void set_color_info(){
    terminal_write_string("Use: color <background> <foreground>.\n");
    terminal_write_string("BLACK = 0\t\t DARK_GREY = 8\nBLUE = 1\t\t  LIGHT_BLUE = 9\nGREEN = 2\t\t LIGHT_GREEN = 10\nCYAN = 3\t\t  LIGHT_CYAN = 11\nRED = 4\t\t   LIGHT_RED = 12\nMAGENTA = 5\t   LIGHT_MAGENTA = 13\nBROWN = 6\t\t LIGHT_BROWN = 14,\nLIGHT_GREY = 7\tWHITE = 15\n");
}