#include <LiquidCrystal.h>
#include <stdlib.h>
LiquidCrystal tela(13, 12, 11,10, 9, 8);

bool avalia_jogo_leds = false, avalia_jogo_perguntas = false, avalia_pergunta_final = false;

const int TAM_LEDS = 5;
const int TAM_PERGUNTAS = 5;

const int BOTAO_INICIA = 3;
const int BOTAO_ESQUERDO = 7;
const int BOTAO_DIREITO = 6;

const int LEDS_ESQUERDO = 5;
const int LEDS_DIREITO = 4;

const int BUZZER = 2;

int leds[TAM_LEDS];
char mostra_sequencia[50];

String perguntas[TAM_PERGUNTAS] = {"2 + 2 = 5?", "3 + 5 = 8?", "7 eh primo?", "3 + 8 = 12 ?", "11 eh primo ?"};
String respostas[TAM_PERGUNTAS] = {"Nao", "Sim", "Sim", "Nao", "Sim"};

void setup()
{
  randomSeed(analogRead(0));

  pinMode(BOTAO_INICIA, INPUT_PULLUP);
  pinMode(BOTAO_DIREITO, INPUT_PULLUP);
  pinMode(BOTAO_ESQUERDO,INPUT_PULLUP);
  
  pinMode(LEDS_ESQUERDO, OUTPUT);
  pinMode(LEDS_DIREITO, OUTPUT);
  
  tela.begin(16, 2);
  Serial.begin(9600);
  
}


void loop()
{	
  
  home();
  
  inicia();  

  //jogo_leds();

  jogo_perguntas();
  
}


void jogo_leds()
{
  if(avalia_jogo_leds)
  {
    limpa_tela();
    tela.print("Fase 1/3 : Leds");
    
    delay(1000);
    
    limpa_tela();
    tela.print("Memorize os leds");

    delay(1000);
	
    preenche_aleatorio();
    exibe_leds();
    
    limpa_tela();
    tela.print("Sua vez agora.");
    
    if(compara())
    {
      vitoria();
    }
    else
    {
      derrota();
    }
    delay(1000);
  
  }
}

void jogo_perguntas()
{
  if(avalia_jogo_perguntas)
  {
    limpa_tela();
    tela.print("Fase 2/3 : Perguntas");

    delay(1000);

    perrcorre_perguntas();
  }
  
}

void perrcorre_perguntas(){
  int valida_uso[TAM_PERGUNTAS];
  int aux = 0;

  while(aux != TAM_PERGUNTAS)
  {
    int pergunta = random(5);
    bool confirma = true;

    for(int j = 0; j < aux; j++){
      if(valida_uso[j] == pergunta) confirma = false;  
    }  

    if(confirma)
    {
      limpa_tela();
      tela.print(perguntas[pergunta]);

      tela.setCursor(0, 1);
      tela.print("Sim || Nao");

      valida_uso[aux] = pergunta;
      aux++;
      delay(3000);
    }
  }

}

void inicia(){
  int botao_play = digitalRead(BOTAO_INICIA);
  
  if(botao_play == LOW)
  {
    digitalWrite(BOTAO_INICIA, HIGH);
    avalia_jogo_perguntas = true;
  }  
}

void preenche_aleatorio(){
  for(int i =0; i< TAM_LEDS; i++)
  {
    leds[i] = random(2);
  }
}

void exibe_leds(){
  for(int i = 0; i < TAM_LEDS; i++)
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
    
    }
}

int compara (){
  int valida[TAM_LEDS];
  int cont = 0;

  delay(1000);
  
    while(true){

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
}

void home(){
  tela.setCursor(0, 0);
  tela.print("Aperte o 1 botao");
  
  tela.setCursor(0, 1);
  tela.print("Para comecar");
}

void limpa_tela()
{
  tela.clear();
  tela.setCursor(0, 0);
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
  limpa_tela();
  tela.print("Sucesso!");

  musica_vitoria();
  delay(4000);

  avalia_jogo_perguntas = true;
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

  delay(1000);

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
  
  delay(1000);
}