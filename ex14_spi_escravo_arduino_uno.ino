/* DÉCIMO QUARTO EXEMPLO (COMUNICAÇÃO SERIAL SPI - ESCRAVO COM ARDUINO UNO)
   DANIEL RODRIGUES DE SOUSA 06/10/2019 */

/*   
 *    CONFIGURAÇÃO DE PINOS:
 *   
 *    ARDUINO UNO --->  ARDUINO UNO       
 *        
 *        MESTRE            ESCRAVO
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    13 (SCK)    --->  13 (SCK)
 *    12 (MISO)   <---  12 (MISO)
 *    11 (MOSI)   --->  11 (MOSI)
 *    10 (SS)     --->  10 (SS)
 *    
 *    ARDUINO UNO --->  ARDUINO MEGA
 *    
 *        MESTRE            ESCRAVO
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    13 (SCK)    --->  52 (SCK)
 *    12 (MISO)   <---  50 (MISO)
 *    11 (MOSI)   --->  51 (MOSI)
 *    10 (SS)     --->  53 (SS)    
 *    
 */

#include <SPI.h>  // Inclui a biblioteca SPI para comunicação serial síncrona

boolean       flag_recebido;   // Flag indicativa de recepção de byte via interrupção SPI
unsigned char escravo_recebe;  // Armazena o byte recebido do mestre via MOSI
unsigned char escravo_envia;   // Armazena o byte a ser transmitido ao mestre na próxima transferência

void setup()
{
  Serial.begin(115200);    // Configura a velocidade de transmissão serial em 115200 bps
  
  pinMode(MISO, OUTPUT);   // Configura o pino MISO como saída para transmissão do escravo ao mestre

  SPCR |= _BV(SPE);        // Habilita o periférico SPI em modo escravo no registrador SPCR
  flag_recebido = 0;       // Inicializa a flag de recepção desativada

  SPI.attachInterrupt();   // Habilita a interrupção de conclusão de transferência SPI (SPI_STC_vect)
}

void loop()
{ 
  if( flag_recebido )                    // Verifica se um novo byte foi recebido pela interrupção SPI
  {
    flag_recebido = 0;                   // Limpa a flag de sinalização de recepção

    Serial.println((char)escravo_recebe);  // Exibe o caractere recebido do mestre no monitor serial
       
    escravo_envia = 'd';                 // Carrega o caractere 'd' na variável de resposta do escravo

    SPDR = escravo_envia;                // Escreve o byte no registrador de dados SPI (SPDR) para envio ao mestre
  }
}

/*
 *  INTERRUPÇÃO SPI 
 */

ISR(SPI_STC_vect)         // Rotina de interrupção acionada ao término de uma transferência SPI
{
  escravo_recebe = SPDR;  // Lê o registrador de dados SPI (SPDR) e armazena o byte recebido
  flag_recebido  = 1;     // Sinaliza ao loop principal que um novo dado está disponível
}
