/* QUINTO EXEMPLO (BARRA DE LEDS / EFEITO KNIGHT RIDER COM INTERRUPÇÃO DE TIMER)
   DANIEL RODRIGUES DE SOUSA 16/09/2019 
   PROJETO: https://wokwi.com/projects/481670010009743361 */

#include <TimerOne.h>  // Inclui a biblioteca TimerOne para controle de interrupção por timer

#define TEMPO_TIMER1_US 1000  // Define o período de interrupção do Timer1 em microssegundos (1 ms)

#define LED0      2   // Definição do pino do LED 0
#define LED1      3   // Definição do pino do LED 1
#define LED2      4   // Definição do pino do LED 2
#define LED3      5   // Definição do pino do LED 3
#define LED4      6   // Definição do pino do LED 4
#define LED5      7   // Definição do pino do LED 5
#define LED6      8   // Definição do pino do LED 6
#define LED7      9   // Definição do pino do LED 7
#define LED_DEBUG 13  // Definição do pino do LED interno de depuração

// Variáveis compartilhadas entre a rotina de interrupção (ISR) e o loop()
volatile unsigned char contador_100ms = 0;  // Contador de milissegundos para base de tempo de 100 ms
volatile unsigned int  contador_500ms = 0;  // Contador de milissegundos para base de tempo de 500 ms
volatile bool          flag_100ms     = false;  // Flag de sinalização para execução da tarefa de 100 ms
volatile bool          flag_500ms     = false;  // Flag de sinalização para execução da tarefa de 500 ms

void sm_super_maquina(void);  // Prototipagem das funções auxiliares do programa
void LEDs(char led);
void timer_ISR(void);

void setup()
{
  pinMode(LED0, OUTPUT);       // Define a direção dos pinos dos LEDs como saída
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED_DEBUG, OUTPUT);  // Define o pino do LED de depuração como saída

  digitalWrite(LED_DEBUG, LOW);  // Inicializa o LED de depuração desligado

  Timer1.initialize(TEMPO_TIMER1_US);  // Configura o período do Timer1 para 1000 us (1 ms)
  Timer1.attachInterrupt(timer_ISR);   // Associa a função timer_ISR à interrupção do Timer1
}

void loop()
{
  if(flag_100ms)             // Verifica se transcorreu o intervalo de 100 ms sinalizado pela ISR
  {
    flag_100ms = 0;          // Limpa a flag de sinalização de 100 ms
    sm_super_maquina();      // Executa o próximo passo da máquina de estados dos LEDs
  }

  if(flag_500ms)                                         // Verifica se transcorreu o intervalo de 500 ms sinalizado pela ISR
  {
    flag_500ms = 0;                                      // Limpa a flag de sinalização de 500 ms
    digitalWrite(LED_DEBUG, !digitalRead(LED_DEBUG));    // Inverte o estado lógico do LED de depuração
  }
}

// Rotina de Serviço de Interrupção (ISR) do Timer1: executada a cada 1 ms
// Não executar digitalWrite(), Serial ou a máquina de estados diretamente aqui
void timer_ISR()
{
  contador_100ms++;  // Incrementa o contador da base de tempo de 100 ms
  contador_500ms++;  // Incrementa o contador da base de tempo de 500 ms

  if(contador_100ms >= 100)   // Verifica se atingiu 100 interrupções (100 ms)
  {
    contador_100ms = 0;       // Reinicia o contador de 100 ms
    flag_100ms     = true;    // Sinaliza o transcurso de 100 ms para o loop()
  }

  if(contador_500ms >= 500)   // Verifica se atingiu 500 interrupções (500 ms)
  {
    contador_500ms = 0;       // Reinicia o contador de 500 ms
    flag_500ms     = true;    // Sinaliza o transcurso de 500 ms para alternar o LED de depuração
  }
}

void sm_super_maquina(void)
{
  static unsigned char state_led = 0;  // Armazena o estado atual da máquina de estados

  switch(state_led)  // Seleciona a palavra binária conforme o estado atual
  {
    case 0:  LEDs(0b00000001); state_led++; break;  // Aciona o bit 0 e avança para o próximo estado
    case 1:  LEDs(0b00000011); state_led++; break;  // Aciona os bits 0 e 1 e avança para o próximo estado
    case 2:  LEDs(0b00000110); state_led++; break;  // Aciona os bits 1 e 2 e avança para o próximo estado
    case 3:  LEDs(0b00001100); state_led++; break;  // Aciona os bits 2 e 3 e avança para o próximo estado
    case 4:  LEDs(0b00011000); state_led++; break;  // Aciona os bits 3 e 4 e avança para o próximo estado
    case 5:  LEDs(0b00110000); state_led++; break;  // Aciona os bits 4 e 5 e avança para o próximo estado
    case 6:  LEDs(0b01100000); state_led++; break;  // Aciona os bits 5 e 6 e avança para o próximo estado
    case 7:  LEDs(0b11000000); state_led++; break;  // Aciona os bits 6 e 7 e avança para o próximo estado
    case 8:  LEDs(0b10000000); state_led++; break;  // Aciona o bit 7 e avança para o próximo estado
    case 9:  LEDs(0b11000000); state_led++; break;  // Aciona os bits 7 e 6 e avança para o próximo estado
    case 10: LEDs(0b01100000); state_led++; break;  // Aciona os bits 6 e 5 e avança para o próximo estado
    case 11: LEDs(0b00110000); state_led++; break;  // Aciona os bits 5 e 4 e avança para o próximo estado
    case 12: LEDs(0b00011000); state_led++; break;  // Aciona os bits 4 e 3 e avança para o próximo estado
    case 13: LEDs(0b00001100); state_led++; break;  // Aciona os bits 3 e 2 e avança para o próximo estado
    case 14: LEDs(0b00000110); state_led++; break;  // Aciona os bits 2 e 1 e avança para o próximo estado
    case 15: LEDs(0b00000011); state_led = 0; break;  // Aciona os bits 1 e 0 e reinicia a máquina de estados
    default: LEDs(0b00000000); state_led = 0; break;  // Apaga todos os LEDs e reinicia em caso de estado inválido
  }
}

void LEDs(char led)  // Função LEDs: recebe um byte contendo o estado de cada LED
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