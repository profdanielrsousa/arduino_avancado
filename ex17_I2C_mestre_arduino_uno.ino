/* DÉCIMO SÉTIMO EXEMPLO (COMUNICAÇÃO SERIAL I2C - MESTRE COM ARDUINO UNO)
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

#define END_DEVICE  9  // Define o endereço I2C de 7 bits do dispositivo escravo

unsigned char mestre_envia;   // Armazena o byte a ser transmitido ao escravo
unsigned char mestre_recebe;  // Armazena o byte recebido do escravo

void setup(void)
{
  Serial.begin(115200);               // Configura a velocidade de transmissão serial em 115200 bps
  Wire.setClock(400000);              // Configura o clock do barramento I2C para 400 kHz (Fast Mode)
  Wire.begin();                       // Inicializa o barramento I2C em modo mestre
  mestre_envia = mestre_recebe = 0;   // Inicializa as variáveis de transmissão e recepção em zero
}

void loop(void)
{
  Wire.beginTransmission(END_DEVICE); // Inicia a transmissão endereçada ao escravo (END_DEVICE)
  Wire.write(mestre_envia);           // Escreve o byte de dados no buffer de transmissão I2C
  Wire.endTransmission();             // Encerra a transmissão e libera o barramento I2C

  Wire.beginTransmission(END_DEVICE); // Inicia a comunicação com o endereço END_DEVICE
  Wire.requestFrom(END_DEVICE, 1);    // Solicita 1 byte de dados ao dispositivo escravo

  if(Wire.available())                // Verifica se há byte disponível no buffer de recepção I2C
  {
    mestre_recebe = Wire.read();      // Lê o byte recebido do escravo
  }
  
  Wire.endTransmission();             // Finaliza a comunicação no barramento I2C

  Serial.println(mestre_recebe);      // Exibe o valor numérico lido do escravo no monitor serial
  
  mestre_envia++;                     // Incrementa o contador a ser enviado na próxima iteração
  
  delay(1000);                        // Aguarda 1000 ms antes do próximo ciclo de comunicação
}
