/* PRIMEIRO PROGRAMA (JOGO GENIUS / SIMON)
   DANIEL RODRIGUES DE SOUSA 04/04/2020 
   PROJETO: https://wokwi.com/projects/475274789705804801 */

/*---------------------------------------------------------
    REQUER A BIBLIOTECA anyrtttl
    INSTALAR EM FERRAMENTAS > GERENCIAR BIBLIOTECAS.
    NO CAMPO REFINE A SUA BUSCA, DIGITE RTTTL. APONTE PARA
    O anyrtttl E CLIQUE EM INSTALAR.
  --------------------------------------------------------*/

#include <anyrtttl.h>  // Inclui a biblioteca anyrtttl para reprodução de melodias RTTTL
#include <binrtttl.h>  // Inclui a biblioteca binrtttl de suporte ao formato RTTTL
#include <pitches.h>   // Inclui a biblioteca pitches com as definições das notas musicais

// Músicas no formato RTTTL
const char * SuperMarioWorld     = "SuperMar:d=4,o=5,b=125:a,8f.,16c,16d,16f,16p,f,16d,16c,16p,16f,16p,16f,16p,8c6,8a.,g,16c,a,8f.,16c,16d,16f,16p,f,16d,16c,16p,16f,16p,16a#,16a,16g,2f,16p,8a.,8f.,8c,8a.,f,16g#,16f,16c,16p,8g#.,2g,8a.,8f.,8c,8a.,f,16g#,16f,8c,2c6";
const char * SuperMarioBros      = "SuperMar:d=4,o=5,b=125:32p,8g,16c,8e,8g.,16c,8e,16g,16c,16e,16g,8b,a,8p,16c,8g,16c,8e,8g.,16c,8e,16g,16c#,16e,16g,8b,a,8p,16b,8c6,16b,8c6,8a.,16c6,8b,16a,8g,16f#,8g,8e.,16c,8d,16e,8f,16e,8f,8b.4,16e,8d.,c";
const char * SuperMarioBrosDeath = "DeathMus:d=16,o=5,b=75:b,f6,p,f6,f6,f.6,e.6,d6,c6,g,p,e,c";

const unsigned int notas[4] = {NOTE_E6, NOTE_CS5, NOTE_A5, NOTE_E5};  // Define as notas musicais para cada tecla/sequência

#define TAMANHO_JOGO  5   // Define o tamanho da sequência a ser memorizada

#define LED1    13        // Definição dos pinos dos LEDs, botões e buzzer
#define LED2    12
#define LED3    11
#define LED4    10

#define BOT1    9
#define BOT2    8
#define BOT3    7
#define BOT4    6

#define BUZZER  2

unsigned char jogo[TAMANHO_JOGO]; // Variáveis utilizadas no programa
unsigned char contador;
unsigned char indice_jogo;
unsigned char botao;

void liga_leds(unsigned char led);  // Prototipagem de funções
unsigned char retorna_botao(void);
void gera_sequencia(void);

void setup() {

  pinMode(LED1, OUTPUT);  // Definição de entradas e saídas
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(BOT1, INPUT);
  pinMode(BOT2, INPUT);
  pinMode(BOT3, INPUT);
  pinMode(BOT4, INPUT);

  randomSeed(analogRead(0));  // Inicializa a semente aleatória com leitura analógica
  gera_sequencia();           // Gera a sequência do jogo de forma aleatória

  indice_jogo = 0;            // Inicializa o índice da sequência atual do jogo
}

void loop() {
  //---------------------------------------------------------
  //                   INICIA O JOGO
  //---------------------------------------------------------

  if(indice_jogo == 0)  // Verifica se o jogo ainda não foi iniciado
  {
    while(retorna_botao() == 0) delay(50);             // Aguarda qualquer tecla ser pressionada
    
    anyrtttl::blocking::play(BUZZER, SuperMarioWorld); // Toca a música de abertura
  
    liga_leds(0xFF);   // Indica o início do jogo acendendo todos os LEDs
    delay(1000);       // Aguarda 1 segundo
    liga_leds(0);      // Apaga todos os LEDs
    delay(1000);       // Aguarda 1 segundo
  }

  //---------------------------------------------------------
  //                 MOSTRA A SEQUÊNCIA
  //---------------------------------------------------------

  contador = 0;  // Reinicia o contador para exibir a sequência atual

  while(contador <= indice_jogo)  // Enquanto o contador for menor ou igual ao índice, mostra a sequência
  {
    liga_leds(jogo[contador]);                  // Liga o LED correspondente à posição atual

    tone(BUZZER, notas[jogo[contador]-1], 100); // Gera o tom correspondente ao LED
    
    delay(1000);                  // Aguarda 1 segundo
    liga_leds(0);                 // Desliga os LEDs
    delay(1000);                  // Aguarda 1 segundo
    contador++;                   // Incrementa o contador para o próximo passo
  }

  //---------------------------------------------------------
  //                    JOGADOR JOGA
  //---------------------------------------------------------

  contador = 0;  // Reinicia o contador para ler a sequência do jogador
  
  while(contador <= indice_jogo)  // Enquanto o contador for menor ou igual ao índice, aguarda jogadas
  {
    liga_leds(0);                 // Desliga os LEDs

    while(retorna_botao() == 0) delay(50);  // Aguarda qualquer tecla ser pressionada

    botao = retorna_botao();      // Grava na variável botão a tecla pressionada
    
    tone(BUZZER, notas[botao-1], 100);      // Gera o tom correspondente ao botão pressionado
    
    liga_leds(botao);             // Liga o LED correspondente ao botão
    while(retorna_botao() != 0) delay(50);  // Aguarda a liberação das teclas
    liga_leds(0);                 // Desliga os LEDs
    
    if(botao == jogo[contador])   // Verifica se o botão pressionado corresponde ao valor gerado
    {
      // Acerto
      contador++;                 // Aponta para a próxima posição da sequência
      if(contador > indice_jogo)  // Verifica se o jogador completou a sequência atual
      {
          indice_jogo++;          // Incrementa o índice para adicionar um novo passo
          break;                  // Sai do laço while
      }
    }
    else                          // Caso o botão pressionado seja diferente do valor gerado
    {
      // Erro
      tone(BUZZER, NOTE_B4, 1000); // Gera o tom de erro
      
      liga_leds(0xFF);             // Indica nos LEDs que o jogador errou
      delay(500);                  // Aguarda 500 milissegundos
      liga_leds(0);                // Desliga os LEDs
      delay(500);                  // Aguarda 500 milissegundos
      liga_leds(0xFF);             // Liga todos os LEDs novamente
      delay(500);                  // Aguarda 500 milissegundos
      liga_leds(0);                // Desliga os LEDs
    
      anyrtttl::blocking::play(BUZZER, SuperMarioBrosDeath); // Toca a música de derrota
    
      indice_jogo = 0;             // Reinicia o índice do jogo
      break;                       // Sai do laço while
    }    
  }

  if(indice_jogo == TAMANHO_JOGO) // Verifica se o índice atingiu o tamanho máximo a ser memorizado
  {
    delay(250);                   // Aguarda 250 milissegundos
    liga_leds(1);                 // Indica nos LEDs que finalizou o jogo
    delay(250);                   // Aguarda 250 milissegundos
    liga_leds(2);                 // Liga o LED2
    delay(250);                   // Aguarda 250 milissegundos
    liga_leds(3);                 // Liga o LED3
    delay(250);                   // Aguarda 250 milissegundos
    liga_leds(4);                 // Liga o LED4
    delay(250);                   // Aguarda 250 milissegundos
    liga_leds(0);                 // Desliga todos os LEDs
    
    anyrtttl::blocking::play(BUZZER, SuperMarioBros); // Toca a música de vitória

    gera_sequencia();             // Gera uma nova sequência do jogo de forma aleatória
    
    indice_jogo = 0;              // Reinicia o índice do jogo
  }
  
  delay(2000);                    // Aguarda 2 segundos antes de reiniciar o ciclo
}

void liga_leds(unsigned char led)
{
  digitalWrite(LED1, LOW);  // Desliga todos os LEDs
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  
  if(led == 1) digitalWrite(LED1, HIGH);  // Se led = 1, liga LED1
  if(led == 2) digitalWrite(LED2, HIGH);  // Se led = 2, liga LED2
  if(led == 3) digitalWrite(LED3, HIGH);  // Se led = 3, liga LED3
  if(led == 4) digitalWrite(LED4, HIGH);  // Se led = 4, liga LED4
  if(led == 0xFF)                         // Se led = 0xFF, liga todos os LEDs
  {
    digitalWrite(LED1, HIGH);   
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
  } 
}

unsigned char retorna_botao(void)
{ // Para cada tecla, retorna um valor; caso contrário, retorna 0
  if(digitalRead(BOT1) == 0)      return(1);
  else if(digitalRead(BOT2) == 0) return(2);
  else if(digitalRead(BOT3) == 0) return(3);
  else if(digitalRead(BOT4) == 0) return(4);
  else                            return(0);  
}

void gera_sequencia(void)
{
  contador = 0;                     // Inicializa o contador
  do
  {
    jogo[contador] = random(1, 5);  // Gera um número aleatório de 1 a 4 e armazena no vetor
    contador++;                     // Incrementa o índice
  }while(contador < TAMANHO_JOGO);  // Preenche até o tamanho máximo
}