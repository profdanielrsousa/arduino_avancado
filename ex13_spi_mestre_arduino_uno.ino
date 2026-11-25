/* DÉCIMO TERCEIRO EXEMPLO (COMUNICAÇÃO SERIAL SPI - MESTRE COM ARDUINO UNO)
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

unsigned char mestre_envia;   // Armazena o byte a ser transmitido ao escravo via MOSI
unsigned char mestre_recebe;  // Armazena o byte recebido simultaneamente do escravo via MISO

void setup(void)
{
  Serial.begin(115200);                 // Configura a velocidade de transmissão serial em 115200 bps
  
  digitalWrite(SS, HIGH);               // Mantém o pino Slave Select (SS) em nível lógico alto (escravo desabilitado)
  SPI.setClockDivider(SPI_CLOCK_DIV8);  // Configura o divisor de clock do SPI para 2 MHz (16 MHz / 8)

  SPI.begin();                          // Inicializa o barramento SPI em modo mestre
}

void loop(void)
{
  digitalWrite(SS, LOW);                       // Coloca o pino SS em nível lógico baixo para iniciar a comunicação com o escravo
  
  mestre_envia  = 'b';                         // Carrega o caractere 'b' na variável de transmissão
  mestre_recebe = SPI.transfer(mestre_envia);  // Transmite o byte ao escravo e recebe simultaneamente o byte de resposta

  Serial.println((char)mestre_recebe);         // Exibe o caractere recebido do escravo no monitor serial

  digitalWrite(SS, HIGH);                      // Retorna o pino SS para nível lógico alto encerrando a transferência SPI

  delay(1000);                                 // Aguarda 1000 ms antes da próxima transmissão
}
