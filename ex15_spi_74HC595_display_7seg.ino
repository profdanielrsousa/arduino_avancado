/* DÉCIMO QUINTO EXEMPLO (DISPLAY 7 SEGMENTOS DE 2 DÍGITOS COM 74HC595 VIA SPI POR HARDWARE)
   DANIEL RODRIGUES DE SOUSA 25/11/2026
   PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481424924883731457 */

/*   
 *    CONFIGURAÇÃO DE PINOS (SPI POR HARDWARE):
 *   
 *    ARDUINO UNO --->  MÓDULO 74HC595 (CONECTOR P1)
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    11 (MOSI)   --->  SDI (DS)
 *    13 (SCK)    --->  SCLK (SH_CP)
 *    10 (SS)     --->  LOAD (ST_CP)
 *    
 */

#include <SPI.h>  // Inclui a biblioteca SPI para comunicação serial síncrona por hardware

// Mapa de bits para display de 7 segmentos Ânodo Comum (0 = segmento aceso, 1 = segmento apagado)
// Ordem dos bits: DP(Q7), G(Q6), F(Q5), E(Q4), D(Q3), C(Q2), B(Q1), A(Q0)
const byte digitos[] = {
  //pgfedcba
  0b11000000,  // Algarismo 0
  0b11111001,  // Algarismo 1
  0b10100100,  // Algarismo 2
  0b10110000,  // Algarismo 3
  0b10011001,  // Algarismo 4
  0b10010010,  // Algarismo 5
  0b10000010,  // Algarismo 6
  0b11111000,  // Algarismo 7
  0b10000000,  // Algarismo 8
  0b10010000   // Algarismo 9
};

void exibirNumero(int valor);  // Prototipagem da função auxiliar de exibição no display

void setup(void)
{
  pinMode(SS, OUTPUT);                  // Configura o pino SS (pino 10 no Arduino Uno) como saída
  digitalWrite(SS, HIGH);               // Mantém o sinal de latch inicialmente em nível lógico alto

  SPI.setClockDivider(SPI_CLOCK_DIV2);  // Configura o divisor de clock do SPI para 8 MHz (16 MHz / 2)
  SPI.begin();                          // Inicializa o barramento SPI por hardware (pinos 11 MOSI e 13 SCK)
}

void loop(void)
{
  for(int i = 0; i <= 99; i++)  // Executa a contagem sequencial de 00 a 99
  {
    exibirNumero(i);            // Atualiza os dois dígitos do display via SPI por hardware
    delay(250);                 // Aguarda 250 ms antes do próximo incremento
  }
}

void exibirNumero(int valor)  // Separa as dezenas e unidades e atualiza os dois registradores 74HC595 em cascata
{
  valor = constrain(valor, 0, 99);      // Limita o valor de entrada à faixa válida de 00 a 99

  int dezena  = valor / 10;             // Extrai o algarismo das dezenas
  int unidade = valor % 10;             // Extrai o algarismo das unidades

  SPI.transfer(digitos[unidade]);       // Transmite o 1º byte: entra em U1 e é deslocado para U2 (display das unidades)
  SPI.transfer(digitos[dezena]);        // Transmite o 2º byte: permanece armazenado em U1 (display das dezenas)

  digitalWrite(SS, LOW);                // Gera pulso em nível baixo no pino SS (LOAD / ST_CP) para preparar a trava
  delayMicroseconds(1);                 // Aguarda 1 us para garantir o tempo mínimo de estabilização do sinal
  digitalWrite(SS, HIGH);               // Gera borda de subida no pino SS (LOAD / ST_CP) atualizando ambos os displays simultaneamente
}