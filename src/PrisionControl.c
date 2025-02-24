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

#define DEBOUNCE_DELAY 1000 // Atraso de 1000ms para debouncing

static volatile uint32_t last_interrupt_time = 0; // Variavel que salva ultima interrupção

// Variaveis de estado do botão
bool isButtonA = false;
bool isButtonB = false;
bool isButtonJOY = false;

// Leds Variables
const npLED_t clr_door_open = {0,10,0};
const npLED_t clr_door_close = {10,0,0};
const npLED_t clr_lights_on = {10,10,10};
const npLED_t clr_lights_off = {0, 0, 0};

const uint8_t id_lights[4] = {1, 3, 21, 23};
const uint8_t id_doors[4] = {8, 6, 18, 16};

bool lights[4] = {true, true, true, true};
bool doors[4] = {false, false, false, false};

npLED_t prisionLeds[25] = {
  {0,0,1}, {1,1,1}, {0,0,1}, {1,1,1}, {0,0,1},  // camada presos Baixo
  {0,0,1}, {1,0,0}, {0,0,1}, {1,0,0}, {0,0,1},  // camada portas Baixo
  {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},  // corredor
  {0,0,1}, {1,0,0}, {0,0,1}, {1,0,0}, {0,0,1},  // camada portas Cima
  {0,0,1}, {1,1,1}, {0,0,1}, {1,1,1}, {0,0,1}}; // camada presos Cima

// Variaveis de controle da PRISÃO
uint8_t menu_state = 0;           // [0,1,2]
int8_t selection_position_M = 1;  // [1,2]
int8_t selection_position_S = 1;  // [1,2,3,4]

const uint8_t menu_SelRngMin = 1;
const uint8_t menu_SelRngMax = 2;
const uint8_t SelRngMin = 1;
const int8_t SelRngMax = 4;

/**
 * Função que realiza função do BOTÃO A
 */
void dobuttonA()
{
  // ACENDE E DESLIGA LUZ DO CORREDOR
  // EMERGENCIA
  isButtonA = false;
}

/**
 * Função que realiza função do BOTÃO B
 * volta para o menu inicial de controle
 */
void dobuttonB()
{
  menu_state = 0;
  isButtonB = false;
  printf("DobuttonB: BACK TO MENU 0\n");
}

/**
 * Função que realiza função do BOTÃO do Joystick
 * Ativa a seleção da funcao da prisão
 * Muda os estados da prisão
 */
void dobuttonJoy()
{
  printf("SYSTEM: dobuttonjoy init\n");
  // Selecao da posição
  int16_t selection_position = (menu_state == 0) ? selection_position_M : selection_position_S;

  // Realiza funcoes de acordo com seleção
  if (menu_state == 0) // ativa o submenu
  {
    printf("  Dobuttonjoy: MENU BASIC\n");
    menu_state = selection_position;
  }
  else if (menu_state == 1) // funcoes das PORTAS
  {
    printf("  Dobuttonjoy: MENU PORTAS\n");
    if (doors[selection_position-1]) // Fecha a porta indicada
    {
      prisionLeds[id_doors[selection_position-1]] = clr_door_close;
      doors[selection_position-1] = false;
      printf("  Dobuttonjoy: DOOR %hd CLOSE\n", selection_position);
    }
    else // Abre a porta indicada
    {
      prisionLeds[id_doors[selection_position-1]] = clr_door_open;
      doors[selection_position-1] = true;
      printf("  Dobuttonjoy: DOOR %hd OPEN\n", selection_position);
    }
  }
  else if (menu_state == 2) // funcoes das LUZES
  {
    printf("  Dobuttonjoy: MENU LUZES\n");
    if (lights[selection_position-1]) // Desliga a luz indicada
    {
      prisionLeds[id_lights[selection_position-1]] = clr_lights_off;
      lights[selection_position-1] = false;
      printf("  Dobuttonjoy: LIGHT %hd OFF\n", selection_position);
    }
    else // Liga a luz indicada
    {
      prisionLeds[id_lights[selection_position-1]] = clr_lights_on;
      lights[selection_position-1] = true;
      printf("  Dobuttonjoy: LIGHT %hd ON\n", selection_position);
    }
  }

  Matriz_change(prisionLeds);
  isButtonJOY = false;
}

/**
 * Função que gere o menu de seleção de acordo com o valor do joystick
 */
void doSelectMenu()
{
  uint16_t joy_val = joystick_get_value(0); // Captura valor do joystick eixo Y
  int8_t *selection_position;              // Ponteiro para posição de seleção
  int16_t min_range, max_range;

  // Define qual variável modificar e seus limites
  if (menu_state == 0)
  {
    selection_position = &selection_position_M;
    min_range = menu_SelRngMin;
    max_range = menu_SelRngMax;
  }
  else
  {
    selection_position = &selection_position_S;
    min_range = SelRngMin;
    max_range = SelRngMax;
  }

  // Atualiza a posição baseado no valor do joystick
  if (joy_val >= 3000)
  {
    (*selection_position)--; // Diminuir seleção
  }
  else if (joy_val <= 1000)
  {
    (*selection_position)++; // Aumentar seleção
  }

  // Garante que a posição esteja dentro dos limites
  if (*selection_position < min_range)
    *selection_position = min_range;
  if (*selection_position > max_range)
    *selection_position = max_range;

  // Atualiza o display com a nova seleção
  setDisplay_Selector(menu_state, *selection_position);
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
      printf("SYSTEM: callback A called\n");
      isButtonA = true;
    }
    if (gpio == BUTTON_B)
    {
      printf("SYSTEM: callback B called\n");
      isButtonB = true;
    }
    if (gpio == BUTTON_JOY)
    {
      printf("SYSTEM: callback JOY called\n");
      isButtonJOY = true;
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
  gpio_init(BUTTON_JOY);
  gpio_set_dir(BUTTON_JOY, GPIO_IN);
  gpio_pull_up(BUTTON_JOY);

  // Inicia Matriz de LEDS com PIO
  initMatriz();
  Matriz_change(prisionLeds);

  // Inicializa o leitor joystick
  joystick_init();

  // Incializa o display I2C
  display_init();

  // Configuração da interrupção com callback
  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BUTTON_JOY, GPIO_IRQ_EDGE_FALL, true);
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
    else if (isButtonJOY) // Detecta mudança de botao JOY e processa
    {
      dobuttonJoy();
    }
    else // Funcao principal do sistema
    {
      doSelectMenu();
    }

    // Reduz processamento
    sleep_ms(100);
  }
}
