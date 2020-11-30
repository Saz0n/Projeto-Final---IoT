
/* Botão 0: Alarme de 2 sensores
   Botão 1: Alarme simples
   Botão 2: Portas automáticas
   Devido a falta de velocidade do tinkercad clicar mais de 1 vez
   para o programa funcionar*/

/* Biblicotecas adicionais*/
#include <IRremote.h>
#include<Servo.h>   

/* Definições*/
#define movimento1 A1
#define movimento2 A2
#define movimento3 1
#define buzzer 4
/* Ativação do servo*/
Servo m1;

int led[7]={9,10,A4,A5,6,8,7};
int letras[6][7]=
  {
  {0,0,0,1,0,0,0},
  {1,1,1,0,0,0,1},
  {0,0,0,1,0,0,0},
  {0,0,1,1,0,0,1},
  {0,1,0,1,0,1,1}, 
  {0,1,1,0,0,0,0},    
};
int on[3][7]=
 {
 {0,0,0,0,0,0,1},
 {0,0,0,1,0,0,1},
 {0,1,1,1,0,0,0},
};



int const controle = A0;
int estado1=0;
int estado2=0;
int estado3=0;
int estado4=0;
int estado5=0;
int estado6=0;

int ligado1=0;
int ligado2=0;
int ligado3=0;
int ligado4=0;

int oi=0;
int z=0;
int contador=0;
int contador2=0;

const int trig = 3;
const int echo = 2;
double distancia;
long tempof;
long tempo_us;
long tempoi;

long int codTecla0 = 16593103;
long int codTecla1 = 16582903;
long int codTecla2 = 16615543;
long int codTecla3 = 16599223;

/* Ativa e decodifica o controle*/
IRrecv receptor(controle);
decode_results valorSaida;

/*Entradas e saidas*/
void setup()
{
  pinMode(movimento1, INPUT);
  pinMode(movimento2, INPUT);
  pinMode(movimento3, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  m1.attach(5);
  receptor.enableIRIn();
  Serial.begin(9600);
  for(int i=0;i<7;i++)
 {
   pinMode(led[i],OUTPUT);
 }
  
}
/* Detecção e tradução do controle*/
void loop()
{
  if ((receptor.decode(&valorSaida))||(oi==HIGH))
  {
    	oi=1;
    
        /* Leitura dos sensores*/
    	if(ligado1==HIGH)
    	{
   			estado1=digitalRead(movimento1);
  			estado2=digitalRead(movimento2);
    		if(estado1==HIGH)
    		{
      			estado3=HIGH;
    		}
    		if(estado2==HIGH)
    		{
     			estado4=HIGH;
    		}
    	}
    	if(ligado2==HIGH)
        {
          estado5=digitalRead(movimento3);
          if(estado5==HIGH)
          {
            estado6=HIGH;
          }
        }
    	if(ligado3==HIGH)
        {
        Serial.println("oi");
      	digitalWrite(trig,HIGH);
		delayMicroseconds(10);
		digitalWrite(trig,LOW);
		while(digitalRead(echo)==0);  
		tempoi = micros();
		while(digitalRead(echo)==1);
		tempof = micros();
		tempo_us = tempof-tempoi;
		distancia=(0.5*0.034029*tempo_us);
        	if(distancia<=200)
        	{
              m1.write(0);
              delay(2500);
        	}
          	else
            {
              m1.write(90);
            }
        }
        /*Comparação de teclas*/
    	if(valorSaida.value == codTecla0)
    	{
          	ligado1=1;
    	}
    	if(valorSaida.value == codTecla1)
    	{		
          	ligado2=1;
    	}
    	if(valorSaida.value == codTecla2)
    	{	
          	ligado3=1;
    	}
        /* Ligar o display*/
        if((ligado1==HIGH)||(ligado2==HIGH))
          if(contador2==LOW)
        	{
         	 z=0;
          	while(z<6)
          	{
          		for(int i=0;i<7;i++)
    			{
      				digitalWrite(led[i],letras[z][i]);
   				}
          		z++;
           	 	delay(400);
          	}
          	z=0;
          	while(z<2)
          	{
            	for(int i=0;i<7;i++)
    			{
      				digitalWrite(led[i],on[z][i]);
   				}
          		z++;
            	delay(400);
            	contador2=1;
          	}
        }
        /*Desligamento do sistema*/
    	if(valorSaida.value==16580863)
    	{
      		ligado1=0;
          	ligado2=0;
          	ligado3=0;
          	estado3=0;
            estado4=0;
          	estado5=0;
            estado6=0;
          	contador2=0;
          	digitalWrite(buzzer,LOW);
          	z=0;
          	while(z<6)
          	{
          		for(int i=0;i<7;i++)
    			{
      				digitalWrite(led[i],letras[z][i]);
   				}
          		z++;
           	 	delay(400);
          	}
          	z=0;
          	contador=0;
          	while(contador<3)
          	{
            	for(int i=0;i<7;i++)
    			{
      				digitalWrite(led[i],on[z][i]);
   				}
          		z=z+2;
              	contador++;
                if(contador==2)
                  z=3;
            	delay(400);
          	}
          	receptor.resume();
          	m1.write(90);
   		}
        /*Som do piezo*/
    	if((estado3==HIGH)&&(estado4==HIGH)&&(ligado1==HIGH))
    	{
    		digitalWrite(buzzer,HIGH);
          	receptor.resume();
    	}
    	if((estado6==HIGH)&&(ligado2==HIGH))
        {
    		digitalWrite(buzzer,HIGH);
          	receptor.resume();
    	}
  		delay(500);
    	receptor.resume();
   }

}