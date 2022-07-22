#define Qtd_Amostras 100    // ***Quantas Amostras o filtro terá para filtrar, mude para testar outros filtros***
#define Intervalo_Amostragem 1 // ***definindo o intervalo de amostragem em ms. Mude para testar novos filtros***

int Leitura_analogica = 0; // Variável global que salva o dado bruto lido da porta serial.
// A estratégia usada aqui é porque o comando analogRead possui um custo alto para o Arduino.
// Com isso salvamos na variável para essa leitura ser feita apenas uma vez a cada interação de loop. 

unsigned long timer1=0; // A variável que irá contar o útimo 

void setup() {
  Serial.begin(9600); // Inicio da comunicação serial
  Serial.println("CLEARDATA"); // Comando para o Serial Plotter
  Serial.println("Sem_Filtro, Filtro_Recursivo"); // Inicia os titulos dos eixos do Serial Plotter.
}

void loop() {
  Leitura_analogica = analogRead(A0);  // Leitura_analogica aqui é o valor bruto
  Amostragem(); // Essa é a função que fará a amostragem no tempo que determinamos no intervalo de amostragem que definimos na segunda linha de código.
  Serial.print(Leitura_analogica); // Imprime o dado bruto
  Serial.print(",");
  Serial.println(filtroRecursivo(0)); // Imprime o dado filtrado
  // A função recebe o valor 0 que é para saber que não é para alterar o seu valor, somente imprimir o que está lá.
}

void Amostragem(){ // Essa função verifica se o tempo de amostragem  selecionado ocorreu
  if(millis() - timer1>Intervalo_Amostragem){ // Caso o tempo de amostragem tenha ocorrido, ele envia 1 para a função de filtro de media movel
    //Dessa forma a função sabe que é para atualizar o valor de saída para um novo valor filtrado
    filtroRecursivo(1);
    timer1 = millis(); // atualiza para contar o tempo mais uma vez
  }
}

float filtroRecursivo(bool atualiza_saida){ // Implementação matematica do filtro de 1a ordem analogico.
  
  static float Saida_Filtro = 0;

  if(atualiza_saida == 0) return((double)Saida_Filtro);
  
  else{
  Saida_Filtro += (float)(Leitura_analogica - Saida_Filtro)/(float)Qtd_Amostras;
  }
}