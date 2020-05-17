#include "mbed.h"

DigitalOut redLED(LED1);
Serial pc(USBTX, USBRX);
Serial xbee(D12, D11);

void reply_messange(char *xbee_reply, char *messange){
  xbee_reply[0] = xbee.getc();
  xbee_reply[1] = xbee.getc();
  xbee_reply[2] = xbee.getc();
  if(xbee_reply[1] == 'O' && xbee_reply[2] == 'K'){
   pc.printf("%s\r\n", messange);
   xbee_reply[0] = '\0';
   xbee_reply[1] = '\0';
   xbee_reply[2] = '\0';
  }
}

int main(){
  pc.baud(9600);

  char xbee_reply[3];

  // XBee setting
  xbee.baud(9600);
  xbee.printf("+++");
  xbee_reply[0] = xbee.getc();
  xbee_reply[1] = xbee.getc();
  if(xbee_reply[0] == 'O' && xbee_reply[1] == 'K'){
   pc.printf("enter AT mode.\r\n");
   xbee_reply[0] = '\0';
   xbee_reply[1] = '\0';
  }

  // SLAVE_MY hould be different between Slave A and Slave B
  xbee.printf("ATMY 0x164\r\n");
  reply_messange(xbee_reply, "setting MY : 0x164");

  xbee.printf("ATDL 0x0\r\n");
  reply_messange(xbee_reply, "setting DL : 0x0");

  xbee.printf("ATID 0x6465\r\n");
  reply_messange(xbee_reply, "setting PAN ID : 0x6465");

  xbee.printf("ATWR\r\n");
  reply_messange(xbee_reply, "write config");

  xbee.printf("ATCN\r\n");
  reply_messange(xbee_reply, "exit AT mode");
  xbee.getc();

  // start
  pc.printf("start\r\n");
  char receive_data = 0;
  redLED = 1;

  while(1){
    receive_data = xbee.getc();
    pc.printf("Receive data %c\r\n", receive_data);
    if (receive_data == 'a') {
      redLED = 0;
      xbee.printf("A received the data.");
    }
    wait(0.5);
    redLED = 1;
    receive_data = 0;
  }
}