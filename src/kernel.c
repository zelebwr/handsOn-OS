#include "shell.h"
#include "kernel.h"

int main() {
  clearScreen(); // clear the screen at the start
  shell(); // start the shell
}

static unsigned char current_text_color = 0x07; // Default text color (light gray on black background)

void setTextColor(unsigned char color) {
  current_text_color = color; // Set the current text color
}

void printString(char *str)
{
  int i = 0; 
  unsigned int AX; 
  char character_to_print; 

  while (str[i] != '\0') {
    character_to_print = str[i];
    AX = 0x0E << 8 | (unsigned char)character_to_print;
    // 0x0E for teletype output, 
    // "<< 8" to shift it to the high byte, 
    // "| (unsigned char)character_to_print" to set the low byte
    interrupt(0x10, AX, 0, 0, 0); // Call BIOS interrupt 0x10 to print character
    i++;
  }
}

void readString(char *buf)
{
  int buffer_index = 0;
  char current_character;
  unsigned int bios_key_input_ax; // stores the result from interrupt 0x16
  unsigned int bios_teletype_ax; // prepares AX for interrupt 0x10
  int max_buffer_size = 127; // maximum size of the buffer for null terminator at buf[127]

  while (true) {
    bios_key_input_ax = interrupt(0x16, (0x00 << 8), 0, 0, 0); // call BIOS interrupt 0x16 to read a key
    current_character = (char)(bios_key_input_ax & 0xFF); // get the character from AX

    if (current_character == '\r' || current_character == '\n') {
      break;
    }
    
    else if (current_character == '\b') { // if character is Backspace
      if (buffer_index > 0) {
        buffer_index--; // move back one character in the buffer
        
        bios_teletype_ax = (0x0E << 8) | '\b'; // prepare AX for interrupt 0x10 to print Backspace
        interrupt(0x10, bios_teletype_ax, 0, 0, 0); // call BIOS interrupt 0x10 to print Backspace

        bios_teletype_ax = (0x0E << 8) | ' '; // prepare AX for interrupt 0x10 to print space
        interrupt(0x10, bios_teletype_ax, 0, 0, 0); // call BIOS interrupt 0x10 to print space (overwite with space)

        bios_teletype_ax = (0x0E << 8) | '\b'; // prepare AX for interrupt 0x10 to print backspace again (moving the cursor back again)
        interrupt(0x10, bios_teletype_ax, 0, 0, 0); // call BIOS interrupt 0x10 to print Backspace again
      } 

    } 
    
    else {
      // for other non-control characters
      if (buffer_index < max_buffer_size) { // check if buffer is not full
        buf[buffer_index] = current_character; // store the character in the buffer
        buffer_index++; // increment the buffer index

        bios_teletype_ax = (0x0E << 8) | current_character; // prepare AX for interrupt 0x10 to print the character
        interrupt(0x10, bios_teletype_ax, 0, 0, 0); // call BIOS interrupt 0x10 to print the character
      }
    }
  }
  // when the loop ends, we need to null-terminate the string
  buf[buffer_index] = '\0'; // set the last character to null terminator
}

void clearScreen()
{
  unsigned int ax, bx, cx, dx; 

  // ah = 0x06 (scroll Up)
  // al = 0x00 (number of lines to scroll, 0 = clear the screen)
  // bh = attribute to fill blank lines
  // ch, cl = (row, column) top-left corner of the window (0,0)
  // dh, dl = (row, column) bottom-right corner of the window (24,79) for 80x25 text mode

  ax = (0x06 << 8); // scroll Up function with no lines to scroll
  bx = (unsigned int)current_text_color << 8; // light gray text on black background
  cx = (0x00 << 8); // top-left corner (row 0, column 0)
  dx = (0x18 << 8) | 0x4F; // cottom-right corner (row 24, column 79)
  interrupt(0x10, ax, bx, cx, dx); // call BIOS interrupt 0x10 to clear the screen

  // set cursor to the top-left corner
  //  ah = 0x02 (Set Cursor Position)
  //  bh = 0x00 (Page number, usually 0)
  //  dh = 0x00 (Row, top row)
  //  dl = 0x00 (Column, left column)

  ax = (0x02 << 8); // set Cursor Position function
  bx = 0x00; // page number 0
  // cx = 0x00; // doesn't mean anything here
  dx = (0x00 << 8) | 0x00; // top-left corner (row 0, column 0)
  interrupt(0x10, ax, bx, 0, dx); // call BIOS interrupt 0x10 to set the cursor position to top left corner
  return;
}