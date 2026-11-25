/* OITAVO EXEMPLO (CONTROLE DE MOTOR DE PASSO UNIPOLAR E BIPOLAR - USO DE ABAS)
   DANIEL RODRIGUES DE SOUSA 27/09/2019
   PROJETO: https://wokwi.com/projects/481673989340199937 */

#define MOTOR_PARADO        0  // Define o estado de motor parado
#define MOTOR_HORARIO       1  // Define o sentido de rotação horário
#define MOTOR_ANTI_HORARIO  2  // Define o sentido de rotação anti-horário

#define PASSO_COMPLETO      0  // Define o modo de acionamento em passo completo
#define MEIO_PASSO          1  // Define o modo de acionamento em meio passo

#define BOBINA1   2  // Definição do pino da bobina 1 do motor de passo
#define BOBINA2   3  // Definição do pino da bobina 2 do motor de passo
#define BOBINA3   4  // Definição do pino da bobina 3 do motor de passo
#define BOBINA4   5  // Definição do pino da bobina 4 do motor de passo

String buffer_serial = "";  // Buffer para armazenar os comandos recebidos pela serial

void motor_passo_inicializacao( void );                                                                       // Prototipagem das funções definidas na aba auxiliar
void motor_passo_unipolar( unsigned int numero_passos, unsigned int tempo, unsigned modo, unsigned char sentido );
void motor_passo_bipolar( unsigned int numero_passos, unsigned int tempo, unsigned modo, unsigned char sentido );
void sm_motor_passo_unipolar_passo_comp( void );
void sm_motor_passo_unipolar_meio_passo( void );
void sm_motor_passo_bipolar_passo_comp( void );
void sm_motor_passo_bipolar_meio_passo( void );
void motor_passo( char motor );

void setup() {
  Serial.begin(9600);           // Configura a velocidade de transmissão serial em 9600 bps
  Serial.setTimeout(10);        // Configura o tempo limite (timeout) de recepção de dados em 10 ms

  motor_passo_inicializacao();  // Inicializa os pinos de saída das bobinas do motor de passo

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