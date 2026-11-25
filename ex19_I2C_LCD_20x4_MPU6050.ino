/* DÉCIMO NONO EXEMPLO (BARRAMENTO I2C - DISPLAY LCD 20X4 E SENSOR IMU MPU-6050)
   DANIEL RODRIGUES DE SOUSA 24/11/2026
   PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481591403608282113 */

/*   
 *    CONFIGURAÇÃO DE PINOS:
 *   
 *    ARDUINO UNO --->  DISPLAY LCD 20X4 I2C / SENSOR MPU-6050
 *        
 *        MESTRE            ESCRAVOS (BARRAMENTO COMPARTILHADO)
 *    PINO (FUNÇÃO)     PINO (FUNÇÃO)
 *    A4 (SDA)    <-->  SDA (LCD 0x27) + SDA (MPU-6050 0x68) OBS: PULL-UP 10K
 *    A5 (SCL)    --->  SCL (LCD 0x27) + SCL (MPU-6050 0x68) OBS: PULL-UP 10K
 *    5V          --->  VCC (LCD E MPU-6050)
 *    GND         --->  GND (LCD E MPU-6050)
 *    
 */

#include <Wire.h>               // Inclui a biblioteca Wire para comunicação serial I2C
#include <LiquidCrystal_I2C.h>  // Inclui a biblioteca para controle do display LCD via interface I2C
#include <Adafruit_MPU6050.h>   // Inclui a biblioteca da Adafruit para o sensor inercial MPU-6050
#include <Adafruit_Sensor.h>    // Inclui a biblioteca base unificada de sensores da Adafruit

LiquidCrystal_I2C tela(0x27, 20, 4);  // Instancia o objeto do display LCD 20x4 no endereço I2C 0x27
Adafruit_MPU6050  sensorMpu;          // Instancia o objeto do sensor acelerômetro e giroscópio MPU-6050

void setup() {
  Serial.begin(115200);                           // Configura a velocidade de transmissão serial em 115200 bps

  tela.init();                                    // Inicializa a comunicação e os registradores do display LCD
  tela.backlight();                               // Liga a luz de fundo (backlight) do display LCD
  tela.setCursor(0, 0);                           // Posiciona o cursor na coluna 0, linha 0
  tela.print("Iniciando sensor...");              // Exibe mensagem inicial de status no display

  if(!sensorMpu.begin())                          // Verifica se o sensor MPU-6050 responde no barramento I2C
  {
    tela.setCursor(0, 1);                         // Posiciona o cursor na coluna 0, linha 1
    tela.print("Erro no MPU6050!");               // Exibe mensagem de falha de comunicação com o sensor
    while(1)                                      // Mantém o programa travado em caso de erro de hardware
    {
      delay(10);                                  // Aguarda 10 ms em cada ciclo de espera
    }
  }

  sensorMpu.setAccelerometerRange(MPU6050_RANGE_8_G);   // Configura a fundo de escala do acelerômetro para +-8 g
  sensorMpu.setFilterBandwidth(MPU6050_BAND_21_HZ);     // Configura a largura de banda do filtro passa-baixa para 21 Hz

  tela.clear();                                   // Limpa todos os caracteres do display LCD
  tela.setCursor(0, 0);                           // Posiciona o cursor na coluna 0, linha 0
  tela.print("Sistema Pronto!");                  // Exibe confirmação de inicialização concluída
  delay(1000);                                    // Aguarda 1000 ms para leitura da mensagem inicial
  tela.clear();                                   // Limpa o display antes de iniciar a exibição contínua
}

void loop() {
  sensors_event_t eventoAceleracao;               // Estrutura que armazena os dados de aceleração linear (m/s^2)
  sensors_event_t eventoGiroscopio;               // Estrutura que armazena os dados de velocidade angular (rad/s)
  sensors_event_t eventoTemperatura;              // Estrutura que armazena a leitura do sensor interno de temperatura

  sensorMpu.getEvent(&eventoAceleracao, &eventoGiroscopio, &eventoTemperatura);  // Realiza a leitura simultânea dos sensores do MPU-6050

  float acelX       = eventoAceleracao.acceleration.x;   // Armazena a aceleração no eixo X em m/s^2
  float acelY       = eventoAceleracao.acceleration.y;   // Armazena a aceleração no eixo Y em m/s^2
  float acelZ       = eventoAceleracao.acceleration.z;   // Armazena a aceleração no eixo Z em m/s^2
  float temperatura = eventoTemperatura.temperature;     // Armazena a temperatura interna do chip em graus Celsius

  tela.setCursor(0, 0);                           // Posiciona o cursor no início da linha 0
  tela.print("Acel X: ");                         // Exibe o rótulo da aceleração no eixo X
  tela.print(acelX, 2);                           // Exibe o valor de aceleração em X com 2 casas decimais
  tela.print(" m/s2   ");                         // Exibe a unidade e espaços extras para limpar resíduos anteriores

  tela.setCursor(0, 1);                           // Posiciona o cursor no início da linha 1
  tela.print("Acel Y: ");                         // Exibe o rótulo da aceleração no eixo Y
  tela.print(acelY, 2);                           // Exibe o valor de aceleração em Y com 2 casas decimais
  tela.print(" m/s2   ");                         // Exibe a unidade e espaços extras para limpar resíduos anteriores

  tela.setCursor(0, 2);                           // Posiciona o cursor no início da linha 2
  tela.print("Acel Z: ");                         // Exibe o rótulo da aceleração no eixo Z
  tela.print(acelZ, 2);                           // Exibe o valor de aceleração em Z com 2 casas decimais
  tela.print(" m/s2   ");                         // Exibe a unidade e espaços extras para limpar resíduos anteriores

  tela.setCursor(0, 3);                           // Posiciona o cursor no início da linha 3
  tela.print("Temp:   ");                         // Exibe o rótulo da temperatura
  tela.print(temperatura, 1);                     // Exibe o valor de temperatura com 1 casa decimal
  tela.print(" C      ");                         // Exibe a unidade e espaços extras para limpar resíduos anteriores

  delay(250);                                     // Aguarda 250 ms antes da próxima atualização das leituras
}
