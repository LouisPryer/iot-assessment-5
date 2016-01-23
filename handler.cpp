#include "mbed.h"
#include "C12832.h"

DigitalOut redLed (LED1);
PwmOut buzzer (D6);

C12832 display (D11, D13, D12, D7, D10);
Serial host (USBTX, USBRX);
Serial xbee (D1, D0);

int main (void) {
  host.baud(38400);
  bool xbeeWaiting = true;
  bool hostWaiting = false;
  for(;;) {
    while(xbeeWaiting) {
      wait(0.5);
      if(xbee.readable()) {
        xbeeWaiting = false;
      }
    }
    xbee.putc('A');
    host.putc('B');
    hostWaiting = true;
    int timer = 0;
    while(hostWaiting) {
      redLed = 0;
      buzzer.period(1.0);
      buzzer=0.5;
      if(timer >= 60) hostWaiting = false;
      wait(1);
      buzzer=0.0;
      redLed = 1;
      timer++;
      if(host.readable()) hostWaiting = false;
    }
    redLed = 1;
    buzzer=0.0;
    if(timer < 61) {
      char name = host.getc();
      xbee.putc(name);
    }
    xbeeWaiting = true;
  }
}
