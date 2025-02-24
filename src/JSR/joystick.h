#ifndef JOYSTICK_H
#define JOYSTICK_H

void joystick_init(); // inicializa o joystick
uint16_t joystick_get_value(uint8_t axis); // retorna valor joystick, de acordo ao Axis provido

#endif