/* DÉCIMO SEGUNDO EXEMPLO (COMUNICAÇÃO SERIAL RS-485 COM ARDUINO UNO - ESCRAVO / NÓ 2)
   DANIEL RODRIGUES DE SOUSA 25/10/2019 */

/*   
 *    CONFIGURAÇÃO DE PINOS:
 *   
 *    ARDUINO UNO --->  MAX485 (OU SIMILARES - VER DATASHEET)       
 *        
 *        ESCRAVO          MAX485
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    3 (SW RX)   <---  1 (RO)
 *    4 (SW TX)   --->  4 (DI)
 *    2 (DIR)     --->  2 (RE) + 3 (DE)
 *    
 *    OUTROS PINOS DO MAX485
 *    
 *    VCC         --->  8
 *    B           <-->  7  (INSERIR RESISTOR DE 120 OHMS
 *    A           <-->  6   ENTRE PINOS 6 E 7)
 *    GND         --->  5
 *    
 */

#include <SoftwareSerial.h>  // Inclui a biblioteca SoftwareSerial para emulação de porta serial via software

#define DIR   2    // Definição do pino de controle de direção (RE/DE) do transceptor MAX485
#define TEMPO 200  // Define o multiplicador de tempo para timeout de recepção (200 x 10 ms = 2000 ms)

SoftwareSerial SoftSerial(3, 4);  // Instancia a porta serial por software nos pinos 3 (RX) e 4 (TX)

String buffer_tmp = "";  // Buffer temporário para armazenar as mensagens recebidas

unsigned long int tempo_atual;  // Armazena o tempo atual do sistema em milissegundos
unsigned long int tempo_10ms;   // Temporizador de referência para base de tempo de 10 ms

unsigned int timeout;  // Contador regressivo para validação do fim de recepção da mensagem
bool flag_timeout;     // Flag indicativa de recepção pendente pela porta serial por software (RS-485)

void setup() {
  pinMode(DIR, OUTPUT);         // Configura o pino de controle de direção do MAX485 como saída
  digitalWrite(DIR, LOW);       // Inicializa o transceptor MAX485 em modo de recepção (LOW)

  SoftSerial.begin(4800);       // Inicializa a comunicação serial por software com taxa de 4800 bps
  SoftSerial.setTimeout(100);   // Configura o tempo limite (timeout) de leitura da SoftSerial em 100 ms

  Serial.begin(9600);           // Inicializa a comunicação serial por hardware (USB) com taxa de 9600 bps
  Serial.setTimeout(100);       // Configura o tempo limite (timeout) de leitura da Serial em 100 ms

  tempo_10ms   = millis();      // Carrega o instante inicial de referência no temporizador de 10 ms
  flag_timeout = 0;             // Inicializa a flag de timeout desativada
}

void loop() {
  if(SoftSerial.available() > 0)          // Verifica se há dados disponíveis na porta serial por software (MAX485)
  {
    buffer_tmp   = SoftSerial.readString();  // Lê a mensagem recebida do barramento RS-485
    timeout      = TEMPO;                    // Carrega o contador de timeout (2000 ms)
    flag_timeout = 1;                        // Sinaliza pendência de resposta pelo barramento RS-485
  }

  tempo_atual = millis();                 // Atualiza o tempo atual do sistema

  if((tempo_atual - tempo_10ms) >= 10)    // Verifica se transcorreu a base de tempo de 10 ms
  {
    tempo_10ms = millis();                // Recarrega o temporizador de 10 ms
    timeout--;                            // Decrementa o contador regressivo de timeout
    if(!timeout)                          // Verifica se o tempo de espera expirou
    {
      if(flag_timeout)                    // Verifica se há mensagem recebida aguardando resposta via RS-485
      {
        flag_timeout = 0;                 // Limpa a flag de sinalização de timeout
        digitalWrite(DIR, HIGH);          // Habilita o transceptor MAX485 em modo de transmissão (HIGH)
        SoftSerial.print("UNO 2 -> ");    // Envia o identificador do nó Uno 2 pelo barramento RS-485
        SoftSerial.print(buffer_tmp);     // Envia o conteúdo da mensagem de resposta pelo barramento RS-485
        SoftSerial.flush();               // Aguarda o esvaziamento completo do buffer de transmissão da SoftSerial
        digitalWrite(DIR, LOW);           // Retorna o transceptor MAX485 para o modo de recepção (LOW)

        Serial.print("UNO 2 -> ");        // Ecoa o identificador do nó Uno 2 no monitor serial local
        Serial.print(buffer_tmp);         // Ecoa o conteúdo da mensagem enviada no monitor serial local
        Serial.flush();                   // Aguarda o esvaziamento completo do buffer da porta serial USB
      }
    }
  }
}