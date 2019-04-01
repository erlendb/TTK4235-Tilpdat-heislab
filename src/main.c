#include <stdio.h>
#include <comedilib.h>
#include <time.h>

#include "driver/elev.h"
#include "door.h"
#include "queue.h"

int main() {

  // Initialize hardware
  if (!elev_init()) {
   printf("Unable to initialize elevator hardware!\n");
    return 1;
  }
  printf("Press STOP button to stop elevator and exit program.\n");

  //Heistilstander
  enum states {idle, go, stay, stop};
  int state; //Starter i go-state for å kjøre heisen til kjent starttilstand

  //Stopptilstander
  enum stops {NOT_STOPPED, STOPPED, STOPPED_BETWEEN};
  int stopped = NOT_STOPPED;

  //Etasje
  int floor;

  //Timer
  time_t doorTimer = -1;

  //Kø
  queue_clear();

  //Kjører heisen til kjent tilstand.
  queue_update(0, ORDER_ALL); //første etasje, alle retninger
  set_direction(DIRN_DOWN);
  state = go;

	while(1) {
    //Greier som skal skje for hver iterasjon, uavhengig av state, utføres her

    //Ser etter stoppsignal. Går til stop-state hvis nødstopp aktiveres.
		if (elev_get_stop_signal()) {
    	state = stop;
		}

		//Ser etter bestillinger. Iterasjon gjennom alle knapper og queue_update()
    for (int f = 0; f < N_FLOORS; f++) {
    if (queue[f] != ORDER_ALL) {
      if (elev_get_button_signal(BUTTON_COMMAND,f)
      || (elev_get_button_signal(BUTTON_CALL_UP, f) && elev_get_button_signal(BUTTON_CALL_DOWN, f))
      || (elev_get_button_signal(BUTTON_CALL_UP, f) && queue[f] == ORDER_DOWN)
      || (elev_get_button_signal(BUTTON_CALL_DOWN, f) && queue[f] == ORDER_UP)
      ) {
        queue_update(f,ORDER_ALL);
      }
      else if (elev_get_button_signal(BUTTON_CALL_UP,f)) queue[f] = ORDER_UP;
      else if (elev_get_button_signal(BUTTON_CALL_DOWN,f)) queue[f] = ORDER_DOWN;
      }
    }

    //Tilstander: idle, go, stay, stop
    switch(state) {
    	case idle:
        //Idle-state skal ikke gjøre noe som helst. Sender rett videre til go-state hvis det finnes noe i køen. Alternativt venter på ny bestilling.

    		//Gå til go-state hvis det finnes bestilling i køen
        if (queue_count()) {
          state = go;
        }

		    break;

    	case go:
        //Sender heisen i riktig retning ut fra idle-state og stop-state
        printf("Stopped %d",stopped);
        if (stopped == STOPPED_BETWEEN) {
          //Vill algoritme som finner riktig retning for stuck heis:
          if (queue_check_above(lastFloor-(direction==DIRN_DOWN))) set_direction(DIRN_UP);
          else set_direction(DIRN_DOWN);
          //Resetter stoppflagg
          stopped = NOT_STOPPED;

        } else {
          set_direction(direction);
        }
        //Go-state skal kjøre heisen i retning av en bestilling. Skal stoppe hvis vi passerer etasjer med bestillinger i samme retning.

        //Oppdaterer etasjevariabel
		    floor = elev_get_floor_sensor_signal();

        //Hvis vi har havnet i en etasje:
        if (floor != -1) {

          //Oppdaterer etasjelys og lastFloor
        	elev_set_floor_indicator(floor);
		    	lastFloor = floor;

          queue_print(); /*
          printf("qstop %d %d %d %d", queue_stop(0), queue_stop(1), queue_stop(2), queue_stop(3));
          printf("Floor) %d\n", floor); */
          //Sjekker om vi har en bestilling (aka om vi skal stoppe). Går eventuelt til stay-state.
          if (queue_stop(floor)) {
            state = stay;
          } else {
            //Bestemme retning på heisen
            //Snu retningen hvis vi er øverst eller nederst
            if (floor == 0) {
              set_direction(DIRN_UP);
            } else if (floor == N_FLOORS-1) {
              set_direction(DIRN_DOWN);
            }
            //Snu retningen hvis det ikke er flere bestillinger lenger fram i kjøreretningen
            if (direction == DIRN_UP && !queue_check_above(floor)) set_direction(DIRN_DOWN);
            if (direction == DIRN_DOWN && !queue_check_below(floor)) set_direction(DIRN_UP);
          }

		    } else {
          //Hvis vi er mellom to etasjer skal heisen bare surre videre i samme retning :)
        }

		    break;

    	case stay:
        //Stay-state skal passe på alt som skjer i en etasje: åpne/lukke dør, obstruksjon...

        //Åpne døra og start timeren i det vi kommer inn i stay-state
    		if (doorTimer == -1) {
	        set_direction(DIRN_STOP);
	        door_open();
	        doorTimer = time(NULL);
		    }
        /*
        //Obstruksjon
        else {
          if (elev_get_obstruction_signal()) doorTimer = time(NULL);
        }
        */
        //Lukk døra og resett timeren etter 3 sekunder. Fjerner denne etasjen fra køen. Går til idle etterpå.
        if (time(NULL) - doorTimer > 3) {
        	door_close();
        	doorTimer = -1;
        	queue_update(floor,-1);
        	state = idle;
        }

		    break;

    	case stop:
        //Stop-state aktiveres ved nødstopp. Her skal køen slettes og nye bestillinger skal ikke tas hensyn til.

        //Stopper motor, setter stopplampa, resetter dørtimer og evt. åpner døra når vi går inn i nødstopp
        if (!stopped) {
          set_direction(DIRN_STOP);
          elev_set_stop_lamp(1);
          doorTimer = -1;

          //Åpner døra hvis vi er i en etasje
          if (floor != -1) door_open();
        }
        //Setter stoppflagg
        stopped = STOPPED;

        //Tømmer køen og hindrer nye Bestillinger
        queue_clear();

        //Nødstopp deaktivert: skru av lampe, lukk døra og gå til idle
        if (!elev_get_stop_signal()) {
          elev_set_stop_lamp(0);
          door_close();
          //Endrer stoppflagget hvis heisen står mellom to etasjer så det kan brukes av go-state
          if (floor == -1) stopped = STOPPED_BETWEEN;
          //Fjerner stoppflagget hvis vi står i en etasje
          else stopped = NOT_STOPPED;
          state = idle;
        }

		    break;
    }
	}

  return 0;
}
