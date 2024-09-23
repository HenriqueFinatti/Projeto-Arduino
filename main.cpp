#include <LiquidCrystal.h>
#include <stdlib.h>
LiquidCrystal tela(13, 12, 11,10, 9, 8);

const int TAM = 5;

const int BOTAO_INICIA = 3;
const int BOTAO_ESQUERDO = 7;
const int BOTAO_DIREITO = 6;

const int LEDS_ESQUERDO = 5;
const int LEDS_DIREITO = 4;

const int BUZZER = 2;

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
    
    tela.clear();
    tela.setCursor(0,0);
    tela.print("Sua vez agora.");

    delay(1000);
    
    if(compara())
    {
      tela.clear();
      tela.setCursor(0,0);
      tela.print("Fase 2/3 : Perguntas");

      musica_vitoria();

    }
    else{
      musica_derrota();
    }
    delay(1000);
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

void home(){
  tela.setCursor(0, 0);
  tela.print("Aperte o 1 botao");
  
  tela.setCursor(0, 1);
  tela.print("Para comecar");
}

int inicia(){
  int botao_play = digitalRead(BOTAO_INICIA);
  
  if(botao_play == LOW)
  {
    digitalWrite(BOTAO_INICIA, HIGH);
    return 1;	
  }  
  return 0;
}

void preenche_aleatorio(){
  for(int i =0; i< TAM; i++)
  {
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

int compara (){

  delay(1000);
  Serial.println("entrou no funcao");
  
  int valida[TAM];
  int cont = 0;
  
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
        }
        else
        {
            tela.clear();
            tela.setCursor(0, 0);
            tela.print("Errou");
            return 0;
        }
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
        }
        else
        {
            tela.clear();
            tela.setCursor(0, 0);
            tela.print("Errou");
            return 0;
        }

      }

        delay(100);

        if(cont == TAM)
        {
            tela.clear();
            tela.setCursor(0,0);
            tela.print("CORRETO");
          	delay(1000);
          
            return 1;
        }
    }   
}