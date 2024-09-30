#include <LiquidCrystal.h>
#include <stdlib.h>
LiquidCrystal tela(13, 12, 11,10, 9, 8);

bool avalia_jogo_leds = false, avalia_jogo_perguntas = false, avalia_pergunta_final = false;

int contador = 0;

const int TAM_LEDS = 5;
const int TAM_PERGUNTAS = 10;

const int BOTAO_INICIA = 3;
const int BOTAO_ESQUERDO = 7;
const int BOTAO_DIREITO = 6;

const int LEDS_ESQUERDO = 5;
const int LEDS_DIREITO = 4;

const int BUZZER = 2;

int leds[TAM_LEDS];
char mostra_sequencia[50];

String perguntas[TAM_PERGUNTAS] = {
  "3 + 5 = 8?", //sim
  "2 + 2 = 5?", // nao
  "7 eh primo?", //sim
  "3 + 8 = 12?",// nao
  "56 / 8 = 7?", //sim
  "12 eh primo ?",//nao
  "13 * 7 = 91?",//sim
  "75 - 24 = 49?",//nao
  "7 * 4 = 28?", //sim
  "Palmeiras tem mundial?"//nao
};


void setup()
{
  randomSeed(analogRead(0));
  pinMode(BOTAO_INICIA, INPUT_PULLUP);
  pinMode(BOTAO_DIREITO, INPUT_PULLUP);
  pinMode(BOTAO_ESQUERDO,INPUT_PULLUP);
  
  pinMode(LEDS_ESQUERDO, OUTPUT);
  pinMode(LEDS_DIREITO, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(BOTAO_INICIA), desiste_inicia, RISING);

  tela.begin(16, 2);
  Serial.begin(9600);
  
}

void loop()
{	
  
  home();
  
  jogo_leds();

  jogo_perguntas();

  pergunta_final(); 
}


void limpa_tela()
{
  tela.clear();
  tela.setCursor(0, 0);
}

void musica_tempo_acabando() {
  int notas[] = {523, 494, 466, 440, 415, 392};
  int duracao = 300;

  for (int i = 0; i < 6; i++) {
    tone(BUZZER, notas[i]);
    delay(duracao);
    noTone(BUZZER);
    delay(100);
    duracao -= 30;
  }

}

void musica_vitoria (){
  tone(BUZZER, 523);

  delay(200);
  noTone(BUZZER);

  tone(BUZZER, 659);  

  delay(200);
  noTone(BUZZER);

  tone(BUZZER, 784);  

  delay(200);
  noTone(BUZZER);

  tone(BUZZER, 1046); 

  delay(500);
  noTone(BUZZER);

}

void musica_derrota(){
  tone(BUZZER, 294);

  delay(500);

  noTone(BUZZER);
  tone(BUZZER, 262);

  delay(500);

  noTone(BUZZER);
  tone(BUZZER, 220);

  delay(1000);

  noTone(BUZZER);
  
}

void desiste_inicia(){

  if(contador == 0){
    avalia_jogo_leds = true;
    contador = 1;
  }
  else{
    musica_derrota();

    limpa_tela();
    tela.print("Desistiu");

    avalia_jogo_leds = false;
    avalia_jogo_perguntas = false;
    avalia_pergunta_final = false;
    contador = 0;
  }
}

void derrota()
{
  musica_derrota();

  limpa_tela();
  tela.print("Errou");

  avalia_jogo_leds = false;
  avalia_jogo_perguntas = false; 
  avalia_pergunta_final = false;
}

void vitoria()
{
  musica_vitoria();

  limpa_tela();
  tela.print("Sucesso!");

  musica_vitoria();
  delay(4000);

}

void jogo_leds()
{
  
  while(avalia_jogo_leds){
    int j = 0;

    limpa_tela();
    tela.print("Fase 1/3 : Leds");
    
    delay(1000);
    
    limpa_tela();
    tela.print("Memorize os leds");

    delay(1000);
	
    preenche_aleatorio();
    exibe_leds();
    
    if(compara() && avalia_jogo_leds == true)
    {
      vitoria();
      avalia_jogo_perguntas = true;
    }
    else
    {
      if(avalia_jogo_leds == true){
        derrota();
      }
    }
    delay(1000);

    if(j == 0)break;
  }
}

void jogo_perguntas()
{
  if(avalia_jogo_perguntas)
  {
    limpa_tela();

    tela.print("Fase 2/3 : Perguntas");
    for(int i = 0; i < 13; i++){
      delay(300);
      tela.scrollDisplayLeft();
    }
    delay(2000);

    percorre_perguntas();
  }
  
}

void percorre_perguntas(){
  int valida_uso[5];//criando vetor de verificacao das perguntas que ja foram selecionadas.
  int aux = 0, questoes_jogadas = 5;
  int verifica_questoes_puladas =0;
  
  while(aux < questoes_jogadas && avalia_jogo_perguntas == true)
  {
    int tempo = 10;
    bool confirma = true;
    int pergunta = random(9); // nessa variavel armazenamos valores aleotrios entre 0 e 9.

    for(int j = 0; j < aux; j++){
      if(valida_uso[j] == pergunta) confirma = false;
    }  

    if(confirma)//se a pergunta nao foi usada printamos na tela, e preenchemos o vetor de verificacao.
    {
      limpa_tela();
      tela.print(perguntas[pergunta]);

      tela.setCursor(0, 1);
      tela.print("Sim || Nao");

      delay(2500);
      
      //nesse while true iremos, esperar o usuario selecionar a opcao de sim ou nao.
      while (tempo != -1 && avalia_jogo_perguntas == true)
      { 
        
        limpa_tela();
        tela.print("Tempo: ");
        tela.print(tempo);
        tela.print(" s");
        
        if( tempo == 3) {
          musica_tempo_acabando();
        }

        delay(1000);

        if(digitalRead(BOTAO_ESQUERDO) == LOW){//quando o usuario clicar botao esquerdo
          tempo = 0;
          while(digitalRead(BOTAO_ESQUERDO) == LOW){//esse while funciona para se o usuario fica segurando o botao.
            digitalWrite(LEDS_ESQUERDO, HIGH);
          }
          delay(500);
          digitalWrite(LEDS_ESQUERDO, LOW);
          if(pergunta % 2 == 0){//Se o indice for par, significa que a resposta é "sim";
            limpa_tela();

            valida_uso[aux] = pergunta;
            aux++;

            if(verifica_questoes_puladas > 0){
              sprintf(mostra_sequencia, "Correto %d/%d", aux-1, 5);
            }
            else{
              sprintf(mostra_sequencia, "Correto %d/%d", aux, 5);
            }
            tela.print(mostra_sequencia);
            tela.setCursor(0, 1);
            tela.print("*Sim || Nao");
            
            break;
          }
          else{
            limpa_tela();
            tela.print("Errou!");
            tela.setCursor(0, 1);
            tela.print("*Sim || Nao");
            //confirma = false;
            aux = 5;
            //avalia_jogo_perguntas = false;
            break;
            derrota();
          }
        }

        if(digitalRead(BOTAO_DIREITO) == LOW){//quando o usuario cliar no botao direito, referente ao botao não.
          tempo = 0;
          while(digitalRead(BOTAO_DIREITO) == LOW){//enquanto o usuario estiver clicando no botao, o led ficara acesso.
            digitalWrite(LEDS_DIREITO, HIGH);
          }
          delay(500);
          digitalWrite(LEDS_DIREITO, LOW);

          if(pergunta %2 != 0){
            limpa_tela();

            valida_uso[aux] = pergunta;
            aux++;

            if(verifica_questoes_puladas > 0){
              sprintf(mostra_sequencia, "Correto %d/%d", aux-1, 5);
            }
            else{
              sprintf(mostra_sequencia, "Correto %d/%d", aux, 5);
            }
            tela.print(mostra_sequencia);
            tela.setCursor(0, 1);
            tela.print("Sim || *Nao");
            
            break;
          }
          else{
            limpa_tela();
            tela.print("Errou!");
            tela.setCursor(0, 1);
            tela.print("Sim || *Nao");
            aux = 10;
            derrota();
            break;
          }

        }

        if(tempo == 0){
          limpa_tela();
          tela.setCursor(0,0);
          tela.print("Questao pulada");
          delay(1500);
          tempo = 0;

          valida_uso[aux] = pergunta;
          
          questoes_jogadas++;
          aux++;
          verifica_questoes_puladas ++;

          if(verifica_questoes_puladas > 1){
            aux = 10;
            limpa_tela();
            tela.print("Perdeu");
            desiste_inicia();
          }

        }
        tempo --;
        
        
      }
      
      delay(3000);
    }

  }

  if(aux == questoes_jogadas){
    vitoria();
    avalia_pergunta_final = true;
  }
  

}

void preenche_aleatorio(){
  for(int i =0; i < TAM_LEDS; i++)
  {
    leds[i] = random(2);
  }
}

void exibe_leds(){
  int i = 0;
  while(avalia_jogo_leds ==  true && i < TAM_LEDS)
  {
    if(leds[i] == 0)
    {
      
      digitalWrite(LEDS_ESQUERDO, HIGH);
    }
    else
    {
      digitalWrite(LEDS_DIREITO, HIGH);
    }
    delay(1000);
    digitalWrite(LEDS_DIREITO, LOW); 
    digitalWrite(LEDS_ESQUERDO, LOW);
    delay(1000);

    i++;
  }
}

int compara (){
  

  int valida[TAM_LEDS];
  int cont = 0;

  delay(1000);
  if(avalia_jogo_leds){
    limpa_tela();
    tela.print("Sua vez agora.");
  }
  while(avalia_jogo_leds){
    
    

    if(digitalRead(BOTAO_ESQUERDO) == LOW)
    {
      while(digitalRead(BOTAO_ESQUERDO) == LOW)
      {
        digitalWrite(LEDS_ESQUERDO, HIGH);
      }

      delay(1000);
      digitalWrite(LEDS_ESQUERDO, LOW);

      if(leds[cont] == 0)
      {
        cont++;
        sprintf(mostra_sequencia, "Correto %d/%d", cont, TAM_LEDS);
        
        limpa_tela();
        tela.print(mostra_sequencia);
      }
      else return 0;
    }

    if(digitalRead(BOTAO_DIREITO) == LOW)
    {
      while(digitalRead(BOTAO_DIREITO) == LOW)
      {
        digitalWrite(LEDS_DIREITO, HIGH);
      }

      delay(1000);
      digitalWrite(LEDS_DIREITO, LOW);

      if(leds[cont] == 1)
      {
        cont++;
        sprintf(mostra_sequencia, "Correto %d/%d", cont, TAM_LEDS);

        limpa_tela();
        tela.print(mostra_sequencia);
      }
      else return 0;

    }

    delay(100);

    if(cont == TAM_LEDS) return 1;
  }   

  return 0;
}

void home(){
  tela.setCursor(0, 0);
  tela.print("Aperte o 1 botao");
  
  tela.setCursor(0, 1);
  tela.print("Para comecar");
}

void pergunta_final(){

  if(avalia_pergunta_final){
    int tempo = 10;
    int pergunta = TAM_PERGUNTAS - 1;

    limpa_tela();
    tela.print("Fase 3/3");
    tela.setCursor(0, 1);
    tela.print("Pergunta Final");
    
    delay(1000);
 
    limpa_tela();
    tela.print(perguntas[pergunta]);
    for(int i = 0; i < 13; i++){
      delay(300);
      tela.scrollDisplayLeft();
    }

    tela.setCursor(0, 1);
    tela.print("Sim || Nao");
    delay(2500);
    
    while (tempo != -1 && avalia_pergunta_final == true)
    { 
      
      limpa_tela();
      tela.print("Tempo: ");
      tela.print(tempo);
      tela.print(" s");
      
      if( tempo == 3) {
        musica_tempo_acabando();
      }

      delay(1000);

      if(digitalRead(BOTAO_ESQUERDO) == LOW){//quando o usuario clicar botao esquerdo
        tempo = 0;
        while(digitalRead(BOTAO_ESQUERDO) == LOW){//esse while funciona para se o usuario fica segurando o botao.
           digitalWrite(LEDS_ESQUERDO, HIGH);
        }
        digitalWrite(LEDS_ESQUERDO, LOW);
        delay(500);

        limpa_tela();
        tela.print("Errou!");

        tela.setCursor(0, 1);
        tela.print("*Sim || Nao");
        
        derrota();
        break;
      
      }

      if(digitalRead(BOTAO_DIREITO) == LOW){//quando o usuario cliar no botao direito, referente ao botao não.
        tempo = 0;
        while(digitalRead(BOTAO_DIREITO) == LOW){//enquanto o usuario estiver clicando no botao, o led ficara acesso.
           digitalWrite(LEDS_DIREITO, HIGH);
        }
        digitalWrite(LEDS_DIREITO, LOW);

        delay(500);

        limpa_tela();
        tela.print("Acertou!");
        tela.setCursor(0, 1);
        tela.print("Sim || *Nao");


        delay(1000);
        limpa_tela();
        tela.setCursor(0, 1);
        vitoria();

        limpa_tela();
        tela.print("PARABENS");

        tela.setCursor(0, 1);
        tela.print("Jogo Concluido");

        avalia_jogo_leds = false;
        avalia_jogo_perguntas = false; 
        avalia_pergunta_final = false;
      }

      tempo --;
      
    }
    
    delay(3000);
  }
}