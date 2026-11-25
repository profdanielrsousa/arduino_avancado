/* VIGÉSIMO TERCEIRO EXEMPLO (IoT COM THINGSPEAK, ESP32 E SENSOR DHT22)
   DANIEL RODRIGUES DE SOUSA 03/05/2024
   PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481680148000447489 */
   
#include <DHT.h>         // Inclui a biblioteca do sensor DHT22 para leitura de temperatura e umidade
#include <WiFi.h>        // Inclui a biblioteca para gerenciamento da conexão Wi-Fi no ESP32
#include <ThingSpeak.h>  // Inclui a biblioteca oficial para envio de dados à plataforma ThingSpeak

#define DHT_PIN 13       // Define o pino GPIO 13 conectado ao pino de dados do sensor DHT22

DHT dht(DHT_PIN, DHT22); // Instancia o objeto de controle do sensor DHT22

#define ssid  "Wokwi-GUEST"  // Define o nome (SSID) da rede Wi-Fi
#define senha ""             // Define a senha da rede Wi-Fi (vazia para rede aberta)

// OBS: Altere os parâmetros abaixo conforme as credenciais do seu canal no ThingSpeak
#define myChannelNumber 1234567               // Define o número de identificação (Channel ID) do canal no ThingSpeak
#define myApiKey        "ABCDEFGHIJKLMNOP"    // Define a chave de escrita (Write API Key) do canal no ThingSpeak
#define server          "api.thingspeak.com"  // Define o endereço do servidor da API do ThingSpeak

WiFiClient client;  // Instancia o objeto cliente TCP/IP para comunicação com o servidor ThingSpeak

int status;         // Armazena o código de retorno HTTP da requisição de escrita no ThingSpeak

void setup()
{
  dht.begin();                          // Inicializa a comunicação com o sensor DHT22
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

  WiFi.mode(WIFI_STA);                  // Configura o módulo Wi-Fi do ESP32 em modo estação (Station)

  ThingSpeak.begin(client);             // Inicializa a comunicação com o servidor ThingSpeak
}

void loop()
{
  dht.read();                                                    // Realiza a leitura bruta dos dados de temperatura e umidade do sensor DHT22
 
  ThingSpeak.setField(1, dht.readTemperature());                 // Atribui o valor de temperatura (°C) ao Campo 1 (Field 1) do canal
  ThingSpeak.setField(2, dht.readHumidity());                    // Atribui o valor de umidade relativa (%) ao Campo 2 (Field 2) do canal
  
  status = ThingSpeak.writeFields(myChannelNumber, myApiKey);    // Envia simultaneamente todos os campos configurados para o ThingSpeak

  Serial.println("Temperatura: " + String(dht.readTemperature(), 2) + "°C");   // Exibe a temperatura medida com 2 casas decimais
  Serial.println("Umidade relativa: " + String(dht.readHumidity(), 1) + "%");  // Exibe a umidade relativa medida com 1 casa decimal
  
  if(status == 200)                                              // Verifica se o servidor retornou código HTTP 200 (sucesso)
  {
    Serial.println("Dados enviados com sucesso");                // Exibe mensagem de confirmação do envio no monitor serial
  }
  else                                                           // Caso ocorra falha ou rejeição no envio dos dados
  {
    Serial.println("Erro de envio " + String(status));           // Exibe o código de erro HTTP retornado pelo ThingSpeak
  }
 
  Serial.println("---");                                         // Imprime separador visual entre os ciclos de envio

  delay(15000);                                                  // Aguarda 15000 ms (15 s) respeitando a taxa mínima da conta gratuita ThingSpeak
}
