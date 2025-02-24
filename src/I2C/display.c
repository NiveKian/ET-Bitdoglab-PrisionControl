#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

static ssd1306_t ssd; // Inicializa a estrutura do display

/**
 * Função Atualiza posição do selecionador no display
 */
void setDisplay_Selector(uint8_t type, uint16_t select_position)
{
  // Realiza o desenho base
  ssd1306_fill(&ssd, false);                         // Limpa o display
  ssd1306_rect(&ssd, 1, 1, 126, 62, true, false);    // Desenha um retângulo
  ssd1306_draw_string(&ssd, "PRISION CTRL 1", 5, 7); // Desenha uma string
  ssd1306_line(&ssd, 5, 17, 119, 17, true);

  // Realiza o desenho do tipo de MENU
  switch (type)
  {
  case 0:
    ssd1306_draw_string(&ssd, "  PORTA", 5, 21);
    ssd1306_draw_string(&ssd, "  LUZES", 5, 30);
    break;
  case 1:
    ssd1306_draw_string(&ssd, "  PORTA N1  ", 5, 21);
    ssd1306_draw_string(&ssd, "  PORTA N2  ", 5, 30);
    ssd1306_draw_string(&ssd, "  PORTA N3  ", 5, 39);
    ssd1306_draw_string(&ssd, "  PORTA N3  ", 5, 48);
    break;
  case 2:
    ssd1306_draw_string(&ssd, " LUZES N1  ", 5, 21);
    ssd1306_draw_string(&ssd, "  LUZES N2  ", 5, 30);
    ssd1306_draw_string(&ssd, "  LUZES N3  ", 5, 39);
    ssd1306_draw_string(&ssd, "  LUZES N3  ", 5, 48);
    break;
  default:
    break;
  }

  // Realiza desenho da posição do seletor de menu
  switch (select_position)
  {
  case 1:
    ssd1306_draw_string(&ssd, "0", 5, 21);
    break;
  case 2:
    ssd1306_draw_string(&ssd, "0", 5, 30);
    break;
  case 3:
    ssd1306_draw_string(&ssd, "0", 5, 39);
    break;
  case 4:
    ssd1306_draw_string(&ssd, "0", 5, 48);
    break;
  default:
    break;
  }

  ssd1306_send_data(&ssd); // Atualiza o display
}

void setDisplay(char *str, uint8_t x, uint8_t y)
{
  ssd1306_draw_string(&ssd, str, x, y); // Desenha uma string
  ssd1306_send_data(&ssd);              // Atualiza o display
}

void cleanDisplay()
{
  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);
}

void display_init()
{
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                     // Pull up the data line
  gpio_pull_up(I2C_SCL);                     // Pull up the clock line

  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);                                      // Envia os dados para o display

  cleanDisplay();         // Limpa display
  setDisplay_Selector(0,1); // Atualiza o conteúdo do display
}