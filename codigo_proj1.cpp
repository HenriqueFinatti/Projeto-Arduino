#include <LiquidCrystal.h>

LiquidCrystal tela(12, 11, 5, 4, 3, 2);

void setup()
{
  pinMode(8,INPUT_PULLUP);
  
  tela.begin(16, 2);
  Serial.begin(9600);
  
}

void loop()
{
  home();
  tela.setCursor(0,0);
  
  if(inicia(8)){
    tela.print("Inicio de jogo");
  }
}