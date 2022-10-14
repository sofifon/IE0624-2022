#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

// Variables globales
int intr_count = 0;
int sec = 0;
int msec = 0;
int boton = 0;
int estado;

//Definicion de estados para FSM
#define CM 0 // Vehiculos avanzando Cars moving
#define CB 1 // Parpadeo vehiculo Cars Blink
#define CS  2 // Vehiculo detenido Cars Stop
#define PM  3 // Peatones avanzando Pedestrian moving
#define PB 4 // parpadeo peatones Pedestrian Blink
#define PS 5 // peatones detenido

struct FSM{
    void (*stateptr)(void);//nombre presente
    unsigned char time; //Tiempo que dura
    unsigned char next[2];//Proximo estado
};

typedef struct FSM semaforo;

int estado; // Se define el estado base o default.

//Definicion de los estados
void CarsMoving(void){
    PORTB = (0<<PB5)|(1<<PB4)|(1<<PB3)|(0<<PB2); //Activa luz verde carros
}
void CarsBlink(void){
  PORTB ^= (1<<PB4); // Parpadea verde carros  |(1<<PB3)
}
void CarsStop(void){
    PORTB = (1<<PB5)|(0<<PB4)|(1<<PB3)|(0<<PB2); //Activa luz roja carros
}
void PedestriansMoving(void){
    PORTB = (1<<PB5)|(0<<PB4)|(0<<PB3)|(1<<PB2); //Activa luz verde peatones
}
void PedestriansBlink(void){
  PORTB ^= (1<<PB2); // Parpadea verde peatones (1<<PB5)|
}
void PedestriansStop(void){
    PORTB = (1<<PB5)|(0<<PB4)|(1<<PB3)|(0<<PB2); //Activa luz roja peatones
}

// Definicion FSM con sus respectivos tiempos y estados siguientes
semaforo fsm[6] = {
    {&CarsMoving,10,{CM,CB}},
    {&CarsBlink,6,{CS,CS}},
    {&CarsStop,1,{PM,PM}},
    {&PedestriansMoving,10,{PB,PB}},
    {&PedestriansBlink,6,{PS,PS}},
    {&PedestriansStop,1,{CM,CM}}
};


void lights_setup() {//funciones para configurar puertos de semaforo
  GIMSK |= 0x8; // Enables Ports B and C Pin Change Interrupts
  PCMSK1 |= 0b00000001; // PCINT11 - pA0
  DDRB = 0x3C; //Configuracion del puerto
  TCCR0B = 0x5; //para el reloj
  MCUCR |= (1<<ISC11);    // se configura con flanco negativo del reloj
}

void timer_setup(){//funcion para inicializar el timer
  TCCR0A=0x00;   //Se usa el modo normal de operacion del timer
  TCCR0B=0x00;
  TCCR0B |= (1<<CS00)|(1<<CS02);   //Para disminuir la frecuencia
  sei();

  TCNT0 = 0; //InicializarelTimer0
  TIMSK  |= (1 << TOIE0); //activa interrupcion de timer 0


}

void statusChange(){
  switch (estado){
    case CM:
      (fsm[estado].stateptr)(); // Luces verde de carro
      if (sec == fsm[estado].time){ //si pasa el tiempo y se ha presionado el boton
        if (boton){
          estado = fsm[estado].next[boton];
          intr_count = 0; //resetea contadores
          sec = 0;
          msec =0;
        }
        else{
          sec=9;
        }
      }
      else{
        estado = CM; // mantiene el estado hasta que el boton se presiona
      }
      break;

    case CB:
      if (msec == fsm[estado].time){ // despues del tiempo definido pase de estado
        estado = fsm[estado].next[boton];
        intr_count = 0;
        sec = 0;
        msec =0;
      }
      else{
        estado = CB; // si no se cumple la condicion de tiempo siga parpadeando
      }
      break;

    case CS:
      (fsm[estado].stateptr)();
      if (sec == fsm[estado].time){ // si pasaron 2 segundos pase de estado
        estado = fsm[estado].next[boton];
        intr_count = 0;
        sec = 0;
        msec =0;
      }
      break;

    case PM:
      (fsm[estado].stateptr)();
      if (sec == fsm[estado].time){
        estado = fsm[estado].next[boton];
        intr_count = 0;
        sec = 0;
        msec =0;
      }
      break;

    case PB:
      if (msec == fsm[estado].time){
        estado = fsm[estado].next[boton];
        intr_count = 0;
        sec = 0;
        msec =0;
        boton = 0; // desactiva el boton
      }
      else{
        estado = PB;
      }
      break;

      case PS:
        (fsm[estado].stateptr)();
        if (sec == fsm[estado].time){ // si pasaron 2 segundos pase de estado
          estado = fsm[estado].next[boton];
          intr_count = 0;
          sec = 0;
          msec =0;
        }
        break;

    default:
      break;
  }
}

int main(void)
{

  lights_setup();
  timer_setup();
  estado = CM;

  while (1) {
    statusChange();
  }
}

ISR (TIMER0_OVF_vect){      //Interrupcion por contador
  if (intr_count == 20){  //cuenta medio segundo para los parpadeps
    if( estado == CB ){
      (fsm[CB].stateptr)();
    }
    else if( estado == PB){
      (fsm[PB].stateptr)();
    }
    ++msec; //contador de medios de segundo
  }
  if (intr_count == 60){    // cuenta un segundo
    intr_count = 0;
    ++sec; // cuenta un segundo
  }
  else intr_count++;
}

ISR (PCINT1_vect){        // Interrupcion por boton
  boton = 1;
}
