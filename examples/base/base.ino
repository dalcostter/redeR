#define NETWORK_ROOT

#include <redeR.h>

char network_address[]={'U','U','U','U',0 };
char id=0;
int POT = A0;
int BUTTON = 2;

char id_modulo = 10;


void setup() {
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
  network_init(network_address, id);
}

void loop() {

int leitura_pot = analogRead(POT);

if (digitalRead(botao)==HIGH){
  if (id_modulo==10){
    id_modulo=11;
  }
  else{
    id_modulo=10;
  }
  delay(200); //para acalmar o loop e nao trocar o valor 10 vezes por apertada.
}

send_data(char(leitura_pot),id_modulo);


}
