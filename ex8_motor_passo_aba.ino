/* OITAVO EXEMPLO - ABA AUXILIAR (FUNÇÕES DE CONTROLE DO MOTOR DE PASSO)
   DANIEL RODRIGUES DE SOUSA 27/09/2019
   PROJETO: https://wokwi.com/projects/481673989340199937 */

// Tabela de sequência de acionamento para motor unipolar em meio passo (8 estados)
const char motor_unipolar_meio_passo[ 8 ] = { 0b00001000, 
                                              0b00001100, 
                                              0b00000100, 
                                              0b00000110, 
                                              0b00000010, 
                                              0b00000011, 
                                              0b00000001, 
                                              0b00001001 };

// Tabela de sequência de acionamento para motor unipolar em passo completo (4 estados)
const char motor_unipolar_passo_comp[ 4 ] = { 0b00001000,
                                              0b00000100,
                                              0b00000010,
                                              0b00000001 };

// Tabela de sequência de acionamento para motor bipolar em meio passo (8 estados)
const char motor_bipolar_meio_passo[ 8 ] =  { 0b00001001,
                                              0b00001000,
                                              0b00001010,
                                              0b00000010,
                                              0b00000110,
                                              0b00000100,
                                              0b00000101,
                                              0b00000001 };

// Tabela de sequência de acionamento para motor bipolar em passo completo (4 estados)
const char motor_bipolar_passo_comp[ 4 ] =  { 0b00001010,
                                              0b00001001,
                                              0b00000101,
                                              0b00000110 };

unsigned char state_motor_controle;  // Armazena o estado de controle da direção do motor

void motor_passo_unipolar( unsigned int numero_passos, unsigned int tempo, unsigned modo, unsigned char sentido )
{
  state_motor_controle = sentido;              // Define o sentido de rotação do motor unipolar
  while( numero_passos )                       // Executa o laço enquanto restarem passos a percorrer
  {
    if( modo == PASSO_COMPLETO )               // Verifica se o modo selecionado é passo completo
    {
      sm_motor_passo_unipolar_passo_comp();    // Avança um passo na máquina de estados de passo completo
    }
    else                                       // Caso contrário, executa em meio passo
    {
      sm_motor_passo_unipolar_meio_passo();    // Avança um passo na máquina de estados de meio passo
    }
    delay(tempo);                              // Aguarda o intervalo entre passos em milissegundos
    numero_passos--;                           // Decrementa o contador de passos restantes
  }
}

void motor_passo_bipolar( unsigned int numero_passos, unsigned int tempo, unsigned modo, unsigned char sentido )
{
  state_motor_controle = sentido;              // Define o sentido de rotação do motor bipolar
  while( numero_passos )                       // Executa o laço enquanto restarem passos a percorrer
  {
    if( modo == PASSO_COMPLETO )               // Verifica se o modo selecionado é passo completo
    {
      sm_motor_passo_bipolar_passo_comp();     // Avança um passo na máquina de estados de passo completo
    }
    else                                       // Caso contrário, executa em meio passo
    {
      sm_motor_passo_bipolar_meio_passo();     // Avança um passo na máquina de estados de meio passo
    }
    delay(tempo);                              // Aguarda o intervalo entre passos em milissegundos
    numero_passos--;                           // Decrementa o contador de passos restantes
  }
}

void sm_motor_passo_unipolar_passo_comp( void )
{ 
  static unsigned char state_motor_passo;  // Armazena o índice atual da tabela de estados (0 a 3)

  switch( state_motor_controle )           // Seleciona a ação conforme o sentido de rotação configurado
  {
    case MOTOR_PARADO:
      motor_passo( 0b00000000 );                                  // Desenergiza todas as bobinas do motor
      break;

    case MOTOR_HORARIO:
      state_motor_passo = (state_motor_passo + 1) & 0x03;         // Incrementa o índice circularmente de 0 a 3
      motor_passo( motor_unipolar_passo_comp[ state_motor_passo ] );  // Aciona as bobinas correspondentes ao estado atual
      break;

    case MOTOR_ANTI_HORARIO:
      state_motor_passo = (state_motor_passo - 1) & 0x03;         // Decrementa o índice circularmente de 3 a 0
      motor_passo( motor_unipolar_passo_comp[ state_motor_passo ] );  // Aciona as bobinas correspondentes ao estado atual
      break;
  }
}

void sm_motor_passo_unipolar_meio_passo( void )
{ 
  static unsigned char state_motor_passo;  // Armazena o índice atual da tabela de estados (0 a 7)

  switch( state_motor_controle )           // Seleciona a ação conforme o sentido de rotação configurado
  {
    case MOTOR_PARADO:
      motor_passo( 0b00000000 );                                  // Desenergiza todas as bobinas do motor
      break;

    case MOTOR_HORARIO:
      state_motor_passo = (state_motor_passo + 1) & 0x07;         // Incrementa o índice circularmente de 0 a 7
      motor_passo( motor_unipolar_meio_passo[ state_motor_passo ] );  // Aciona as bobinas correspondentes ao estado atual
      break;

    case MOTOR_ANTI_HORARIO:
      state_motor_passo = (state_motor_passo - 1) & 0x07;         // Decrementa o índice circularmente de 7 a 0
      motor_passo( motor_unipolar_meio_passo[ state_motor_passo ] );  // Aciona as bobinas correspondentes ao estado atual
      break;
  }
}

void sm_motor_passo_bipolar_passo_comp( void )
{ 
  static unsigned char state_motor_passo;  // Armazena o índice atual da tabela de estados (0 a 3)

  switch( state_motor_controle )           // Seleciona a ação conforme o sentido de rotação configurado
  {
    case MOTOR_PARADO:
      motor_passo( 0b00000000 );                                  // Desenergiza todas as bobinas do motor
      break;

    case MOTOR_HORARIO:
      state_motor_passo = (state_motor_passo + 1) & 0x03;         // Incrementa o índice circularmente de 0 a 3
      motor_passo( motor_bipolar_passo_comp[ state_motor_passo ] );   // Aciona as bobinas correspondentes ao estado atual
      break;

    case MOTOR_ANTI_HORARIO:
      state_motor_passo = (state_motor_passo - 1) & 0x03;         // Decrementa o índice circularmente de 3 a 0
      motor_passo( motor_bipolar_passo_comp[ state_motor_passo ] );   // Aciona as bobinas correspondentes ao estado atual
      break;
  }
}

void sm_motor_passo_bipolar_meio_passo( void )
{ 
  static unsigned char state_motor_passo;  // Armazena o índice atual da tabela de estados (0 a 7)

  switch( state_motor_controle )           // Seleciona a ação conforme o sentido de rotação configurado
  {
    case MOTOR_PARADO:
      motor_passo( 0b00000000 );                                  // Desenergiza todas as bobinas do motor
      break;

    case MOTOR_HORARIO:
      state_motor_passo = (state_motor_passo + 1) & 0x07;         // Incrementa o índice circularmente de 0 a 7
      motor_passo( motor_bipolar_meio_passo[ state_motor_passo ] );   // Aciona as bobinas correspondentes ao estado atual
      break;

    case MOTOR_ANTI_HORARIO:
      state_motor_passo = (state_motor_passo - 1) & 0x07;         // Decrementa o índice circularmente de 7 a 0
      motor_passo( motor_bipolar_meio_passo[ state_motor_passo ] );   // Aciona as bobinas correspondentes ao estado atual
      break;
  }
}

void motor_passo_inicializacao( void )
{
  pinMode(BOBINA1, OUTPUT);  // Define o pino da bobina 1 como saída
  pinMode(BOBINA2, OUTPUT);  // Define o pino da bobina 2 como saída
  pinMode(BOBINA3, OUTPUT);  // Define o pino da bobina 3 como saída
  pinMode(BOBINA4, OUTPUT);  // Define o pino da bobina 4 como saída
}

void motor_passo( char motor )  // Função motor_passo: recebe um byte contendo o estado de cada bobina
{
  digitalWrite(BOBINA1, bitRead(motor, 0));  // Atualiza o estado da BOBINA1 conforme o bit 0 de motor
  digitalWrite(BOBINA2, bitRead(motor, 1));  // Atualiza o estado da BOBINA2 conforme o bit 1 de motor
  digitalWrite(BOBINA3, bitRead(motor, 2));  // Atualiza o estado da BOBINA3 conforme o bit 2 de motor
  digitalWrite(BOBINA4, bitRead(motor, 3));  // Atualiza o estado da BOBINA4 conforme o bit 3 de motor
}