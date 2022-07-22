#define Qtd_Amostras 10    // ***Quantas Amostras o filtro terá para filtrar, mude para testar outros filtros***
#define Intervalo_Amostragem 1 // ***definindo o intervalo de amostragem em ms. Mude para testar novos filtros***

int Leitura_analogica = 0; // Variável global que salva o dado bruto lido da porta serial.
// A estratégia usada aqui é porque o comando analogRead possui um custo alto para o Arduino.
// Com isso salvamos na variável para essa leitura ser feita apenas uma vez a cada interação de loop. 

unsigned long timer1=0; // A variável que irá contar o útimo 

void setup() {
  Serial.begin(9600); // Inicio da comunicação serial
  Serial.println("CLEARDATA"); // Comando para o Serial Plotter
  Serial.println("Sem_Filtro, Filtro_Media_Movel"); // Inicia os titulos dos eixos do Serial Plotter.
}

void loop() {
  Leitura_analogica = analogRead(A0);  // Leitura_analogica aqui é o valor bruto
  Amostragem(); // Essa é a função que fará a amostragem no tempo que determinamos no intervalo de amostragem que definimos na segunda linha de código.
  Serial.print(Leitura_analogica); // Imprime o dado bruto
  Serial.print(",");
  Serial.println(filtroMediaMovel(0)); // Imprime o dado filtrado
  // A função recebe o valor 0 que é para saber que não é para alterar o seu valor, somente imprimir o que está lá.
}

void Amostragem(){ // Essa função verifica se o tempo de amostragem  selecionado ocorreu
  if(millis() - timer1>Intervalo_Amostragem){ // Caso o tempo de amostragem tenha ocorrido, ele envia 1 para a função de filtro de media movel
    //Dessa forma a função sabe que é para atualizar o valor de saída para um novo valor filtrado
    filtroMediaMovel(1);
    timer1 = millis(); // atualiza para contar o tempo mais uma vez
  }
}

float filtroMediaMovel(bool atualiza_saida){ // A função de media movel trabalha com variavel estática, que salva as variaveis sem perder
  static  int Leituras_anteriores[Qtd_Amostras]; // Esse é o vetor que servirá como buffer circular
  static  int Posicao = 0; // A posicao atual de leitura, que deverá ficar salva
  static long Soma=0; // A soma total do buffer circular
  static float Media = 0; // A media, que é a saída da função quando é chamada
  static bool zera_vetor = 1;  // A variavel para saber se é a primeira execução. Se for, ele zera todo o buffer circular.
  
  if (zera_vetor){ // Zerando todo o buffer circular, para que as subtrações das sobrescrição não atrapalhe o filtro
    for(int i=0; i<= Qtd_Amostras; i++){
      Leituras_anteriores[i] = 0;
    }
    zera_vetor = 0;
//    Serial.println("Não entra mais no laço");
  }

  if(atualiza_saida == 0) return((double)Media); // Se o parametro recebido na funcao for zero, ele retorna somente o valor de media calculado anteriormente
  
  else{ // Caso seja 1, signfica que está no tempo de amostragem, e ai atualiza a variável média
  Soma = Leitura_analogica - Leituras_anteriores[Posicao%Qtd_Amostras] + Soma;
  Leituras_anteriores[Posicao%Qtd_Amostras] = Leitura_analogica;
  Media = (float)Soma/(float)(Qtd_Amostras);
  Posicao = (Posicao+1)%Qtd_Amostras;
  return((double)Media);
  }
}