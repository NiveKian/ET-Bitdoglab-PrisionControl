#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "pico/stdlib.h"

#define JOYSTICK_X_PIN 26 // GPIO para eixo X
#define JOYSTICK_Y_PIN 27 // GPIO para eixo Y
#define ADC_CHANNEL_1 1   // adc dp eixo X
#define ADC_CHANNEL_O 0   // adc do eixo y

// Variaveis de controle Joystick
static uint16_t joystick_value[2] = {0,0}

/**
 * Função para os valores do eixos do joystick
 */
void joystick_read_axis()
{
  // Leitura do eixo X do joystick
  adc_select_input(1);
  sleep_us(2);
  joystick_value[1] = adc_read();

  // Leitura do eixo Y do joystick
  adc_select_input(0);
  sleep_us(2);
  joystick_value[0] = adc_read();

  // Estabiliza o valor do meio
  if (joystick_value[0] <= 1890 && joystick_value[0] >= 1880)
  {
    joystick_value[0] = 1886;
  }
  else if (joystick_value[1] <= 2190 && joystick_value[1] >= 2180)
  {
    joystick_value[1] = 2186;
  }
}

/**
 * Função para retornar valores do joystick
 * axis é o valor do potenciometro, sendo 0 para Y, e 1 para X
 */
uint16_t joystick_get_value(uint8_t axis)
{
  joystick_read_axis();
  return joystick_value[axis];
}

/**
 * Função para inicializar o joystick
 */
void joystick_init()
{
  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN);
}