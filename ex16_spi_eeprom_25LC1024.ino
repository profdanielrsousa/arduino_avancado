/* DÉCIMO SEXTO EXEMPLO (COMUNICAÇÃO SERIAL SPI - MEMÓRIA EEPROM 25LC1024)
   DANIEL RODRIGUES DE SOUSA 06/10/2019 */

/*   
 *    CONFIGURAÇÃO DE PINOS:
 *   
 *    ARDUINO UNO --->  EEPROM 25LC1024       
 *        
 *        MESTRE           ESCRAVO
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    13 (SCK)    --->  6 (SCK)
 *    12 (MISO)   <---  2 (MISO)
 *    11 (MOSI)   --->  5 (MOSI)
 *    10 (SS)     --->  1 (SS)
 *    
 *    VCC         --->  3 (WP)
 *    VCC         --->  7 (HOLD)
 *    VCC         --->  8 (VCC)
 *    GND         --->  4 (VSS)
 *    
 */
 
#include <SPI.h>  // Inclui a biblioteca SPI para comunicação serial síncrona

unsigned long int endereco;  // Armazena o endereço de 24 bits da memória EEPROM (0x000000 a 0x01FFFF)
unsigned char     dado;      // Armazena o byte a ser escrito ou lido da memória EEPROM

void setup(void)
{
  Serial.begin(115200);                 // Configura a velocidade de transmissão serial em 115200 bps

  digitalWrite(SS, HIGH);               // Mantém o pino Slave Select (SS) em nível lógico alto (EEPROM desabilitada)
  
  SPI.setBitOrder(MSBFIRST);            // Configura a ordem de transmissão dos bits com o mais significativo primeiro
  SPI.setDataMode(SPI_MODE0);           // Configura o barramento SPI no Modo 0 (CPOL = 0, CPHA = 0)
  SPI.setClockDivider(SPI_CLOCK_DIV2);  // Configura o divisor de clock do SPI para 8 MHz (16 MHz / 2)

  SPI.begin();                          // Inicializa o barramento SPI em modo mestre
 
  delay(100);                           // Aguarda 100 ms para estabilização da alimentação da EEPROM

  randomSeed(analogRead(0));            // Inicializa a semente do gerador pseudoaleatório com ruído do canal analógico A0

  endereco = 0;                         // Inicializa o ponteiro de endereço da memória em 0x000000
}

void loop(void)
{
  dado = random(256);                   // Gera um byte pseudoaleatório entre 0 e 255

  Serial.print("Endereco = 0x");        // Exibe o prefixo hexadecimal do endereço no monitor serial
  Serial.print(endereco, HEX);          // Exibe o endereço atual de gravação/leitura em hexadecimal

  Serial.print(" | Escrito = 0x");      // Exibe o rótulo do dado escrito no monitor serial
  Serial.print(dado, HEX);              // Exibe o valor gerado que será gravado na EEPROM

  /*
   * ESCRITA DE DADOS NA EEPROM (INÍCIO)
   */

  digitalWrite(SS, LOW);                  // Coloca o pino SS em nível lógico baixo para selecionar a EEPROM
  SPI.transfer(0b00000110);               // Envia a instrução WREN (0x06) para habilitar o latch de escrita
  digitalWrite(SS, HIGH);                 // Retorna o pino SS para nível lógico alto para efetivar o comando WREN
         
  digitalWrite(SS, LOW);                  // Coloca o pino SS em nível lógico baixo para iniciar a gravação

  SPI.transfer(0b00000010);               // Envia a instrução WRITE (0x02) para escrita de byte na memória
  SPI.transfer((endereco >> 16) & 0xFF);  // Transmite o byte mais significativo do endereço de 24 bits (bits 23..16)
  SPI.transfer((endereco >> 8) & 0xFF);   // Transmite o byte intermediário do endereço de 24 bits (bits 15..8)
  SPI.transfer(endereco & 0xFF);          // Transmite o byte menos significativo do endereço de 24 bits (bits 7..0)

  SPI.transfer(dado);                     // Transmite o byte de dados a ser gravado no endereço especificado

  digitalWrite(SS, HIGH);                 // Retorna o pino SS para nível lógico alto encerrando o ciclo de escrita
  
  delay(4);                               // Aguarda 4 ms para conclusão do ciclo interno de gravação (tWC)

  /*
   * ESCRITA DE DADOS NA EEPROM (FIM)
   */

  /*
   * LEITURA DE DADOS NA EEPROM (INÍCIO)
   */
  
  digitalWrite(SS, LOW);                  // Coloca o pino SS em nível lógico baixo para iniciar a leitura

  SPI.transfer(0b00000011);               // Envia a instrução READ (0x03) para leitura de byte da memória
  SPI.transfer((endereco >> 16) & 0xFF);  // Transmite o byte mais significativo do endereço de 24 bits (bits 23..16)
  SPI.transfer((endereco >> 8) & 0xFF);   // Transmite o byte intermediário do endereço de 24 bits (bits 15..8)
  SPI.transfer(endereco & 0xFF);          // Transmite o byte menos significativo do endereço de 24 bits (bits 7..0)
  
  dado = SPI.transfer(0xFF);              // Envia byte dummy (0xFF) para gerar clock e ler o byte armazenado na EEPROM

  digitalWrite(SS, HIGH);                 // Retorna o pino SS para nível lógico alto encerrando o ciclo de leitura

  /*
   * LEITURA DE DADOS NA EEPROM (FIM)
   */

  Serial.print(" | Lido = 0x");         // Exibe o rótulo do dado lido no monitor serial
  Serial.println(dado, HEX);              // Exibe o valor recuperado da EEPROM em hexadecimal

  endereco = (endereco + 1) & 0x0001FFFF; // Incrementa o endereço circularmente até o limite de 128 KB (0x01FFFF)
}
