/*
timer_button_key
 */

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char mask = 0;

void TIMER1_OVF_vect()
{
    //PORTC = ~((~PORTC)>>1);
    mask >>= 1;
    PORTC = ~mask;
    reti();
}
void INT0_vect()
{
    static char isButtonDown = 0;
    EIMSK &= (~(1<<INT0));
    if(!isButtonDown)
    {
        isButtonDown = 1;
        EICRA &= (~(1<<ISC00));
        PORTA |= 1;
        mask = 0xff;
        TCCR1B |= (1<<CS10)|(1<<CS11);
    }
    else
    {
        isButtonDown = 0;
        EICRA |= (1<<ISC00);
        PORTA &= (~(1));
        TCCR1B &= (~((1<<CS10)|(1<<CS11)));
        PORTC = 0;
    }
    EIMSK |=(1<<INT0);

    reti();
}

int main(void)
{
    DDRA = 0x01;
    PORTA = 0;
    DDRC = 0xff;
    PORTC = 0;
    //timer1
    TCCR1A = 0;
    TCCR1C = 0;
    ICR1 = 0xffff;//установка максимум счетчика TCNT1
    TCNT1 = 0;//установка начаоьного значения счетчика
    TCCR1B = 0;//установка старших бит режима счетчика и установка источника тактового сигнала
    TIMSK |= (1<<TOIE1);//разрешение прирывания по переполнению таймера1

    //внешнее прерывание 0
    EICRA |= (1<<ISC01) | (1<<ISC00);
    EIMSK |=(1<<INT0);
    EIFR |= (1<<INTF0);
    sei();
    //TCCR1B |= (1<<CS10);
    while(1);

    return 0;
}
