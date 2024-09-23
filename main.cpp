#include <LiquidCrystal.h>
#include <stdlib.h>
LiquidCrystal tela(13, 12, 11,10, 9, 8);

const int TAM = 10;
int leds[TAM];

const int BOTAO_INICIA = 3;
const int BOTAO_ESQUERDO = 7;
const int BOTAO_DIREITO = 6;

const int LEDS_ESQUERDO = 5;
const int LEDS_DIREITO = 4;

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
