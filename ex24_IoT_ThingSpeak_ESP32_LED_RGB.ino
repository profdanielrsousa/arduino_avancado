/* VIGÉSIMO QUARTO EXEMPLO (IoT COM THINGSPEAK, ESP32 E LED RGB)
   DANIEL RODRIGUES DE SOUSA 03/05/2024
   PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481682063649828865 */

#include <WiFi.h>         // Inclui a biblioteca para gerenciamento da conexão Wi-Fi no ESP32
#include <ThingSpeak.h>   // Inclui a biblioteca oficial para leitura e escrita de dados no ThingSpeak

#define LED_RED   5       // Define o pino GPIO 5 conectado ao canal vermelho do LED RGB
#define LED_GREEN 17      // Define o pino GPIO 17 conectado ao canal verde do LED RGB
#define LED_BLUE  16      // Define o pino GPIO 16 conectado ao canal azul do LED RGB

#define LED_RED_FIELD   1 // Define o Campo 1 (Field 1) do canal para controle do LED vermelho
#define LED_GREEN_FIELD 2 // Define o Campo 2 (Field 2) do canal para controle do LED verde
#define LED_BLUE_FIELD  3 // Define o Campo 3 (Field 3) do canal para controle do LED azul

#define ssid  "Wokwi-GUEST"  // Define o nome (SSID) da rede Wi-Fi
#define senha ""             // Define a senha da rede Wi-Fi (vazia para rede aberta)

// OBS: Altere os parâmetros abaixo conforme as credenciais do seu canal no ThingSpeak
#define myChannelNumber 1234567               // Define o número de identificação (Channel ID) do canal no ThingSpeak
#define myApiKey        "ABCDEFGHIJKLMNOP"    // Define a chave de leitura (Read API Key) do canal no ThingSpeak
#define server          "api.thingspeak.com"  // Define o endereço do servidor da API do ThingSpeak

WiFiClient client;        // Instancia o objeto cliente TCP/IP para comunicação com o servidor ThingSpeak

int led_red_status   = 0; // Armazena o estado lógico lido para o LED vermelho
int led_green_status = 0; // Armazena o estado lógico lido para o LED verde
int led_blue_status  = 0; // Armazena o estado lógico lido para o LED azul

int status;               // Armazena o código de retorno HTTP da última leitura no ThingSpeak

void setup()
{
  pinMode(LED_RED, OUTPUT);             // Configura o pino do LED vermelho como saída
  pinMode(LED_GREEN, OUTPUT);           // Configura o pino do LED verde como saída
  pinMode(LED_BLUE, OUTPUT);            // Configura o pino do LED azul como saída
   
  Serial.begin(115200);                 // Inicializa a comunicação serial com taxa de 115200 bps

  delay(100);                           // Aguarda 100 ms para estabilização do sistema

  Serial.print("Se conectando a: ");    // Exibe o texto introdutório no monitor serial
  Serial.println(ssid);                 // Exibe o nome da rede Wi-Fi configurada

  WiFi.begin(ssid, senha);              // Inicia a tentativa de conexão com a rede Wi-Fi especificada

  while(WiFi.status() != WL_CONNECTED)  // Aguarda até que a conexão Wi-Fi seja estabelecida
  {
    delay(1000);                        // Aguarda 1000 ms entre as verificações de estado da rede
    Serial.print(".");                  // Imprime um ponto no monitor serial indicando progresso
  }

  Serial.println("\nConectado");        // Exibe confirmação de conexão Wi-Fi no monitor serial
  Serial.print("IP: ");                 // Exibe o rótulo do endereço IP no monitor serial
  Serial.println(WiFi.localIP());       // Exibe o endereço IP local atribuído ao ESP32

  ThingSpeak.begin(client);             // Inicializa a comunicação com o servidor ThingSpeak

  digitalWrite(LED_RED, LOW);           // Inicializa o LED vermelho em nível lógico baixo (apagado)
  digitalWrite(LED_GREEN, LOW);         // Inicializa o LED verde em nível lógico baixo (apagado)
  digitalWrite(LED_BLUE, LOW);          // Inicializa o LED azul em nível lógico baixo (apagado)
}

void loop()
{
  led_red_status   = ThingSpeak.readLongField(myChannelNumber, LED_RED_FIELD, myApiKey);    // Lê o valor inteiro do Campo 1 (LED vermelho) no ThingSpeak
  led_green_status = ThingSpeak.readLongField(myChannelNumber, LED_GREEN_FIELD, myApiKey);  // Lê o valor inteiro do Campo 2 (LED verde) no ThingSpeak
  led_blue_status  = ThingSpeak.readLongField(myChannelNumber, LED_BLUE_FIELD, myApiKey);   // Lê o valor inteiro do Campo 3 (LED azul) no ThingSpeak

  if(led_red_status == 1)                         // Verifica se o comando recebido para o LED vermelho é ligar (1)
  {
    digitalWrite(LED_RED, HIGH);                  // Aciona o pino do LED vermelho em nível lógico alto
    Serial.println("LED_RED ligado!");            // Exibe o estado ligado do LED vermelho no monitor serial
  }
  else if(led_red_status == 0)                    // Verifica se o comando recebido para o LED vermelho é desligar (0)
  {
    digitalWrite(LED_RED, LOW);                   // Desaciona o pino do LED vermelho em nível lógico baixo
    Serial.println("LED_RED desligado!");         // Exibe o estado desligado do LED vermelho no monitor serial
  }

  if(led_green_status == 1)                       // Verifica se o comando recebido para o LED verde é ligar (1)
  {
    digitalWrite(LED_GREEN, HIGH);                // Aciona o pino do LED verde em nível lógico alto
    Serial.println("LED_GREEN ligado!");          // Exibe o estado ligado do LED verde no monitor serial
  }
  else if(led_green_status == 0)                  // Verifica se o comando recebido para o LED verde é desligar (0)
  {
    digitalWrite(LED_GREEN, LOW);                 // Desaciona o pino do LED verde em nível lógico baixo
    Serial.println("LED_GREEN desligado!");       // Exibe o estado desligado do LED verde no monitor serial
  }

  if(led_blue_status == 1)                        // Verifica se o comando recebido para o LED azul é ligar (1)
  {
    digitalWrite(LED_BLUE, HIGH);                 // Aciona o pino do LED azul em nível lógico alto
    Serial.println("LED_BLUE ligado!");           // Exibe o estado ligado do LED azul no monitor serial
  }
  else if(led_blue_status == 0)                   // Verifica se o comando recebido para o LED azul é desligar (0)
  {
    digitalWrite(LED_BLUE, LOW);                  // Desaciona o pino do LED azul em nível lógico baixo
    Serial.println("LED_BLUE desligado!");        // Exibe o estado desligado do LED azul no monitor serial
  }

  status = ThingSpeak.getLastReadStatus();        // Obtém o código de status HTTP da última operação de leitura

  if(status == 200)                               // Verifica se o servidor retornou código HTTP 200 (sucesso)
  {
    Serial.println("Dados recebidos com sucesso");// Exibe confirmação de leitura bem-sucedida no monitor serial
  }
  else                                            // Caso ocorra falha ou erro de resposta do servidor
  {
    Serial.println("Erro de envio " + String(status));  // Exibe o código de erro HTTP retornado pelo ThingSpeak
  }

  Serial.println("---");                          // Imprime separador visual entre os ciclos de leitura

  delay(1000);                                    // Aguarda 1000 ms antes da próxima consulta ao ThingSpeak
}
