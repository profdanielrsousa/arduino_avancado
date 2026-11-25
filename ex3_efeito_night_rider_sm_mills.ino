/* TERCEIRO EXEMPLO (BARRA DE LEDS / EFEITO KNIGHT RIDER COM MÁQUINA DE ESTADOS E MILLIS)
   DANIEL RODRIGUES DE SOUSA 16/09/2019
   PROJETO: https://wokwi.com/projects/481667674527082497 */

// Declaração de variáveis
unsigned long int tempo_atual;  // Armazena o tempo atual do sistema em milissegundos
unsigned long int tempo_100ms;  // Temporizador para atualização da barra de LEDs (100 ms)
unsigned long int tempo_500ms;  // Temporizador para sinalização do LED de depuração (500 ms)

#define LED0      2   // Definição do pino do LED 0
#define LED1      3   // Definição do pino do LED 1
#define LED2      4   // Definição do pino do LED 2
#define LED3      5   // Definição do pino do LED 3
#define LED4      6   // Definição do pino do LED 4
#define LED5      7   // Definição do pino do LED 5
#define LED6      8   // Definição do pino do LED 6
#define LED7      9   // Definição do pino do LED 7

#define LED_DEBUG 13  // Definição do pino do LED interno de depuração

void sm_super_maquina( void );  // Prototipagem das funções auxiliares do programa
void LEDs( char led );

void setup() {
  pinMode(LED0, OUTPUT);  // Define a direção dos pinos dos LEDs como saída
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);

  pinMode(LED_DEBUG, OUTPUT);  // Define o pino do LED de depuração como saída
  
  // Inicialização da temporização usando o método millis()
  tempo_100ms = tempo_500ms = millis();  // Carrega o tempo inicial nos temporizadores
}

void loop() {
  tempo_atual = millis();  // Atualiza o tempo atual do sistema
  
  if((tempo_atual - tempo_100ms) >= 100)  // Verifica se transcorreu a temporização de 100 ms
  {
    tempo_100ms = millis();  // Recarrega o temporizador de 100 ms
    sm_super_maquina();      // Executa o próximo passo da máquina de estados dos LEDs
  }
  
  if((tempo_atual - tempo_500ms) >= 500)  // Verifica se transcorreu a temporização de 500 ms
  {
    tempo_500ms = millis();                                // Recarrega o temporizador de 500 ms
    digitalWrite(LED_DEBUG, !digitalRead(LED_DEBUG));      // Inverte o estado lógico do LED de depuração
  }
}

void sm_super_maquina( void )
{
  static unsigned char state_led;  // Armazena o estado atual da máquina de estados
  
  switch( state_led )              // Seleciona a palavra binária conforme o estado atual
  {
    case 0:
      LEDs( 0b00000001 );  // Carrega a palavra binária acionando o bit 0
      state_led++;         // Avança para o próximo estado
      break;

    case 1:
      LEDs( 0b00000011 );  // Carrega a palavra binária acionando os bits 0 e 1
      state_led++;         // Avança para o próximo estado
      break;

    case 2:
      LEDs( 0b00000110 );  // Carrega a palavra binária acionando os bits 1 e 2
      state_led++;         // Avança para o próximo estado
      break;

    case 3:
      LEDs( 0b00001100 );  // Carrega a palavra binária acionando os bits 2 e 3
      state_led++;         // Avança para o próximo estado
      break;

    case 4:
      LEDs( 0b00011000 );  // Carrega a palavra binária acionando os bits 3 e 4
      state_led++;         // Avança para o próximo estado
      break;

    case 5:
      LEDs( 0b00110000 );  // Carrega a palavra binária acionando os bits 4 e 5
      state_led++;         // Avança para o próximo estado
      break;

    case 6:
      LEDs( 0b01100000 );  // Carrega a palavra binária acionando os bits 5 e 6
      state_led++;         // Avança para o próximo estado
      break;

    case 7:
      LEDs( 0b11000000 );  // Carrega a palavra binária acionando os bits 6 e 7
      state_led++;         // Avança para o próximo estado
      break;

    case 8:
      LEDs( 0b10000000 );  // Carrega a palavra binária acionando o bit 7
      state_led++;         // Avança para o próximo estado
      break;

    case 9:
      LEDs( 0b11000000 );  // Carrega a palavra binária acionando os bits 7 e 6
      state_led++;         // Avança para o próximo estado
      break;

    case 10:
      LEDs( 0b01100000 );  // Carrega a palavra binária acionando os bits 6 e 5
      state_led++;         // Avança para o próximo estado
      break;

    case 11:
      LEDs( 0b00110000 );  // Carrega a palavra binária acionando os bits 5 e 4
      state_led++;         // Avança para o próximo estado
      break;

    case 12:
      LEDs( 0b00011000 );  // Carrega a palavra binária acionando os bits 4 e 3
      state_led++;         // Avança para o próximo estado
      break;

    case 13:
      LEDs( 0b00001100 );  // Carrega a palavra binária acionando os bits 3 e 2
      state_led++;         // Avança para o próximo estado
      break;

    case 14:
      LEDs( 0b00000110 );  // Carrega a palavra binária acionando os bits 2 e 1
      state_led++;         // Avança para o próximo estado
      break;

    case 15:
      LEDs( 0b00000011 );  // Carrega a palavra binária acionando os bits 1 e 0
      state_led = 0;       // Reinicia a máquina de estados para o estado 0
      break;

    default:
      LEDs( 0b00000000 );  // Apaga todos os LEDs em caso de estado inválido
      state_led = 0;       // Reinicia a máquina de estados para o estado 0
      break;
  }
}

void LEDs( char led )  // Função LEDs: recebe um byte contendo o estado de cada LED
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