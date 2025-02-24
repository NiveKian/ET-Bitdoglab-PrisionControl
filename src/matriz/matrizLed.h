#ifndef MATRIZLED_H
#define MATRIZLED_H

#include <stdint.h> // to uint work
#include "ledStruct.h"

void initMatriz(); // Init the LED matriz pin
void buffer_Clear(); // Clear the matriz buffer
void Matriz_change(const npLED_t newDraw[]); // Change the number in the LED

#endif