/* VIGÉSIMO SEGUNDO EXEMPLO (ESP32 E PING)
   DANIEL RODRIGUES DE SOUSA 03/10/2024
   PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481679856380836865 */

#include <WiFi.h>               // Inclui a biblioteca para gerenciamento da conexão Wi-Fi no ESP32
#include <ESPping.h>            // Inclui a biblioteca para envio de pacotes ICMP Echo Request (Ping)
#include <LiquidCrystal_I2C.h>  // Inclui a biblioteca para controle do display LCD via interface I2C

#define COLUNAS 20  // Define o número de colunas do display LCD
#define LINHAS  4   // Define o número de linhas do display LCD

#define LED     15  // Define o pino GPIO 15 conectado ao LED indicador

const char *ssid     = "Wokwi-GUEST";  // Define o nome (SSID) da rede Wi-Fi
const char *password = "";             // Define a senha da rede Wi-Fi (vazia para rede aberta)

bool flag_inicializacao = 0;  // Flag para alternância visual durante a tentativa de conexão Wi-Fi

LiquidCrystal_I2C lcd(0x27, COLUNAS, LINHAS);  // Instancia o objeto do display LCD 20x4 no endereço I2C 0x27

const char *host = "www.google.com";  // Define o nome de domínio (host) de destino para resolução DNS e Ping
IPAddress   ip;                       // Armazena o endereço IPv4 resolvido do servidor de destino

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
  lcd.print("Loc.:");             // Exibe o rótulo do endereço IP local no display LCD
  lcd.print(WiFi.localIP());      // Exibe o endereço IP local no display LCD

  if(WiFi.hostByName(host, ip))                 // Consulta o servidor DNS para resolver o endereço IP do domínio
  {
    Serial.print("IP do site ");                // Exibe o texto introdutório no monitor serial
    Serial.print(host);                         // Exibe o nome do host consultado
    Serial.print(" eh: ");                      // Exibe o conector textual no monitor serial
    Serial.println(ip);                         // Exibe o endereço IPv4 resolvido do host

    lcd.setCursor(0, 2);                        // Posiciona o cursor na coluna 0, linha 2 do display LCD
    lcd.print("Host:");                         // Exibe o rótulo do endereço IP do servidor de destino
    lcd.print(ip);                              // Exibe o endereço IPv4 do host no display LCD
  }
  else                                          // Caso ocorra falha na resolução de nomes (DNS)
  {
    Serial.println("Falha ao resolver o nome do domínio");  // Exibe mensagem de erro de DNS no monitor serial
    while(1);                                   // Mantém o sistema travado em loop infinito em caso de falha
  }     
}

void loop()
{
  if(Ping.ping(host))                           // Envia pacotes ICMP Echo Request e verifica se houve resposta do host
  {
    lcd.setCursor(0, 3);                        // Posiciona o cursor na coluna 0, linha 3 do display LCD
    lcd.print("Ping: ");                        // Exibe o rótulo do tempo de resposta no display LCD
    lcd.print(Ping.averageTime());              // Exibe a latência média medida em milissegundos no display LCD
    lcd.print(" ms");                           // Exibe a unidade de tempo (ms) no display LCD
    Serial.print(Ping.averageTime());           // Exibe a latência média medida no monitor serial
    Serial.println(" ms");                      // Exibe a unidade de tempo e quebra de linha no monitor serial
  }
  else                                          // Caso o servidor de destino não responda ao pacote ICMP
  {
    Serial.println("Sem resposta do host.");    // Exibe mensagem de falha de comunicação no monitor serial
  }
}
