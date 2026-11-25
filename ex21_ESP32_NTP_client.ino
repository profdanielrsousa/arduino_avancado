/* VIGÉSIMO PRIMEIRO EXEMPLO (ESP32 E NTP CLIENT)
   DANIEL RODRIGUES DE SOUSA 28/04/2024
   PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481679406769105921 */

#include <NTPClient.h>          // Inclui a biblioteca para comunicação com servidores NTP (Network Time Protocol)
#include <WiFi.h>               // Inclui a biblioteca para gerenciamento da conexão Wi-Fi no ESP32
#include <WiFiUdp.h>            // Inclui a biblioteca para transporte de pacotes UDP sobre Wi-Fi
#include <LiquidCrystal_I2C.h>  // Inclui a biblioteca para controle do display LCD via interface I2C

#define COLUNAS 20  // Define o número de colunas do display LCD
#define LINHAS  4   // Define o número de linhas do display LCD

#define LED     15  // Define o pino GPIO 15 conectado ao LED indicador

const char *ssid     = "Wokwi-GUEST";  // Define o nome (SSID) da rede Wi-Fi
const char *password = "";             // Define a senha da rede Wi-Fi (vazia para rede aberta)

unsigned long int time_ms;  // Armazena o instante de referência para temporização em milissegundos

bool flag_inicializacao = 0;  // Flag para alternância visual durante a tentativa de conexão Wi-Fi

String atual    = "";  // Armazena a hora formatada atual obtida do servidor NTP
String anterior = "";  // Armazena a última hora formatada exibida para detectar mudança de segundo

WiFiUDP   ntpUDP;                                                 // Instancia o objeto UDP para transporte dos pacotes NTP
NTPClient timeClient(ntpUDP, "a.st1.ntp.br", -10800, 60000);      // Instancia o cliente NTP (servidor NTP.br, fuso UTC-3 = -10800 s, atualização a cada 60 s)

LiquidCrystal_I2C lcd(0x27, COLUNAS, LINHAS);  // Instancia o objeto do display LCD 20x4 no endereço I2C 0x27

void setup()
{
  pinMode(LED, OUTPUT);           // Configura o pino do LED indicador como saída

  Serial.begin(115200);           // Inicializa a comunicação serial com taxa de 115200 bps

  lcd.init();                     // Inicializa a comunicação e os registradores do display LCD
  lcd.backlight();                // Liga a luz de fundo (backlight) do display LCD

  WiFi.begin(ssid, password);     // Inicia a tentativa de conexão com a rede Wi-Fi especificada

  while(WiFi.status() != WL_CONNECTED)          // Aguarda até que a conexão Wi-Fi seja estabelecida
  {
    delay(500);                                 // Aguarda 500 ms entre as verificações de estado da rede
    lcd.setCursor(0, 0);                        // Posiciona o cursor na coluna 0, linha 0 do display LCD
    if(flag_inicializacao)                      // Verifica o estado da flag para alternar o texto no LCD
    {
      lcd.print("Conectando...");               // Exibe a mensagem de conexão em andamento
    }
    else                                        // Caso contrário, apaga a linha para criar efeito de pisca
    {
      lcd.print("             ");               // Limpa os caracteres da mensagem anterior no display LCD
    }

    Serial.print(".");                          // Imprime um ponto no monitor serial indicando progresso
    flag_inicializacao = !flag_inicializacao;   // Inverte o estado lógico da flag de sinalização
  }

  Serial.println("\nConectado");  // Exibe confirmação de conexão Wi-Fi no monitor serial
  Serial.print("IP: ");           // Exibe o rótulo do endereço IP no monitor serial
  Serial.println(WiFi.localIP()); // Exibe o endereço IP local atribuído ao ESP32

  lcd.clear();                    // Limpa todos os caracteres do display LCD
  lcd.setCursor(0, 0);            // Posiciona o cursor na coluna 0, linha 0
  lcd.print("Conectado!");        // Exibe a confirmação de conexão no display LCD
  lcd.setCursor(0, 1);            // Posiciona o cursor na coluna 0, linha 1
  lcd.print("IP: ");              // Exibe o rótulo do endereço IP no display LCD
  lcd.print(WiFi.localIP());      // Exibe o endereço IP local no display LCD

  timeClient.begin();             // Inicializa o cliente NTP para sincronização de horário
}

void loop()
{
  if(millis() - time_ms >= 1000)                // Verifica se transcorreu o intervalo de 1000 ms (1 segundo)
  {
    time_ms = millis();                         // Recarrega o temporizador de referência
    timeClient.update();                        // Atualiza o horário interno consultando o servidor NTP
    atual = timeClient.getFormattedTime();      // Obtém a string de horário formatada (HH:MM:SS)
    digitalWrite(LED, !digitalRead(LED));       // Alterna o estado lógico do LED indicador a cada segundo
  }

  if(atual != anterior)                         // Verifica se houve alteração no horário formatado
  {
    anterior = atual;                           // Atualiza o registro do último horário exibido
    Serial.println(atual);                      // Exibe o horário atualizado no monitor serial
    lcd.setCursor(0, 3);                        // Posiciona o cursor na coluna 0, linha 3 do display LCD
    lcd.print(atual);                           // Exibe o horário atualizado no display LCD
  }
}
