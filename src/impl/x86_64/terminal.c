#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "strings.h"
#include "terminal.h" 
#include "io.h"
#include "print.h"
#include "memory.h"
#include "ata_pio_read.h"

#define MAX_NAMED_POINTERS 32

void set_named_pointer(const char* name, void* value);
void* get_named_pointer(const char* name);
void remove_after_space(char* str);

typedef struct{
    const char* name;
    void* value;
} NamedPointer;

uint16_t* video_memory = (uint16_t*)0xB8000;
size_t inside_name = 0;
size_t free_pages;

static NamedPointer named_pointers[MAX_NAMED_POINTERS];
static int named_pointer_count = 0;

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
        const char* name_start = cmd + 9;
        while(*name_start == ' ') name_start++; 
        char name_buf[64];
        strncpy(name_buf, name_start, sizeof(name_buf)-1);
        name_buf[sizeof(name_buf)-1] = '\0';
        remove_after_space(name_buf);
        void* page = alloc_page();
        set_named_pointer(name_buf, page);
        print_str("Allocated page named: ");
        print_str(name_buf);
        print_str("\n");
        if (!page) {
            print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
            print_str("Failed to allocate page\n");
            return;
        }
    } else if (strncmp(cmd, "writepage", 9) == 0) {
        const char* args = cmd + 9;
        while (*args == ' ') args++;
        char name_buf[64];
        strncpy(name_buf, args, sizeof(name_buf)-1);
        name_buf[sizeof(name_buf)-1] = '\0';
        remove_after_space(name_buf);

        char* data_start = strchr(args, ' ');
        if (data_start) {
            data_start++;
            void* page = get_named_pointer(name_buf);
            if (page) {
                strncpy((char*)page, data_start, 4096); 
                print_str("Wrote to page: ");
                print_str(name_buf);
                print_str("\n");
            } else {
                print_str("Page not found: ");
                print_str(name_buf);
                print_str("\n");
            }
        } else {
            print_str("Usage: writepage <name> <data>\n");
        }
    } else if (strncmp(cmd, "readpage", 8) == 0) {
        const char* name_start = cmd + 8;
        while (*name_start == ' ') name_start++;
        char name_buf[64];
        strncpy(name_buf, name_start, sizeof(name_buf)-1);
        name_buf[sizeof(name_buf)-1] = '\0';
        remove_after_space(name_buf);

        void* page = get_named_pointer(name_buf);
        if (page) {
            print_str("Contents of page ");
            print_str(name_buf);
            print_str(": ");
            print_str((char*)page); 
            print_str("\n");
        } else {
            print_str("Page not found: ");
            print_str(name_buf);
            print_str("\n");
        }
    } else if(strncmp(cmd, "writetolba0", 11) == 0){
        void* page = get_named_pointer(cmd+12);
        if (page) {
            ata_pio_write28(0,1,page);
        } else {
            print_str("Page not found: ");
            print_str("\n");
        }
        
    } else if(strncmp(cmd, "freepages", 9) == 0){
        free_pages = get_total_free_pages();
        print_str("Free pages: ");
        char *buf = kmalloc(sizeof(size_t));
        strcpy(buf, (char*)free_pages);
        print_str(buf);
        print_str("\n");
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

void set_named_pointer(const char* name, void* value) {
    for(int i = 0; i < named_pointer_count; i++) {
        if(strcmp(named_pointers[i].name, name) == 0) {
            named_pointers[i].value = value;
            return;
        }
    }
    if(named_pointer_count < MAX_NAMED_POINTERS) {
        named_pointers[named_pointer_count].name = name;
        named_pointers[named_pointer_count].value = value;
        named_pointer_count++;
    }
}

void* get_named_pointer(const char* name) {
    for(int i = 0; i < named_pointer_count; i++) {
        if(strcmp(named_pointers[i].name, name) == 0) {
            return named_pointers[i].value;
        }
    }
    return NULL;
}

void remove_after_space(char* str) {
    char* space = strchr(str, ' ');
    if (space) {
        *space = '\0';
    }
}