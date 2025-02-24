#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "ledStruct.h"
#include "matriz/matrizLed.h"
#include "I2C/display.h"
#include "JSR/joystick.h"

#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12

#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_JOY 22

#define DEBOUNCE_DELAY 500 // Atraso de 500ms para debouncing

static volatile uint32_t last_interrupt_time = 0; // Variavel que salva ultima interrupção

// Variaveis de estado do botão
bool isButtonA = false;
bool isButtonB = false;

// Leds Variables
const npLED_t clr_door_open = {0, 0, 0};
const npLED_t clr_door_close = {0, 0, 0};
const npLED_t clr_lights = {0, 0, 0};

npLED_t prisionLeds[25] = {
    {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0},
    {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0},
    {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0},
    {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 40, 0},
    {0, 0, 0}, {0, 0, 0}, {255, 40, 0}, {255, 40, 0}, {255, 40, 0}};

/**
 * Função que realiza função do BOTÃO A
 */
void dobuttonA()
{
}

/**
 * Função que realiza função do BOTÃO B
 */
void dobuttonB()
{
}

/**
 * Função que organiza os callbacks do sistema
 */
void gpio_callback(uint gpio, uint32_t events)
{
  // static uint32_t last_interrupt_time = 0;
  uint32_t current_time = to_ms_since_boot(get_absolute_time()); // CONTROLE DE DEBOUNCING NA MAIN ???

  // Verifica se o tempo de debouncing passou
  if (current_time - last_interrupt_time > DEBOUNCE_DELAY)
  {
    if (gpio == BUTTON_A)
    {
      isButtonA = true;
    }
    if (gpio == BUTTON_B)
    {
      isButtonB = true;
    }
    // Atualiza o tempo da última interrupção
    last_interrupt_time = current_time;
  }
}

/**
 * Função que ...
 */
void setup()
{
  // Inicia os LEDS
  gpio_init(LED_RED);
  gpio_set_dir(LED_RED, GPIO_OUT);
  gpio_init(LED_GREEN);
  gpio_set_dir(LED_GREEN, GPIO_OUT);
  gpio_init(LED_BLUE);
  gpio_set_dir(LED_BLUE, GPIO_OUT);

  // Inicia os Botões
  gpio_init(BUTTON_A);
  gpio_set_dir(BUTTON_A, GPIO_IN);
  gpio_pull_up(BUTTON_A);
  gpio_init(BUTTON_B);
  gpio_set_dir(BUTTON_B, GPIO_IN);
  gpio_pull_up(BUTTON_B);

  // Inicia Matriz de LEDS com PIO
  initMatriz();

  // Incializa o display I2C
  display_init();

  // Inicializa o leitor joystick

  // Configuração da interrupção com callback
  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
}

/**
 * Função que ...
 */
int main()
{
  stdio_init_all();
  setup();

  // Programa Inciado!
  printf("Programa Inciado!\n");

  // Loop Principal do programa
  while (true)
  {
    if (isButtonA) // Detecta mudança de botão A e processa
    {
      dobuttonA();
    }
    else if (isButtonB) // Detecta mudança de botão B e processa
    {
      dobuttonB();
    }
    else // Funcao principal do sistema
    {
      // ELSE
    }

    // Reduz processamento
    sleep_ms(100);
  }
}
