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
enum state {initial, firstSwitch,switchLed} SM;
void Tick(){
        switch(SM){
                case initial:
                        SM=firstSwitch;
                        break;

                case firstSwitch:
                        SM=switchLed;
break;
                case switchLed:
                        SM=initial;
                        break;
        }
        switch(SM){
                case initial:
                        break;
                case firstSwitch:
                        PORTB=0x01;
                        break;
                case switchLed:
                        if(PORTB==0x04){
                                PORTB=0x01;
                        }
                        else{
                                PORTB=PORTB<<1;
                        }
                        break;
                default:
                        break;
        }
}
void main() {
 DDRB = 0xFF;
 PORTB = 0x00;
 TimerSet(1000);
 TimerOn();
 while (1) {
        Tick();
        while(!TimerFlag) {};
        TimerFlag = 0;
    }
}                                                                                                                                                                                    1,1           Top
