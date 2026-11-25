/* NONO EXEMPLO (CONTROLE DE MOTOR DE PASSO UNIPOLAR E BIPOLAR - USO DE BIBLIOTECA)
   DANIEL RODRIGUES DE SOUSA 27/09/2019 */

#define BOBINA1   6  // Definição do pino da bobina 1 do motor de passo (sobrescreve o padrão da biblioteca)
#define BOBINA2   7  // Definição do pino da bobina 2 do motor de passo (sobrescreve o padrão da biblioteca)
#define BOBINA3   8  // Definição do pino da bobina 3 do motor de passo (sobrescreve o padrão da biblioteca)
#define BOBINA4   9  // Definição do pino da bobina 4 do motor de passo (sobrescreve o padrão da biblioteca)

#include <motor_passo.h>  // Inclui a biblioteca customizada de controle de motor de passo

String buffer_serial = "";  // Buffer para armazenar os comandos recebidos pela serial

void setup() {
  Serial.begin(9600);           // Configura a velocidade de transmissão serial em 9600 bps
  Serial.setTimeout(10);        // Configura o tempo limite (timeout) de recepção de dados em 10 ms

  motor_passo_inicializacao();  // Inicializa os pinos de saída das bobinas definidos antes da inclusão

  Serial.println("A -> Bipolar: passo completo e sentido horario");       // Exibe o menu de comandos via monitor serial
  Serial.println("a -> Bipolar: passo completo e sentido anti horario");    
  Serial.println("B -> Bipolar: meio passo e sentido horario");
  Serial.println("b -> Bipolar: meio passo e sentido anti horario");
  Serial.println("C -> Unipolar: passo completo e sentido horario");
  Serial.println("c -> Unipolar: passo completo e sentido anti horario");
  Serial.println("D -> Unipolar: meio passo e sentido horario");
  Serial.println("d -> Unipolar: meio passo e sentido anti horario"); 
}

void loop() {
  if(Serial.available() > 0)  // Verifica se há dados disponíveis no buffer da porta serial
  {
    buffer_serial = Serial.readStringUntil('\n');  // Lê a string recebida até o caractere de nova linha
 
    if( buffer_serial == "A" )  // Verifica se o comando recebido é 'A'
    {
      Serial.println("Bipolar: passo completo e sentido horario");     // Informa o modo selecionado no monitor serial
      motor_passo_bipolar(100, 100, PASSO_COMPLETO, MOTOR_HORARIO );   // Aciona o motor bipolar por 100 passos no sentido horário
    }

    if( buffer_serial == "a" )  // Verifica se o comando recebido é 'a'
    {
      Serial.println("Bipolar: passo completo e sentido anti horario");     // Informa o modo selecionado no monitor serial
      motor_passo_bipolar(100, 100, PASSO_COMPLETO, MOTOR_ANTI_HORARIO );   // Aciona o motor bipolar por 100 passos no sentido anti-horário
    }

    if( buffer_serial == "B" )  // Verifica se o comando recebido é 'B'
    {
      Serial.println("Bipolar: meio passo e sentido horario");         // Informa o modo selecionado no monitor serial
      motor_passo_bipolar(100, 100, MEIO_PASSO, MOTOR_HORARIO );       // Aciona o motor bipolar em meio passo no sentido horário
    }

    if( buffer_serial == "b" )  // Verifica se o comando recebido é 'b'
    {
      Serial.println("Bipolar: meio passo e sentido anti horario");    // Informa o modo selecionado no monitor serial
      motor_passo_bipolar(100, 100, MEIO_PASSO, MOTOR_ANTI_HORARIO );  // Aciona o motor bipolar em meio passo no sentido anti-horário
    }

    if( buffer_serial == "C" )  // Verifica se o comando recebido é 'C'
    {
      Serial.println("Unipolar: passo completo e sentido horario");    // Informa o modo selecionado no monitor serial
      motor_passo_unipolar(100, 100, PASSO_COMPLETO, MOTOR_HORARIO );  // Aciona o motor unipolar em passo completo no sentido horário
    }

    if( buffer_serial == "c" )  // Verifica se o comando recebido é 'c'
    {
      Serial.println("Unipolar: passo completo e sentido anti horario");    // Informa o modo selecionado no monitor serial
      motor_passo_unipolar(100, 100, PASSO_COMPLETO, MOTOR_ANTI_HORARIO );  // Aciona o motor unipolar em passo completo no sentido anti-horário
    }

    if( buffer_serial == "D" )  // Verifica se o comando recebido é 'D'
    {
      Serial.println("Unipolar: meio passo e sentido horario");        // Informa o modo selecionado no monitor serial
      motor_passo_unipolar(100, 100, MEIO_PASSO, MOTOR_HORARIO );      // Aciona o motor unipolar em meio passo no sentido horário
    }

    if( buffer_serial == "d" )  // Verifica se o comando recebido é 'd'
    {
      Serial.println("Unipolar: meio passo e sentido anti horario");   // Informa o modo selecionado no monitor serial
      motor_passo_unipolar(100, 100, MEIO_PASSO, MOTOR_ANTI_HORARIO ); // Aciona o motor unipolar em meio passo no sentido anti-horário
    }
  }
}