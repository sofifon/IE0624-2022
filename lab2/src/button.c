#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

volatile int value = 0;

int main(void)
{

  GIMSK |= 0x8; // Enables Ports B and C Pin Change Interrupts
  PCMSK1 |= 0b00000001; // PCINT11
  DDRB = 0x3C; //Configuracion del puerto

  
  sei();
  while (1) {
    PORTB = 0x4;
  }
}

ISR(PCINT1_vect)
{
  PORTB = 0x3C;
  _delay_ms(500);
}
