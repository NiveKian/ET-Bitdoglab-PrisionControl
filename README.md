# EmbarcaTech: Bitdoglab Prison Control Simulator

Simular um sistema de prisão utilizando a placa de testes BitDogLab para trabalho de conclusão do curso EmbarcaTech.


# Componentes BigDogLab

• Matriz 5x5 de LEDs (endereçáveis) WS2812, conectada à GPIO 7.

• Buzzers A e B, com pinos conectados a GPIO 21 e 10 respectivamente.

• Botão A conectado à GPIO 5.

• Botão B conectado à GPIO 6.

• Display SSD1306 conectado via I2C (GPIO 14 e GPIO15).



# Funcionalidades

1) Através do menu disponibilizado no display é possivel selecionar o submenu de Portas ou Luzes

2) No submenu de Portas pode fazer o controle de portas abrindo ou fechando-as. Seria equivalente ao leds correspondentes a portas saindo do vermelho/verde.
2) No submenu de Luzes pode fazer o controle de luzes dos prisioneiros. Seria equivalente ao leds correspondentes das luzes ligando e desligando.
3) Ao clicar o botão B e possivel voltar so submenu para o menu principal.
4) Ao clicar o botão A, um sinal e emitido, e todas as portas se abrem.
5) Um sinal do buzzer é emitido a cada chamada de funcionalidade e uso do menu do sistema.s


# Diagrama de pastas

```
SRC  
├── PrisionControl.c (arquivo principal a ser rodado) 
└── BUZZ
    ├── buzzer.c (responsável pelo controle do e emissão de som dos buzzer)
    ├── buzzer.h 
└── I2C
    ├── display.c (responsável pelo controle do envio do Display)
    ├── display.h
    ├── font.h (arquivo de armazenamento da tradução 8-Hex das letras e numeros usados)
    ├── ssd1306.c (Inicia e Prove funções de manipulação do Display)
    └── ssd1306.h 
└── JSR
    ├── joystick.c (responsável pela leitura do joystick)  
    ├── joystick.h  
└── matriz
    ├── matrizLed.c (responsável pelo controle da matriz de led 5x5)  
    ├── matrizLed.h  
    └── ledStruct.h (salva o struct utilizado para o RGB) 
``` 

# Modelo wokwi

![wokwi_model](Images/wokwi_model.png)