#include <LiquidCrystal.h>
#include <stdlib.h>
LiquidCrystal tela(13, 12, 11,10, 9, 8);

const int TAM = 10;

const int BOTAO_INICIA = 3;
const int BOTAO_ESQUERDO = 7;
const int BOTAO_DIREITO = 6;

const int LEDS_ESQUERDO = 5;
const int LEDS_DIREITO = 4;

int leds[TAM];






void setup()
{
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
  
  if(inicia())
  {
    tela.clear();
    
    tela.setCursor(0, 0);
    tela.print("Fase 1/3 : Leds");
    
    delay(1000);
    
    tela.setCursor(0, 0);
    tela.print("Memorize os leds");
    delay(1000);
	
    preenche_aleatorio();
    exibe_leds();
    delay(1000);
    

    
    
  }
}

void home(){
  tela.setCursor(0, 0);
  tela.print("Aperte o 1 botao");
  
  tela.setCursor(0, 1);
  tela.print("Para comecar");
}

int inicia(){
  int botao_play = digitalRead(BOTAO_INICIA);
  
  if(botao_play == LOW){
    digitalWrite(BOTAO_INICIA, HIGH);
    return 1;	
  }  
  return 0;
}

void preenche_aleatorio(){
  for(int i =0; i< TAM; i++){
    leds[i] = 0+rand()%2;
    
  
  }
}

void exibe_leds(){
  for(int i = 0; i < TAM; i++)
    {
      if(leds[i] == 0)
      {
        digitalWrite(5, HIGH);
      }
      else
      {
        digitalWrite(4, HIGH);
      }
      delay(1000);
      digitalWrite(LEDS_DIREITO, LOW); 
      digitalWrite(LEDS_ESQUERDO, LOW);
      delay(1000);
    
    }
}







