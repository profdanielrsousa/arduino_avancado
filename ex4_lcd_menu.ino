/* QUARTO EXEMPLO (LCD KEYPAD SHIELD, TEMPERATURA INTERNA E PWM)
   DANIEL RODRIGUES DE SOUSA 22/09/2026
   PROJETO: https://www.tinkercad.com/things/gtROW1fddbJ-quarto-exemplo-lcd-keypad-shield-temperatura-interna-e-pwm */

#include <LiquidCrystal.h>  // Inclui a biblioteca LiquidCrystal para controle do display LCD

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // Define os pinos de interface do LCD Keypad Shield (RS, E, D4, D5, D6, D7)

#define TEMPO_TIMEOUT 10  // Define o tempo limite (em segundos) para retorno automático à tela principal

// Definição das constantes dos botões do LCD Keypad Shield
enum Botao {
  BTN_NENHUM = 0,  // Nenhum botão pressionado
  BTN_RIGHT,       // Botão Direito
  BTN_UP,          // Botão Cima
  BTN_DOWN,        // Botão Baixo
  BTN_LEFT,        // Botão Esquerda
  BTN_SELECT       // Botão Seleção
};

// Definição dos estados da máquina de estados (telas do menu)
enum EstadoMenu {
  TELA_APRESENTACAO = 0,  // Tela inicial de apresentação
  TELA_PRINCIPAL,         // Tela principal com resumo dos parâmetros
  TELA_TEMPERATURA,       // Tela de exibição da temperatura interna da CPU
  TELA_MONITOR,           // Tela de configuração do monitor serial
  TELA_PWM,               // Tela de ajuste do ciclo de trabalho (duty cycle) PWM
  TELA_LED                // Tela de configuração do pisca-pisca do LED interno
};

int estadoAtual = TELA_APRESENTACAO;// Armazena o estado atual do menu
int ultimoBotao = BTN_NENHUM;       // Armazena o último estado lido do teclado para detecção de borda
int cmd         = BTN_NENHUM;       // Armazena o evento de botão validado no ciclo atual

int tempAtual = 25;                 // Armazena a temperatura atual lida do sensor interno da CPU
int timeout   = 0;                  // Contador regressivo para retorno automático à tela principal
unsigned long tempoAnterior = 0;    // Armazena o tempo anterior em milissegundos para base de tempo de 1 s
bool flag_led     = false;          // Habilita ou desabilita a sinalização intermitente no LED interno
bool flag_monitor = false;          // Habilita ou desabilita o envio de telemetria pela porta serial
int duty_cycle    = 0;              // Armazena o percentual do ciclo de trabalho PWM (0 a 100%)
int duty_pwm      = 0;              // Armazena o valor convertido do PWM em 8 bits (0 a 255)

String buffer_lcd;  // Buffer auxiliar para montagem de strings exibidas no LCD e na Serial

// Mapa de bits do caractere customizado de grau (°) para o LCD
byte grau[8] = {
  0B00000110,
  0B00001001,
  0B00001001,
  0B00000110,
  0B00000000,
  0B00000000,
  0B00000000,
  0B00000000
};

int lerTeclado(void);             // Prototipagem das funções auxiliares do sistema
int detectarEventoBotao(void);
int lerTempInterna(void);
void temporizacao_sistema(void);
void trata_telas(void);
void trata_botoes(void);

void setup() {
  Serial.begin(9600);               // Inicializa a comunicação serial com taxa de 9600 bps
  pinMode(LED_BUILTIN, OUTPUT);     // Configura o pino 13 (LED interno) como saída digital
  lcd.begin(16, 2);                 // Inicializa o display LCD com 16 colunas e 2 linhas
  lcd.createChar(0, grau);          // Registra o caractere customizado de grau na posição 0 da CGRAM
  lcd.clear();                      // Limpa o conteúdo do display LCD
  estadoAtual   = TELA_APRESENTACAO;// Define a tela de apresentação como estado inicial
  timeout       = TEMPO_TIMEOUT;    // Inicializa o temporizador de inatividade
  tempoAnterior = millis();         // Registra o instante inicial de referência para temporização
}

void loop() {
  cmd = detectarEventoBotao();  // Verifica e captura eventos de pressionamento de botão
  temporizacao_sistema();       // Executa as tarefas periódicas temporizadas a cada 1 segundo
  trata_telas();                // Atualiza a exibição no display LCD conforme o estado atual
  trata_botoes();               // Processa a navegação e os ajustes conforme o botão pressionado
}

// Leitura do sensor de temperatura interno do ATmega328P (canal ADC8 com Vref = 1,1 V)
// Referência: https://youtu.be/wEHCfDzW4ro
int lerTempInterna(void) {
  ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX3);  // Configura Vref interna de 1,1 V e seleciona o canal ADC8
  delay(10);                                          // Aguarda 10 ms para estabilização da referência interna

  ADCSRA |= (1 << ADSC);           // Inicia a primeira conversão ADC descartável (aquecimento)
  while (ADCSRA & (1 << ADSC));    // Aguarda o término da primeira conversão

  ADCSRA |= (1 << ADSC);           // Inicia a conversão ADC válida
  while (ADCSRA & (1 << ADSC));    // Aguarda o término da conversão válida
  int leituraADC = ADCW;           // Lê o resultado de 10 bits dos registradores ADCL e ADCH

  analogReference(DEFAULT);        // Restaura a tensão de referência padrão AVCC (5 V) para o analogRead(A0)
  delay(5);                        // Aguarda 5 ms para estabilização da referência
  analogRead(A0);                  // Realiza leitura descartável para estabilizar o multiplexador no pino A0

  if (leituraADC == 0) return 25;              // Retorna 25 °C padrão caso o simulador retorne 0 no canal ADC8
  return (int)((leituraADC - 324.31) / 1.22);  // Converte o valor ADC em graus Celsius conforme datasheet do ATmega328P
}

void temporizacao_sistema(void)
{
  if(millis() - tempoAnterior >= 1000)   // Verifica se transcorreu o intervalo de 1000 ms (1 segundo)
  {
    tempoAnterior = millis();            // Atualiza o marcador de tempo anterior
    tempAtual     = lerTempInterna();    // Atualiza a leitura de temperatura interna da CPU
    
    timeout--;                           // Decrementa o contador de inatividade das telas secundárias
    if(!timeout)                         // Verifica se o tempo limite de inatividade expirou
    {
      lcd.clear();                       // Limpa o display LCD antes de trocar de tela
      estadoAtual = TELA_PRINCIPAL;      // Retorna automaticamente para a tela principal
    }

    if(flag_led)                         // Verifica se o modo intermitente do LED está habilitado
    {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // Inverte o estado lógico do LED interno
    }
    else                                 // Caso o modo intermitente do LED esteja desabilitado
    {
      digitalWrite(LED_BUILTIN, LOW);    // Mantém o LED interno desligado
    }

    if(flag_monitor)                     // Verifica se o envio de telemetria serial está habilitado
    {
      buffer_lcd = "Temperatura CPU: " + String(tempAtual) + 
                   "Graus | PWM: " + String(duty_cycle) + "% | LED: " + 
                   String(digitalRead(LED_BUILTIN) ? "LIGADO" : "DESLIGADO");  // Formata a string de telemetria
      Serial.println(buffer_lcd);        // Envia os dados formatados pelo monitor serial
    }
  }
}

int lerTeclado(void) {
  int keyPress = analogRead(A0);         // Lê o valor analógico do divisor resistivo dos botões no pino A0
  if (keyPress < 65)  return BTN_RIGHT;  // Identifica pressionamento do botão RIGHT
  if (keyPress < 221) return BTN_UP;     // Identifica pressionamento do botão UP
  if (keyPress < 395) return BTN_DOWN;   // Identifica pressionamento do botão DOWN
  if (keyPress < 602) return BTN_LEFT;   // Identifica pressionamento do botão LEFT
  if (keyPress < 873) return BTN_SELECT; // Identifica pressionamento do botão SELECT
  return BTN_NENHUM;                     // Retorna nenhum botão pressionado caso a leitura esteja acima do limiar
}

int detectarEventoBotao(void)
{
  int botaoLido = lerTeclado();  // Realiza a leitura atual do teclado analógico
  int evento    = BTN_NENHUM;    // Inicializa o evento como nenhum botão acionado

  if((botaoLido != ultimoBotao) && 
     (botaoLido != BTN_NENHUM))  // Detecta borda de acionamento (transição de estado para tecla pressionada)
  {
    evento = botaoLido;          // Registra o botão pressionado como evento válido
    delay(50);                   // Aguarda 50 ms para tratamento de repique (debounce)
  }
  ultimoBotao = botaoLido;       // Atualiza o estado anterior do botão

  return evento;                 // Retorna o evento de botão detectado
}

void trata_telas(void)
{
  switch(estadoAtual)  // Seleciona a rotina de exibição conforme o estado atual do menu
  {
    case TELA_APRESENTACAO:
      lcd.setCursor(0, 0);               // Posiciona o cursor na coluna 0, linha 0
      lcd.print("CURSO DE ARDUINO");     // Exibe o título na primeira linha do LCD
      lcd.setCursor(0, 1);               // Posiciona o cursor na coluna 0, linha 1
      lcd.print(" FATEC ITAQUERA ");     // Exibe a instituição na segunda linha do LCD
      break;

    case TELA_PRINCIPAL:
      lcd.setCursor(0, 0);               // Posiciona o cursor na coluna 0, linha 0
      buffer_lcd = "CPU:" + String(tempAtual);  // Formata o texto da temperatura da CPU
      lcd.print(buffer_lcd);             // Imprime o valor da temperatura no LCD
      lcd.write(byte(0));                // Imprime o caractere customizado de grau (índice 0)
      buffer_lcd = "C MON:" + String(flag_monitor ? "ON" : "OFF");  // Formata a unidade e o status do monitor
      lcd.print(buffer_lcd);             // Imprime o status do monitor na primeira linha

      lcd.setCursor(0, 1);               // Posiciona o cursor na coluna 0, linha 1
      buffer_lcd = "PWM:" + String(duty_cycle) + "% LED:" + String(flag_led ? "ON" : "OFF");  // Formata PWM e LED
      lcd.print(buffer_lcd);             // Imprime os parâmetros de PWM e LED na segunda linha
      break;

    case TELA_TEMPERATURA:
      lcd.setCursor(0, 0);               // Posiciona o cursor na coluna 0, linha 0
      lcd.print("TEMPERATURA CPU ");     // Exibe o cabeçalho da tela de temperatura

      lcd.setCursor(6, 1);               // Posiciona o cursor na coluna 6, linha 1
      buffer_lcd = String(tempAtual);    // Converte a temperatura atual para texto
      lcd.print(buffer_lcd);             // Imprime o valor numérico da temperatura
      lcd.write(byte(0));                // Imprime o caractere customizado de grau (índice 0)
      buffer_lcd = "C";                  // Define o símbolo da unidade Celsius
      lcd.print(buffer_lcd);             // Imprime a unidade Celsius no LCD
      break;

    case TELA_MONITOR:
      lcd.setCursor(0, 0);               // Posiciona o cursor na coluna 0, linha 0
      lcd.print("CONFIG. MONITOR");      // Exibe o título da tela de configuração do monitor
      lcd.setCursor(6, 1);               // Posiciona o cursor na coluna 6, linha 1
      lcd.print(flag_monitor ? "ON" : "OFF");  // Exibe o estado atual do monitor serial (ON/OFF)
      break;

    case TELA_PWM:
      lcd.setCursor(0, 0);               // Posiciona o cursor na coluna 0, linha 0
      lcd.print("CONFIG. DUTY PWM");     // Exibe o título da tela de configuração do PWM
      lcd.setCursor(6, 1);               // Posiciona o cursor na coluna 6, linha 1
      buffer_lcd = String(duty_cycle) + "%";   // Formata o percentual do ciclo de trabalho
      lcd.print(buffer_lcd);             // Imprime o percentual do PWM no LCD
      break;

    case TELA_LED:
      lcd.setCursor(0, 0);               // Posiciona o cursor na coluna 0, linha 0
      lcd.print("CONFIGURA LED");        // Exibe o título da tela de configuração do LED
      lcd.setCursor(6, 1);               // Posiciona o cursor na coluna 6, linha 1
      lcd.print(flag_led ? "ON" : "OFF");// Exibe o estado atual do pisca-pisca do LED (ON/OFF)
      break;
  }
}

void trata_botoes(void) 
{
  if(cmd != BTN_NENHUM)                  // Verifica se houve algum comando válido de botão
  {
      timeout = TEMPO_TIMEOUT;           // Reinicia o temporizador de retorno automático à tela principal
      lcd.clear();                       // Limpa o display LCD para atualização da nova tela ou valor
  }

  if(cmd == BTN_RIGHT)                   // Verifica se o botão RIGHT foi pressionado
  {
    estadoAtual++;                       // Avança para a próxima tela do menu
    if(estadoAtual > TELA_LED)           // Verifica se ultrapassou a última tela do menu
    {
      estadoAtual = TELA_PRINCIPAL;      // Retorna ciclicamente para a tela principal
    }
  }
  if(cmd == BTN_LEFT)                    // Verifica se o botão LEFT foi pressionado
  {
    estadoAtual--;                       // Retrocede para a tela anterior do menu
    if(estadoAtual < TELA_PRINCIPAL)     // Verifica se recuou antes da tela principal
    {
      estadoAtual = TELA_LED;            // Avança ciclicamente para a última tela do menu
    }
  }

  switch (estadoAtual)                   // Executa as ações específicas dos botões na tela ativa
  {
    case TELA_MONITOR:
      if(cmd == BTN_SELECT) flag_monitor = !flag_monitor;  // Alterna o estado do monitor serial ao pressionar SELECT
      break;

    case TELA_PWM:
      if(cmd == BTN_UP)    duty_cycle++; // Incrementa o ciclo de trabalho PWM ao pressionar UP
      if(cmd == BTN_DOWN)  duty_cycle--; // Decrementa o ciclo de trabalho PWM ao pressionar DOWN
      if(duty_cycle > 100) duty_cycle = 100;  // Limita o ciclo de trabalho máximo em 100%
      if(duty_cycle < 0)   duty_cycle = 0;    // Limita o ciclo de trabalho mínimo em 0%
      duty_pwm = map(duty_cycle, 0, 100, 0, 255);  // Converte a escala percentual (0-100%) para 8 bits (0-255)
      analogWrite(3, duty_pwm);          // Atualiza o sinal PWM no pino digital 3
      break;
      
    case TELA_LED:
      if(cmd == BTN_SELECT) flag_led = !flag_led;          // Alterna o estado do LED intermitente ao pressionar SELECT
      break; 
  }      
}