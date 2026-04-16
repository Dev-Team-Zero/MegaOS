#include "vga.h"

size_t terminal_row, terminal_column;
uint8_t terminal_color;
/**
 * @brief A pointer to the VGA memory buffer for terminal output.
 */
volatile uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

/**
 * @brief Creates a VGA entry color value.
 * @param fg The foreground color.
 * @param bg The background color.
 * @return The combined color value.
 */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
    return fg | bg << 4;
}

/**
 * @brief Creates a VGA entry.
 * @param uc The character to display.
 * @param color The color of the character.
 * @return The combined VGA entry.
 */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
	return (uint16_t) uc | (uint16_t) color << 8;
}

/**
 * @brief Clears a specific row in the terminal.
 * @param row The row number to clear.
 */
void terminal_clear_row(size_t row){
	for(size_t col = 0; col < VGA_WIDTH;col++){
		terminal_buffer[row * VGA_WIDTH + col] = vga_entry(' ', terminal_color);;
	}
}

/**
 * @brief Clears the entire terminal.
 */
void terminal_clear(){
	for(size_t row = 0;row < VGA_HEIGHT;row++){
		terminal_clear_row(row);
	}
	terminal_column = 0;
	terminal_row = 0;
}

/**
 * @brief Initializes the terminal by setting the initial cursor position and color, and clearing the screen.
 */
void terminal_initialize(void){
    terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
	
	terminal_clear();
}

/**
 * @brief Sets the color of the terminal text.
 * @param color The new color to set.
 */
void terminal_set_color(uint8_t color){
    terminal_color = color;
}

/**
 * @brief Places a character at a specific position in the terminal.
 * @param c The character to display.
 * @param color The color of the character.
 * @param row The row number.
 * @param col The column number.
 */
void terminal_put_entry_at(char c, uint8_t color, size_t row, size_t col){
    const size_t index = row * VGA_WIDTH + col;
	terminal_buffer[index] = vga_entry(c, color);
}

/**
 * @brief Places a character at the current cursor position and advances the cursor.
 * @param c The character to display.
 */
void terminal_put_char(char c){
    
	if(c == '\n'){
		terminal_row++;
		terminal_column=0;
	} else if(c == '\t'){
		if(terminal_column+(TAB_SIZE+1) >= VGA_WIDTH){
			size_t buff = terminal_column;
			terminal_row++;
			terminal_column=(buff+(TAB_SIZE+1))-VGA_WIDTH;
		} else {
			terminal_column+=TAB_SIZE;
		}
	} else if(c == '\b'){
		if(terminal_column > 0){
			terminal_column--;
			terminal_put_entry_at(' ', terminal_color, terminal_row, terminal_column);
		}
	} else {
		terminal_put_entry_at(c, terminal_color, terminal_row, terminal_column);
		if (terminal_column++ == VGA_WIDTH) {
			terminal_column = 0;
			if (terminal_row++ == VGA_HEIGHT){
				terminal_row = 0;
			}
		}
	}
	
}

/**
 * @brief Writes a string of a specific size to the terminal.
 * @param data The string to write.
 * @param size The number of characters to write.
 */
void terminal_write(const char* data, size_t size){
    for (size_t i = 0; i < size; i++) {
        terminal_put_char(data[i]);
    }
}

/**
 * @brief Writes a null-terminated string to the terminal.
 * @param data The string to write.
 */
void terminal_write_string(const char* data){
    terminal_write(data, strlen(data));
}

/**
 * @brief Moves the cursor to the beginning of the next line.
 */
void terminal_new_line(){
	terminal_column = 0;
}

/**
 * @brief Writes a hexadecimal value to the terminal.
 * @param value The value to write.
 */
void terminal_write_hex(uint64_t value){
	char hex_digits[] = "0123456789ABCDEF";

	terminal_write_string("0x");
	for(int i = 60;i >=0;i-=4){
		uint8_t digit = (value >> i) & 0xF;
		terminal_put_char(hex_digits[digit]);
	}
}