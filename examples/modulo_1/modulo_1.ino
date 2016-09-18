//#define NETWORK_ROOT

#include "\\..\\..\\redeR.h"

char network_address[]={'U','U','U','U',0 };
char id=10;
int LED = 3;



void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  network_init(network_address, id);
}


void loop() {
char buffer2[]={0, 0};

  if (check_and_foward()){
    receive_data(buffer2);
    analogWrite(LED,int(buffer2[1]));
  }
}
