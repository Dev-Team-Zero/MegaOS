#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "strings.h"
#include "terminal.h" 
#include "io.h"
#include "print.h"
#include "memory.h"
#include "ata_pio_read.h"

static Page page[32];
int pages = 0;

uint16_t* video_memory = (uint16_t*)0xB8000;
size_t inside_name = 0;
size_t free_pages;

int cursor_x = 0;
int cursor_y = 0;
extern uint8_t color;
char command_buffer[MAX_CMD_LEN];
char* command_buff;
int command_length = 0; 

void update_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void scroll_screen() {
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            video_memory[(y - 1) * VGA_WIDTH + x] = video_memory[y * VGA_WIDTH + x];
        }
    }
    for (int x = 0; x < VGA_WIDTH; x++) {
        video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (color << 8) | ' ';
    }
    if (cursor_y > 0) cursor_y--;
}

void clear_screen() {
    print_clear();     
    cursor_x = 0;
    cursor_y = 0;
    command_length = 0;
    update_cursor();
}

void handle_command(const char* cmd) {
    if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    } else if (strcmp(cmd, "help") == 0) {
        print_str("Available commands:\n");
        print_str("  help         - Show this help message\n");
        print_str("  clear        - Clear the screen\n");
        print_str("  info         - Display OS information\n");
        print_str("  echo <text>  - Print <text> back to the terminal\n");
        print_str("Type 'help <command>' for more details.\n");
    } else if (strcmp(cmd, "help clear") == 0){
        print_str("clear: Clears the screen and resets the prompt.\n");
    } else if (strcmp(cmd, "help info") == 0){
        print_str("info: Displays information about MegaOS.\n");
    } else if (strcmp(cmd, "help echo") == 0){
        print_str("echo <text>: Writes back the message after the command.\n");
        print_str("Example: > echo hello\n");
        print_str("         hello\n");
    } else if (strcmp(cmd, "info") == 0) {
        print_str("MegaOS v0.5.2\n");
        print_str("64-bit, still in progress.\n");
    } else if (strncmp(cmd, "echo", 4) == 0){
        if (cmd[4] == ' ' && cmd[5] != '\0'){
            print_str(cmd + 5);
        }
        print_str("\n");
    } else if (strcmp(cmd, "\n") == 0 || cmd[0] == '\0') {
        
    } else if(strncmp(cmd, "allocpage", 9) == 0){
        aloc_page_name(cmd + 10);
    } else if (strncmp(cmd, "writepage", 9) == 0) {
        
    } else if (strncmp(cmd, "readpage", 8) == 0) {
        
    } else if(strncmp(cmd, "freepages", 9) == 0){
        
    } else {
        print_str("Unknown command: ");
        print_str(cmd);
        print_str("\n");
    }
}

void process_key(char key) {
    if (key == '\n') {
        command_buffer[command_length] = '\0';
        print_char('\n'); 
        handle_command(command_buffer);
        command_length = 0;
        start_symbol();
    } else if (key == '\b') {
        if (command_length > 0) {
            command_length--;
            print_str("\b \b"); 
        }
    } else {
        if (command_length < MAX_CMD_LEN - 1) {
            command_buffer[command_length++] = key;
            print_char(key); 
        }
    }
}

void start_symbol(){
    print_str("> ");
}





void remove_after_space(char* str) {
    char* space = strchr(str, ' ');
    if (space) {
        *space = '\0';
    }
}

//--------------------------TEST----CODE------------------------------------------

void aloc_page_name(const char* name){
    if(pages >= 32){
        print_str("Max pages allocated.\n");
        return;
    }
    page[pages].name = name;
    page[pages].data = alloc_page();
    if(!page[pages].data){
        print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
        print_str("Failed to allocate page.\n");
        print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
        return;
    }
    pages++;
    print_str("Allocated apge: ");
    print_str(name);
    print_str("\n");
}

void write_to_page(const char* name, void* data){
    int pos = 0;
    for(size_t i = 0;i < 32;i++){
        if(page[i].name = name){
            pos = i;
            break;
        }
    }
    page[pos].data = data;
}

void remove_last_page(){
    free_page(page[pages].data);
    page[pages].name = "";
    pages--;
}