/*      Author:Mehar Singh
 *       lab:
 *  Partner(s) Name: 
 *      Lab Section:
 *      Assignment: Lab #6  Exercise #1
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
volatile unsigned char TimerFlag = 0;
void TimerISR() { TimerFlag = 1;}
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
void TimerOn() {
        TCCR1B = 0x0B;
        OCR1A = 125;
        TIMSK1 = 0x02;
        TCNT1 = 0;
        _avr_timer_cntcurr = _avr_timer_M;
        SREG |= 0x80;
}
void TimerOff() {
        TCCR1B = 0x00;
}
ISR(TIMER1_COMPA_vect) {
        _avr_timer_cntcurr--;
        if (_avr_timer_cntcurr == 0) {
                TimerISR();
                _avr_timer_cntcurr = _avr_timer_M;
        }
}
void TimerSet (unsigned long M) {
        _avr_timer_M = M;
        _avr_timer_cntcurr = _avr_timer_M;
}
enum state {initial,firstled,secondled,thirdled,pausegame} SM;
unsigned char ledtracker =0;
unsigned char sequenceorder =0;
void Tick(){
        switch(SM){
        case initial:
            SM=firstled;
            break;

         case firstled:
            if((~PINA&0x01)==0x01){
            SM=pausegame;
            }
            else{
                 SM=secondled;
            }
            break;

          case secondled:
             if((~PINA&0x01)==0x01){
               SM=pausegame;
             }
            else if(sequenceorder==1){
              sequenceorder=0;
              SM=firstled;
            }
            else{
              SM=thirdled;
            }
              break;

              case thirdled:
               if((~PINA&0x01)==0x01){
                 SM=pausegame;
               }
              else{
                sequenceorder=1;
                SM=secondled;
              }
              break;

              case pausegame:
              if((~PINA&0x01)!=0x01){
                SM=initial;
                if(ledtracker==0x01){
                                                                                                                                                                                          81,13         43%
 PORTB=0X01;
                }
                else if(ledtracker==0x02){
                  PORTB=0x02;
                }
                else{
                  PORTB=0x04;
                }
              }
              break;

              default:
                SM=initial;
            }
              switch(SM){
                case initial:
                  PORTB=0x00;
                  break;
                case firstled:
                  PORTB=0x01;
                  ledtracker=0x01;
                  break;
                case secondled:
                  PORTB=0x02;
                  ledtracker=0x02;
                  break;
                case thirdled:
                  PORTB=0x04;
                  ledtracker=0x04;
                  break;
                case pausegame:
                  break;
                default:
                  break;
              }
            }
void main() {
        DDRA=0x00;
  PORTA=0xFF;
        DDRB=0xFF;
  PORTB=0x00;
        TimerSet(300);
        TimerOn();
  while(1) {
                Tick();
                while (!TimerFlag);
                TimerFlag = 0;
        }
}

                                                  

