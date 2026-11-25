/* VIGÉSIMO QUINTO EXEMPLO (IoT COM SERVIDOR MQTT, ESP32 E ADC)
   DANIEL RODRIGUES DE SOUSA 17/05/2024
   PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481683117975459841 */   

#include <WiFi.h>          // Inclui a biblioteca para gerenciamento da conexão Wi-Fi no ESP32
#include <PubSubClient.h>  // Inclui a biblioteca PubSubClient para publicação e assinatura MQTT

#define SSID        "Wokwi-GUEST"        // Define o nome (SSID) da rede Wi-Fi
#define PASS        ""                   // Define a senha da rede Wi-Fi (vazia para rede aberta)
#define MQTTSERVER  "test.mosquitto.org" // Define o endereço do broker MQTT público
#define PORT        1883                 // Define a porta TCP padrão do protocolo MQTT

// Lista de servidores (brokers) MQTT públicos gratuitos alternativos:
/*
 * test.mosquitto.org
 * broker.hivemq.com
 * mqtt.eclipseprojects.io
 * broker.emqx.io
 */

char clientId[50];  // Armazena o identificador único (Client ID) gerado para a sessão MQTT
char ad_array[5];   // Armazena a representação textual formatada (4 dígitos) da leitura do ADC

WiFiClient   espClient;          // Instancia o cliente TCP/IP subjacente à conexão Wi-Fi
PubSubClient client(espClient);  // Instancia o cliente MQTT vinculado ao transporte TCP/IP

unsigned long time_ms;  // Armazena o instante de referência para temporização em milissegundos
unsigned int  valor_ad; // Armazena o valor digital bruto convertido pelo canal ADC (0 a 4095)

void mqttReconnect(void);                                              // Prototipagem das funções auxiliares do programa
void callback(char* topic, byte* message, unsigned int length);

void setup()
{
  Serial.begin(115200);                 // Inicializa a comunicação serial com taxa de 115200 bps
  delay(10);                            // Aguarda 10 ms para estabilização da interface serial

  Serial.print("Se conectando a: ");    // Exibe o texto introdutório no monitor serial
  Serial.println(SSID);                 // Exibe o nome da rede Wi-Fi configurada

  WiFi.begin(SSID, PASS);               // Inicia a tentativa de conexão com a rede Wi-Fi especificada
  
  while(WiFi.status() != WL_CONNECTED)  // Aguarda até que a conexão Wi-Fi seja estabelecida
  {
    delay(1000);                        // Aguarda 1000 ms entre as verificações de estado da rede
    Serial.print(".");                  // Imprime um ponto no monitor serial indicando progresso
  }

  WiFi.mode(WIFI_STA);                  // Configura o módulo Wi-Fi do ESP32 em modo estação (Station)

  Serial.println("");                   // Insere uma quebra de linha no monitor serial
  Serial.println("Conexão Wi-Fi realizada");  // Exibe confirmação de conexão Wi-Fi no monitor serial
  Serial.print("Endereço IP: ");        // Exibe o rótulo do endereço IP no monitor serial
  Serial.println(WiFi.localIP());       // Exibe o endereço IP local atribuído ao ESP32

  client.setServer(MQTTSERVER, PORT);   // Configura o endereço do broker MQTT e a porta de conexão
  client.setCallback(callback);         // Registra a função de callback para processamento das mensagens recebidas

  randomSeed(micros());                 // Inicializa a semente do gerador pseudoaleatório usando microssegundos
}

void mqttReconnect(void)
{
  while(!client.connected())            // Executa enquanto o cliente não estiver conectado ao broker MQTT
  {
    Serial.print("Tentando conexão MQTT... ");  // Exibe mensagem de tentativa de conexão no monitor serial
    
    long r = random(1000);                      // Gera um número pseudoaleatório entre 0 e 999
    sprintf(clientId, "clientId-%ld", r);       // Monta a string de identificação única do cliente MQTT

    if(client.connect(clientId))                // Tenta estabelecer a conexão com o broker usando o Client ID gerado
    {
      Serial.print(clientId);                   // Exibe o identificador do cliente conectado no monitor serial
      Serial.println(" conectado");             // Exibe confirmação de conexão estabelecida com o broker
    
      client.subscribe("topicName");            // Realiza a inscrição (subscribe) no tópico "topicName"
    }
    else                                        // Caso a tentativa de conexão com o broker MQTT falhe
    {
      Serial.print("Falha, rc=");               // Exibe o prefixo do código de retorno de erro MQTT
      Serial.print(client.state());             // Exibe o código numérico do estado de erro da biblioteca PubSubClient
      Serial.println(" tentando novamente em 5 segundos");  // Informa o tempo de espera antes da nova tentativa
    
      delay(5000);                              // Aguarda 5000 ms (5 segundos) antes de tentar reconectar
    }
  }
}

void callback(char* topic, byte* message, unsigned int length)
{
  Serial.print("Mensagem recebida no tópico: ");  // Exibe o rótulo do tópico de origem no monitor serial
  Serial.print(topic);                            // Exibe o nome do tópico no qual a mensagem foi publicada
  Serial.print(". Mensagem: ");                   // Exibe o rótulo do conteúdo da carga útil (payload)
  
  String stMessage;                               // Armazena a mensagem recebida convertida em objeto String

  for(int i = 0; i < length; i++)                 // Percorre todos os bytes do payload recebido
  {
    Serial.print((char)message[i]);               // Exibe cada caractere recebido no monitor serial
    stMessage += (char)message[i];                // Concatena cada caractere na variável de texto
  }
}

void loop()
{
  if(!client.connected())                         // Verifica se a conexão com o broker MQTT foi perdida
  {
    mqttReconnect();                              // Executa a rotina de reconexão automática ao broker MQTT
  }

  client.loop();                                  // Processa pacotes MQTT pendentes e mantém o keep-alive ativo

  if(millis() - time_ms >= 500)                   // Verifica se transcorreu o intervalo de 500 ms
  {
    time_ms = millis();                           // Recarrega o temporizador de referência
    valor_ad = analogRead(39);                    // Realiza a conversão analógico-digital no pino GPIO 39 (Sensor VN)
    sprintf(ad_array, "%04d", valor_ad);          // Formata o valor inteiro lido em string de 4 dígitos com zeros à esquerda
    Serial.print("ADC: ");                        // Exibe o rótulo da leitura analógica no monitor serial
    Serial.println(ad_array);                     // Exibe o valor formatado do ADC no monitor serial
    client.publish("esp32/adc", ad_array);        // Publica a leitura formatada no tópico MQTT "esp32/adc"
  }
}
