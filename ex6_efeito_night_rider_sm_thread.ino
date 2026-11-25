/* SEXTO EXEMPLO (BARRA DE LEDS / EFEITO KNIGHT RIDER COM THREAD)
   DANIEL RODRIGUES DE SOUSA 21/11/2026 
   PROJETO: https://wokwi.com/projects/481670580841064449 */

#include <Thread.h>            // Inclui a biblioteca Thread para criação e agendamento de tarefas
#include <ThreadController.h>  // Inclui a biblioteca ThreadController para gerenciamento múltiplo de threads

#define LED0      2   // Definição do pino do LED 0
#define LED1      3   // Definição do pino do LED 1
#define LED2      4   // Definição do pino do LED 2
#define LED3      5   // Definição do pino do LED 3
#define LED4      6   // Definição do pino do LED 4
#define LED5      7   // Definição do pino do LED 5
#define LED6      8   // Definição do pino do LED 6
#define LED7      9   // Definição do pino do LED 7
#define LED_DEBUG 13  // Definição do pino do LED interno de depuração

ThreadController controle = ThreadController();  // Instancia o controlador responsável por gerenciar todas as threads

Thread* thread_efeito = new Thread();  // Declara a thread responsável pela animação da barra de LEDs
Thread* thread_debug  = new Thread();  // Declara a thread responsável pelo pisca-pisca do LED de depuração

void sm_super_maquina(void);  // Prototipagem das funções auxiliares do programa
void debug_led(void);
void LEDs(char led);

void setup()
{
  Serial.begin(9600);          // Inicializa a comunicação serial com taxa de 9600 bps

  pinMode(LED0, OUTPUT);       // Define a direção dos pinos dos LEDs como saída
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED_DEBUG, OUTPUT);  // Define o pino do LED de depuração como saída

  thread_efeito->onRun(sm_super_maquina);  // Associa a função sm_super_maquina à thread de animação
  thread_efeito->setInterval(100);         // Define o intervalo de execução da thread de animação para 100 ms

  thread_debug->onRun(debug_led);          // Associa a função debug_led à thread de depuração
  thread_debug->setInterval(500);          // Define o intervalo de execução da thread de depuração para 500 ms

  controle.add(thread_efeito);             // Adiciona a thread de animação ao controlador de threads
  controle.add(thread_debug);              // Adiciona a thread de depuração ao controlador de threads
}

void loop()
{
  controle.run();  // Executa o escalonador verificando e disparando as threads no tempo programado

  // ...outras tarefas aqui!
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
    case 15: LEDs(0b00000011); state_led = 0; break;// Aciona os bits 1 e 0 e reinicia a máquina de estados
    default: LEDs(0b00000000); state_led = 0; break;// Apaga todos os LEDs e reinicia em caso de estado inválido
  }
}

void debug_led(void)
{
  digitalWrite(LED_DEBUG, !digitalRead(LED_DEBUG));  // Inverte o estado lógico do LED de depuração
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