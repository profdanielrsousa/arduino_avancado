/* VIGÉSIMO EXEMPLO (COMUNICAÇÃO SERIAL I2C - MEMÓRIA EEPROM 24LC256)
   DANIEL RODRIGUES DE SOUSA 23/10/2019 */

/*   
 *    CONFIGURAÇÃO DE PINOS:
 *   
 *    ARDUINO UNO --->  24LC256       
 *        
 *        MESTRE            ESCRAVO
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    A4 (SDA)    <-->  5 (SDA) OBS: PULL-UP 10K
 *    A5 (SCL)    --->  6 (SCL) OBS: PULL-UP 10K
 *    
 *    GND         --->  1 (A0)
 *    GND         --->  2 (A1)
 *    GND         --->  3 (A2)
 *    GND         --->  4 (VSS)
 *    GND         --->  7 (WP)
 *    VCC         --->  8 (VCC)
 *    
 *    ARDUINO MEGA --->  24LC256
 *    
 *        MESTRE            ESCRAVO
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    20 (SDA)    <-->  5 (SDA) OBS: PULL-UP 10K
 *    21 (SCL)    --->  6 (SCL) OBS: PULL-UP 10K
 *    
 *    GND         --->  1 (A0)
 *    GND         --->  2 (A1)
 *    GND         --->  3 (A2)
 *    GND         --->  4 (VSS)
 *    GND         --->  7 (WP)
 *    VCC         --->  8 (VCC)
 */

#include <Wire.h>  // Inclui a biblioteca Wire para comunicação serial I2C

#define END_DEVICE  (0b10100000 >> 1)  // Define o endereço I2C de 7 bits da EEPROM 24LC256 (0x50)

unsigned int  endereco;  // Armazena o endereço de 15 bits da memória EEPROM (0x0000 a 0x7FFF)
unsigned char dado;      // Armazena o byte a ser escrito ou lido da memória EEPROM

void setup(void)
{
  Serial.begin(115200);       // Configura a velocidade de transmissão serial em 115200 bps
  Wire.setClock(400000);      // Configura o clock do barramento I2C para 400 kHz (Fast Mode)
  Wire.begin();               // Inicializa o barramento I2C em modo mestre

  delay(100);                 // Aguarda 100 ms para estabilização da alimentação da EEPROM

  randomSeed(analogRead(0));  // Inicializa a semente do gerador pseudoaleatório com ruído do canal analógico A0

  endereco = 0;               // Inicializa o ponteiro de endereço da memória em 0x0000
}

void loop(void)
{
  dado = random(256);                     // Gera um byte pseudoaleatório entre 0 e 255

  Serial.print("Endereco = 0x");          // Exibe o prefixo hexadecimal do endereço no monitor serial
  Serial.print(endereco, HEX);            // Exibe o endereço atual de gravação/leitura em hexadecimal

  Serial.print(" | Escrito = 0x");        // Exibe o rótulo do dado escrito no monitor serial
  Serial.print(dado, HEX);                // Exibe o valor gerado que será gravado na EEPROM

  /*
   * ESCRITA DE DADOS NA EEPROM (INÍCIO)
   */
  
  Wire.beginTransmission(END_DEVICE);     // Inicia a transmissão endereçada à EEPROM 24LC256 (0x50)
  Wire.write((endereco >> 8) & 0xFF);     // Transmite o byte mais significativo do endereço de memória (MSB)
  Wire.write(endereco & 0xFF);            // Transmite o byte menos significativo do endereço de memória (LSB)
  Wire.write(dado);                       // Transmite o byte de dados a ser gravado no endereço especificado
  Wire.endTransmission();                 // Encerra a transmissão e dispara o ciclo interno de gravação
 
  delay(5);                               // Aguarda 5 ms para conclusão do ciclo interno de gravação (tWC)

  /*
   * ESCRITA DE DADOS NA EEPROM (FIM)
   */

  /*
   * LEITURA DE DADOS NA EEPROM (INÍCIO)
   */

  dado = 0;                               // Zera a variável antes de realizar a leitura de verificação
 
  Wire.beginTransmission(END_DEVICE);     // Inicia a comunicação de escrita fictícia (Dummy Write) para posicionar o ponteiro
  Wire.write((endereco >> 8) & 0xFF);     // Transmite o byte mais significativo do endereço de memória (MSB)
  Wire.write(endereco & 0xFF);            // Transmite o byte menos significativo do endereço de memória (LSB)
  Wire.endTransmission();                 // Encerra a etapa de endereçamento sem enviar condição de parada errônea

  Wire.requestFrom(END_DEVICE, 1);        // Solicita 1 byte de leitura a partir do endereço posicionado na EEPROM

  if(Wire.available())                    // Verifica se o byte solicitado está disponível no buffer de recepção I2C
  {
    dado = Wire.read();                   // Lê o byte recuperado da memória EEPROM
  }
  
  Wire.endTransmission();                 // Finaliza a comunicação no barramento I2C

  /*
   * LEITURA DE DADOS NA EEPROM (FIM)
   */

  Serial.print(" | Lido = 0x");           // Exibe o rótulo do dado lido no monitor serial
  Serial.println(dado, HEX);              // Exibe o valor recuperado da EEPROM em hexadecimal

  endereco = (endereco + 1) & 0x7FFF;     // Incrementa o endereço circularmente até o limite de 32 KB (0x7FFF)
}
