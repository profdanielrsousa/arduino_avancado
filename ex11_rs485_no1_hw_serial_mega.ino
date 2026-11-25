/* DÉCIMO PRIMEIRO EXEMPLO (COMUNICAÇÃO SERIAL RS-485 COM ARDUINO MEGA - SERIAL1)
   DANIEL RODRIGUES DE SOUSA 25/10/2019 */

/*   
 *    CONFIGURAÇÃO DE PINOS:
 *   
 *    ARDUINO MEGA ---> MAX485 (OU SIMILARES - VER DATASHEET)       
 *        
 *        ESCRAVO          MAX485
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    19 (RX1)    <---  1 (RO)
 *    18 (TX1)    --->  4 (DI)
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

#define DIR   2    // Definição do pino de controle de direção (RE/DE) do transceptor MAX485
#define TEMPO 200  // Define o multiplicador de tempo para timeout de recepção (200 x 10 ms = 2000 ms)

String buffer_tmp = "";  // Buffer temporário para armazenar as mensagens recebidas

unsigned long int tempo_atual;  // Armazena o tempo atual do sistema em milissegundos
unsigned long int tempo_10ms;   // Temporizador de referência para base de tempo de 10 ms

unsigned int timeout;  // Contador regressivo para validação do fim de recepção da mensagem
bool flag_timeout;     // Flag indicativa de recepção pendente pela porta Serial1 (RS-485)

void setup() {
  pinMode(DIR, OUTPUT);       // Configura o pino de controle de direção do MAX485 como saída
  digitalWrite(DIR, LOW);     // Inicializa o transceptor MAX485 em modo de recepção (LOW)

  Serial1.begin(4800);        // Inicializa a porta serial por hardware Serial1 (RS-485) com taxa de 4800 bps
  Serial1.setTimeout(100);    // Configura o tempo limite (timeout) de leitura da Serial1 em 100 ms

  Serial.begin(9600);         // Inicializa a porta serial principal (USB) com taxa de 9600 bps
  Serial.setTimeout(100);     // Configura o tempo limite (timeout) de leitura da Serial em 100 ms

  tempo_10ms   = millis();    // Carrega o instante inicial de referência no temporizador de 10 ms
  flag_timeout = 0;           // Inicializa a flag de timeout desativada
}

void loop() {
  if(Serial1.available() > 0)           // Verifica se há dados disponíveis na porta Serial1 (MAX485)
  {
    buffer_tmp   = Serial1.readString();  // Lê a mensagem recebida do barramento RS-485
    timeout      = TEMPO;                 // Carrega o contador de timeout (2000 ms)
    flag_timeout = 1;                     // Sinaliza pendência de resposta pelo barramento RS-485
  }

  tempo_atual = millis();               // Atualiza o tempo atual do sistema

  if((tempo_atual - tempo_10ms) >= 10)  // Verifica se transcorreu a base de tempo de 10 ms
  {
    tempo_10ms = millis();              // Recarrega o temporizador de 10 ms
    timeout--;                          // Decrementa o contador regressivo de timeout
    if(!timeout)                        // Verifica se o tempo de espera expirou
    {
      if(flag_timeout)                  // Verifica se há mensagem recebida aguardando resposta via RS-485
      {
        flag_timeout = 0;               // Limpa a flag de sinalização de timeout
        digitalWrite(DIR, HIGH);        // Habilita o transceptor MAX485 em modo de transmissão (HIGH)
        Serial1.print("MEGA 2 -> ");    // Envia o identificador do nó Mega pelo barramento RS-485
        Serial1.print(buffer_tmp);      // Envia o conteúdo da mensagem de resposta pelo barramento RS-485
        Serial1.flush();                // Aguarda o esvaziamento completo do buffer de transmissão da Serial1
        digitalWrite(DIR, LOW);         // Retorna o transceptor MAX485 para o modo de recepção (LOW)

        Serial.print("MEGA 2 -> ");     // Ecoa o identificador do nó Mega no monitor serial local
        Serial.print(buffer_tmp);       // Ecoa o conteúdo da mensagem enviada no monitor serial local
        Serial.flush();                 // Aguarda o esvaziamento completo do buffer da porta serial USB
      }
    }
  }
}