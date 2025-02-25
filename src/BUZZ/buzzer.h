#ifndef BUZZER_H
#define BUZZER_H

#define BUZZER_A_PIN 21
#define BUZZER_B_PIN 10

void init_buzzer(); // inicializa os buzzers A e B
void play_tone(uint pin, uint frequency, uint duration_ms); // Toca um som no pin identificado
void beep(); // beep no buzzer A


#endif