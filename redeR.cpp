#include "redeR.h"

#define ADD_SIZE 5

byte NETWORK_ADD[ADD_SIZE];
byte ID;

char RECEIVE_BUFFER[RF_PAYLOAD];

RF24 nrf24(RADIO_CE,RADIO_CSN);


//---------------------------------------------------------------------------
// Construção da tabela roteamento (Função usada apenas na base)
void table_construction(char *id_list){
  // Pingar todos os modulos
  // Analisar qualidade, escolher modulos da primeira camada
  // Enviar para todos modulos da primeira camada o id_list dos restantes
  // Receber a qualidade dos modulos da primeira camada e escolher segunda 
  // camada analisar possibilidade de mais de um caminho
  
}
//---------------------------------------------------------------------------
void network_init(char *network_add, char id){

  // Copia endereço de MAC e ID do modulo para variavel local, possivel ponto 
  // para gravação a EEPROM
  memcpy(NETWORK_ADD, network_add, ADD_SIZE);

  ID=id;

  // Inicializa radio
  nrf24.begin();

  
  nrf24.setChannel(NETWORK_CH);

  NETWORK_ADD[4]=ID;
  
  nrf24.openReadingPipe(0,NETWORK_ADD);
	
	nrf24.starListening();
}

//---------------------------------------------------------------------------
// Envia dados para modulo X e aguarda Echo retorna verdadeiro ou falso
bool send_data(char *data,char id=0){
  #define TRIALS 2
  // Variavel auxiliar que conterá o caminho até o modulo id
  char path[LEVELS];

  // Varialvel auxiliar de buffer
  char buf[RF_PAYLOAD];

  // Loop para multiplas tentativas de entrega
  for(int i=1;i<=TRIALS;i++){
      
    // Buscar o caminho para envio
    get_path(id, path);

    // Modifica endereço de envio para primeiro modulo da sequencia
    NETWORK_ADD[4] = path[0];
    nrf24.openWritingPipe(NETWORK_ADD);
  
    // Empacotar
    data_packing(path,data,buf);
    
    // Emviar e testar conexão com primeiro nivel
    if(radio_send(buf)){
      return true;
    }
  }
  #ifdef NETWORK_ROOT
    // gerar erro para tabela de roteamento
  #endif
  return false;
}

//---------------------------------------------------------------------------
// Verifica se os dados recebidos são para o modulo, caso contrario retransmite 
// dados para proximo nivel, deve ser chamada após teste de available
bool chek_and_foward(){
  char buf[RF_PAYLOAD];
  
  nrf24.read(buf,RF_PAYLOAD);
  
  for(int i=1;i<LEVELS;i++){
    if(buf[i]==0){// Informação descendo
      if(i==1){
        // o dado é para o modulo em questão, logo deve ser chamado a funçao receive_data
        buf[i]=1;// modifica path para que seja feito ACK
        
        //Copia dados para buffer de recebimento global
        memcpy(RECEIVE_BUFFER,buf,RF_PAYLOAD);
        // Retorna que o dado foi recebido com sucesso
        rede_ack(buf,true);
        return true;
      }
      else {
        // Caso contrario envia para próximo modulo
  
        // Modifica path dentro do buffer
        shift_left_path(buf);

        // Modifica endereço de envio para primeiro modulo da sequencia
        NETWORK_ADD[4] = buf[0];
        nrf24.openWritingPipe(NETWORK_ADD);
  
        // Envia e Testa ACK do radio
        if(!radio_send(buf)){
          shift_right_path(buf);
          buf[i]=1;
          rede_ack(buf,false);
        }
      }
    }
    else if(buf[i]==1){// Informação subindo
        // Modifica path dentro do buffer
        shift_right_path(buf);

        // Modifica endereço de envio para primeiro modulo da sequencia
        NETWORK_ADD[4] = buf[0];
        nrf24.openWritingPipe(NETWORK_ADD);
  
        // Envia
        radio_send(buf);
    }
  }
}

//---------------------------------------------------------------------------
// Retorna o id do modulo e os dados do modulo que o enviou
char receive_data(char *data){
  
}

//---------------------------------------------------------------------------
// Obtem caminho para modulo de endereço "id"
void get_path(char id, char *path){
  #ifdef NETWORK_ROOT
    // codigo para base
    // baseado no valor de way escolha uma rota diferente
	
	if (id==10){
		path[0]=10;
		path[1]=0;
		path[2]=0;
	}
	if (id==11){
		path[0]=10;
		path[1]=11;
		path[2]=0;
	}
	
	
  #else 
    //codigo para modulo
    memcpy(path,RECEIVE_BUFFER,LEVELS);
    shift_right_path(path);
    
  #endif
}

//---------------------------------------------------------------------------
// Empacotador de dados constroi o buffer para enviou cuida das criptografias
void data_packing(char *path,char *data_in, char *data_out){
  // Função deve criptografar data_in e chave

  // Função simples para teste, deve ser alterada
  memcpy(data_out,path,LEVELS);
  memcpy(&data_out[LEVELS+1],data_in,4);
  
  
}

//---------------------------------------------------------------------------
// Desempacotador de dados
void data_unpackig(char *data_in, char *data_out){
  
}

//---------------------------------------------------------------------------
// Teste de comunicação de todos os modulos para construção da tabela
void ping_modules(char *id_list, char *quality){
  
}

//---------------------------------------------------------------------------
bool radio_send(char *data){
  bool success;
  nrf24.stopListening();
  success=nrf24.write(data,RF_PAYLOAD);
  nrf24.startListening();

  return success;
}

//---------------------------------------------------------------------------
void rede_ack(char *path, bool success){
  
}

//---------------------------------------------------------------------------
void shift_right_path(char *data){
  char aux=data[LEVELS-1];
  for(int i=LEVELS-2;i>0;i--){
    data[i+1]=data[i];
  }
  data[0]=aux;
}

//---------------------------------------------------------------------------
void shift_left_path(char *data){
  char aux=data[0];
  for(int i=1;i<LEVELS;i++){
    data[i-1]=data[i];
  }
  data[LEVELS-1]=aux;
}

