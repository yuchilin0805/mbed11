#include "mbed.h"
#include "mbed_rpc.h"

Serial pc(USBTX, USBRX);
Serial xbee(D12, D11);

RpcDigitalOut myled1(LED1,"myled1");
RpcDigitalOut myled2(LED2,"myled2");
RpcDigitalOut myled3(LED3,"myled3");

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

void check_addr(char *xbee_reply, char *messenger){
  xbee_reply[0] = xbee.getc();
  xbee_reply[1] = xbee.getc();
  xbee_reply[2] = xbee.getc();
  xbee_reply[3] = xbee.getc();
  pc.printf("%s = %c%c%c\r\n", messenger, xbee_reply[1], xbee_reply[2], xbee_reply[3]);
  xbee_reply[0] = '\0';
  xbee_reply[1] = '\0';
  xbee_reply[2] = '\0';
  xbee_reply[3] = '\0';
}

int main(){
  int i=0;
  pc.baud(9600);

  char xbee_reply[4];

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
  xbee.printf("ATMY 0x264\r\n");
  reply_messange(xbee_reply, "setting MY : 0x264");

  xbee.printf("ATDL 0x164\r\n");
  reply_messange(xbee_reply, "setting DL : 0x164");

  xbee.printf("ATWR\r\n");
  reply_messange(xbee_reply, "write config");

  xbee.printf("ATMY\r\n");
  check_addr(xbee_reply, "MY");

  xbee.printf("ATDL\r\n");
  check_addr(xbee_reply, "DL");

  xbee.printf("ATCN\r\n");
  reply_messange(xbee_reply, "exit AT mode");
  xbee.getc();

  // start
  pc.printf("start\r\n");
  char buf[100] = {0};
  char outbuf[100] = {0};

  xbee.getc(); //remove the first redundant char
  while(1){
    for (i=0; ; i++) {
      char recv = xbee.getc();
      if (recv == '\r') {
        break;
      }

      buf[i] = pc.putc(recv);
    }

    RPC::call(buf, outbuf);
    pc.printf("%s\r\n", outbuf);
  }
}