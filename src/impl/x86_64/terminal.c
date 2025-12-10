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
        if(cmd[9] == ' ' && cmd[10] != '\0'){
            alloc_page_name(cmd+10);
        } else {
            print_str("Usage: allocpage <name>\n");
        }
    } else if (strncmp(cmd, "writepage", 9) == 0) {
        if (cmd[9] == ' ' && cmd[10] != '\0') {
        char* name = (char*)cmd + 10;
        char* text = strchr(name, ' ');
        if (!text) {
            print_str("Usage: writepage <name> <text>\n");
        } else {
            *text = '\0';
            text++;  
            write_to_page(name, text);
            print_str("Wrote to page: ");
            print_str(name);
            print_str("\n");
        }
    } else {
        print_str("Usage: writepage <name> <text>\n");
    }
    } else if (strncmp(cmd, "readpage", 8) == 0) {
        if (cmd[8] == ' ' && cmd[9] != '\0') {
        char* name = (char*)cmd + 9;
        int found = -1;
        for (int i = 0; i < pages; i++) {
            if (strcmp(page[i].name, name) == 0) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            print_str("Page not found.\n");
        } else {
            print_str("Page ");
            print_str(name);
            print_str(": ");
            print_str((char*)page[found].data);
            print_str("\n");
        }
    } else {
        print_str("Usage: readpage <name>\n");
    }
    } else if(strncmp(cmd, "freelastpage", 12) == 0){
        if (pages == 0) {
            print_str("No pages to free.\n");
        } else {
            remove_last_page();
            print_str("Freed last allocated page.\n");
        }
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

void alloc_page_name(const char* name){
    if(pages >= 32){
        print_str("Max pages allocated.\n");
        return;
    }
    static char stored_names[32][64];  
    strcpy(stored_names[pages], name);
    page[pages].name = stored_names[pages];
    page[pages].data = alloc_page();
    if(!page[pages].data){
        print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
        print_str("Failed to allocate page.\n");
        print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
        return;
    }
    print_str("Allocated page: ");
    print_str(stored_names[pages]);
    print_str("\n");
    pages++;
}

void write_to_page(const char* name, const char* data){
    int pos = -1;
    for(size_t i = 0; i < pages; i++){
        if(strcmp(page[i].name, name) == 0){
            pos = i;
            break;
        }
    }
    if (pos == -1){
        print_str("Page not found.\n");
        return;
    }
    char* page_mem = (char*)page[pos].data;
    size_t len = strlen(data);
    if (len > 4095)
        len = 4095;

    memcpy(page_mem, data, len);
    page_mem[len] = '\0'; 
    print_str("Page written.\n");
}

void remove_last_page(){
    if (pages == 0) return;
    free_page(page[pages - 1].data);
    page[pages - 1].name = "";
    pages--;
}