/* SEGUNDO EXEMPLO (BARRA DE LEDS / EFEITO KNIGHT RIDER COM DELAY)
   DANIEL RODRIGUES DE SOUSA 16/09/2019
   PROJETO: https://wokwi.com/projects/481417912078844929 */

int LED0 = 2;  // Define o pino do LED 0
int LED1 = 3;  // Define o pino do LED 1
int LED2 = 4;  // Define o pino do LED 2
int LED3 = 5;  // Define o pino do LED 3
int LED4 = 6;  // Define o pino do LED 4
int LED5 = 7;  // Define o pino do LED 5
int LED6 = 8;  // Define o pino do LED 6
int LED7 = 9;  // Define o pino do LED 7

void LEDs( char led );     // Prototipagem das funções auxiliares do programa

void setup()
{
  pinMode(LED0, OUTPUT);  // Define a direção dos pinos dos LEDs como saída
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
}

void loop()
{//      76543210
  LEDs(0b00000001);  // Carrega a palavra binária acionando o bit 0
  delay(100);        // Aguarda 100 ms
  LEDs(0b00000110);  // Carrega a palavra binária acionando os bits 1 e 2
  delay(100);        // Aguarda 100 ms
  LEDs(0b00001100);  // Carrega a palavra binária acionando os bits 2 e 3
  delay(100);        // Aguarda 100 ms
  LEDs(0b00011000);  // Carrega a palavra binária acionando os bits 3 e 4
  delay(100);        // Aguarda 100 ms
  LEDs(0b00110000);  // Carrega a palavra binária acionando os bits 4 e 5
  delay(100);        // Aguarda 100 ms
  LEDs(0b01100000);  // Carrega a palavra binária acionando os bits 5 e 6
  delay(100);        // Aguarda 100 ms
  LEDs(0b11000000);  // Carrega a palavra binária acionando os bits 6 e 7
  delay(100);        // Aguarda 100 ms
  LEDs(0b10000000);  // Carrega a palavra binária acionando o bit 7
  delay(100);        // Aguarda 100 ms
  LEDs(0b01100000);  // Carrega a palavra binária acionando os bits 6 e 5
  delay(100);        // Aguarda 100 ms
  LEDs(0b00110000);  // Carrega a palavra binária acionando os bits 5 e 4
  delay(100);        // Aguarda 100 ms
  LEDs(0b00011000);  // Carrega a palavra binária acionando os bits 4 e 3
  delay(100);        // Aguarda 100 ms
  LEDs(0b00001100);  // Carrega a palavra binária acionando os bits 3 e 2
  delay(100);        // Aguarda 100 ms
  LEDs(0b00000110);  // Carrega a palavra binária acionando os bits 2 e 1
  delay(100);        // Aguarda 100 ms
  LEDs(0b00000011);  // Carrega a palavra binária acionando os bits 1 e 0
  delay(100);        // Aguarda 100 ms
}

void LEDs( char led )// Função LEDs: recebe um byte contendo o estado de cada LED
{
  digitalWrite(LED0, bitRead(led, 0));  // Atualiza o estado do LED0 conforme o bit 0 de led
  digitalWrite(LED1, bitRead(led, 1));  // Atualiza o estado do LED1 conforme o bit 1 de led
  digitalWrite(LED2, bitRead(led, 2));  // Atualiza o estado do LED2 conforme o bit 2 de led
  digitalWrite(LED3, bitRead(led, 3));  // Atualiza o estado do LED3 conforme o bit 3 de led
  digitalWrite(LED4, bitRead(led, 4));  // Atualiza o estado do LED4 conforme o bit 4 de led
  digitalWrite(LED5, bitRead(led, 5));  // Atualiza o estado do LED5 conforme o bit 5 de led
  digitalWrite(LED6, bitRead(led, 6));  // Atualiza o estado do LED6 conforme o bit 6 de led
  digitalWrite(LED7, bitRead(led, 7));  // Atualiza o estado do LED7 conforme o bit 7 de led
}