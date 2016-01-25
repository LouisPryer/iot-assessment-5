#include "mbed.h"
#include "C12832.h"

DigitalOut redLed (LED1);
PwmOut buzzer (D6);

C12832 display (D11, D13, D12, D7, D10);
Serial host (USBTX, USBRX);
Serial xbee (D1, D0);

int main (void) {
  redLed = 1;
  host.baud(38400);
  bool xbeeWaiting = true;
  bool hostWaiting = false;
  for(;;) {
    while(xbeeWaiting) { //Will wait for a response from the doorbell device
      wait(0.5f);
      if(xbee.readable()) {
        xbeeWaiting = false;
      }
    }
    xbee.printf('A');
    host.printf('B');
    hostWaiting = true;
    int timer = 0;
    while(hostWaiting) { //Will wait 60 seconds for a reply from facebook, if no response it will wait for another door bell to be rung
      redLed = 0;
      buzzer.period(1.0);
      buzzer=0.5;
      wait(0.5f);
      if(timer >= 60) hostWaiting = false;
      buzzer=0.0;
      redLed = 1;
      wait(0.5f); //Seperating waits into 0.5 second gaps emulates the buzzer and LED to turn off and on, as an indicator someone is at the door
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
