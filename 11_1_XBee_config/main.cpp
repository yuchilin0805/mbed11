#include "mbed.h"

Serial pc(USBTX, USBRX);
Serial xbee(D12, D11);

char str[50];

int main(){
  int i=0;
  pc.baud(9600);
  xbee.baud(9600);
  pc.printf("Begin test\r\n");

  while(1){
    while(pc.readable()){
      char c = pc.getc();
      if(c!='\r' && c!='\n'){
        pc.printf("%c", c);
        xbee.printf("%c", c);
        str[i] = c;
        i++;
        str[i] = '\0';
      }
      else{
        pc.printf("\r\n");
        i=0;
        if (str != "+++"){
          xbee.printf("\r\n");
        }
      }
    }
    while(xbee.readable()){
      char c = xbee.getc();
      if(c!='\r' && c!='\n'){
        pc.printf("%c",c);
      }
      else{
        pc.printf("\r\n");
      }
    }
    wait(0.001);
  }
}