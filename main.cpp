#include "mbed.h"
#include "C12832.h"

Serial xbee(D1,D0);

C12832 lcd(D11, D13, D12, D7, D10); /* LCD */

Ticker tick;                
InterruptIn sw2(PTC6);      /* Switch interrupts */
InterruptIn sw3(PTA4);

static volatile int sw2_trig;       /* Set when switches are pressed */
static volatile int sw3_trig;


void sw2_interrupt (void)
{
    sw2_trig = 1;
}

void sw3_interrupt (void)
{
    sw3_trig = 1;
}

int main (void)
{
    sw2_trig = 0;
    sw3_trig = 0;
    
    sw2.mode(PullUp);
    sw2.fall(&sw2_interrupt);
    
    sw3.mode(PullUp);
    sw3.fall(&sw3_interrupt);
    
    lcd.cls();
    lcd.locate(1,1);
    
    char send = 'a';
    char receive = 'b';
    
    while(true)
    {
        if(sw2_trig & sw3_trig == 0)
        {
            sleep();
        }
        
        else {
            xbee.printf(&send);
            wait(0.5);
            if(xbee.readable())
            {
                char c = xbee.getc();
                
                if (c == receive)
                {
                    lcd.printf("Please wait...");
                    int waiting = 0;
                    while (waiting < 60)
                    {
                        if(xbee.readable())
                        {
                            char d = xbee.getc();
                            switch (d) {
                                case 'S' : lcd.cls(); lcd.locate(1,1); lcd.printf("Sam is on his way and will be with you shortly"); wait(30);
                                case 'L' : lcd.cls(); lcd.locate(1,1); lcd.printf("Louis is on his way and will be with you shortly"); wait(30);
                                case 'M' : lcd.cls(); lcd.locate(1,1); lcd.printf("Max is on his way and will be with you shortly"); wait(30);
                                case 'J' : lcd.cls(); lcd.locate(1,1); lcd.printf("Jemma is on her way and will be with you shortly"); wait(30);
                                default : lcd.cls(); lcd.locate(1,1); lcd.printf("Please wait, someone will be with you shortly"); wait(30);
                            }    
                        }
                        else { waiting++; wait(1); }
                    }
                    if (waiting == 60) { lcd.cls(); lcd.locate(1,1); lcd.printf("No reply received - please knock"); }
                    waiting = 0;
                }
                
                else if(c == 'n') { lcd.cls(); lcd.locate(1,1); lcd.printf("No connection - please knock"); }
            }
            else { lcd.cls(); lcd.locate(1,1); lcd.printf("No connection - please knock"); }
            
            sw2_trig = 0;
            sw3_trig = 0;
            lcd.cls();
         }   
    }
}