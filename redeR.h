#ifndef REDER_H
#define REDER_H

#include "RF24.h"

#define LEVELS 6 // 5 niveis mais 0 de fim


// Configuração do Radio ----------------------------------------------------
#define RF_PAYLOAD 32 // payload usado no NRF24L01
#define RADIO_CE 9
#define RADIO_CSN 10

#define NETWORK_CH 120


#ifdef NETWORK_ROOT
// Construção da tabela roteamento (Função usada apenas na base)
void table_construction(char *id_list);

#else

// Verifica se os dados recebidos são para o modulo, caso contrario retransmite 
// dados para proximo nivel, deve ser chamada após teste de available
bool chek_and_foward();

void shift_right_path(char *data);

void shift_left_path(char *data);

#endif

void network_init();

// Envia dados para modulo X e aguarda Echo retorna verdadeiro ou falso
bool send_data(char *data,char id);

// Retorna o id do moulo e os dados do modulo que o enviou
char receive_data(char *data);

// Obtem caminho para modulo de endereço "id"
void get_path(char id, char *path);

// Empacotador de dados constroi o buffer para enviou cuida das 
void data_packing(char *path,char *data_in, char *data_out);

// Desempacotador de dados
void data_unpackig(char *data_in, char *data_out);

// Teste de comunicação de todos os modulos para construção da tabela
void ping_modules(char *id_list, char *quality);

// Envia dados pelo radio
bool radio_send(char *data);

void rede_ack(char *path, bool success);


#endif



