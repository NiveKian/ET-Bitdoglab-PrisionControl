#ifndef DISPLAY_H
#define DISPLAY_H

void display_init();
void setDisplay(char *str, uint8_t x, uint8_t y);
void setDisplay_Selector(uint8_t type, uint16_t select_position);

#endif