//#define NETWORK_ROOT

#include <redeR.h>

char network_address[]={'U','U','U','U',0 };
char id=11;
int LED = 3;



void setup() {
  pinMode(LED, OUTPUT);
  
  network_init(network_address, id);
}


void loop() {
char buffer2[]={0, 0};

  if (check_and_foward()){
    receive_data(buffer2);
    analogWrite(LED,buffer2[1]);
  }
}
