/* DÉCIMO OITAVO EXEMPLO (COMUNICAÇÃO SERIAL I2C - ESCRAVO COM ARDUINO UNO)
   DANIEL RODRIGUES DE SOUSA 23/10/2019 */

/*   
 *    CONFIGURAÇÃO DE PINOS:
 *   
 *    ARDUINO UNO --->  ARDUINO UNO       
 *        
 *        MESTRE            ESCRAVO
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    A4 (SDA)    <-->  A4 (SDA) OBS: PULL-UP 10K
 *    A5 (SCL)    --->  A5 (SCL) OBS: PULL-UP 10K
 *    
 *    ARDUINO UNO --->  ARDUINO MEGA
 *    
 *        MESTRE            ESCRAVO
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    A4 (SDA)    <-->  20 (SDA) OBS: PULL-UP 10K
 *    A5 (SCL)    --->  21 (SCL) OBS: PULL-UP 10K
 *    
 */
 
#include <Wire.h>  // Inclui a biblioteca Wire para comunicação serial I2C

#define END_DEVICE  9  // Define o endereço I2C de 7 bits deste dispositivo escravo

unsigned char escravo_envia;   // Armazena o byte a ser transmitido ao mestre quando solicitado
unsigned char escravo_recebe;  // Armazena o byte recebido do mestre
bool          flag_recepcao;   // Flag indicativa de chegada de novo dado via interrupção I2C

void evento_recepcao( void );     // Prototipagem das funções de callback do barramento I2C
void evento_transmissao( void );

void setup() 
{
  Serial.begin(115200);                // Configura a velocidade de transmissão serial em 115200 bps
  Wire.onReceive(evento_recepcao);     // Registra a função de callback para recepção de dados do mestre
  Wire.onRequest(evento_transmissao);  // Registra a função de callback para solicitação de leitura do mestre
  Wire.begin(END_DEVICE);              // Inicializa o barramento I2C em modo escravo no endereço END_DEVICE
  escravo_envia = escravo_recebe = 0;  // Inicializa as variáveis de transmissão e recepção em zero
  flag_recepcao = 0;                   // Inicializa a flag de recepção desativada
}

void loop() 
{
  if(flag_recepcao == 1)               // Verifica se um novo byte foi recebido pelo barramento I2C
  {
    flag_recepcao = 0;                 // Limpa a flag de sinalização de recepção
    Serial.println(escravo_recebe);    // Exibe o valor numérico recebido do mestre no monitor serial
  }
}

void evento_recepcao( void ) 
{
  while(Wire.available())              // Executa enquanto houver bytes disponíveis no buffer de recepção I2C
  { 
    escravo_recebe = Wire.read();      // Lê o byte enviado pelo mestre
    flag_recepcao  = 1;                // Sinaliza ao loop principal que um novo dado está disponível
  }
}

void evento_transmissao( void ) 
{
  escravo_envia = ~escravo_recebe;     // Calcula o complemento bit a bit (NOT) do último byte recebido
  Wire.write(escravo_envia);           // Transmite o byte resultante ao mestre pelo barramento I2C
}
