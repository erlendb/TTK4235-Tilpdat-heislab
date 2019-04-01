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
  int elev_state; //Starter i go-state for å kjøre heisen til kjent starttilstand

  //Etasje
  int floor;

  //Timer
  time_t doorTimer = -1;

  //Kø
  queue_clear();

  //Kjører heisen til kjent plassering.
  //OBS! Heisen fullfører ikke bestillingen dersom den initialiseres i første eller siste etasje. Queue_stop(3) og queue_stop(0) returnerer nemlig alltid 1.
  queue_update(0, ORDER_ALL); //første etasje, alle retninger
  set_direction(DIRN_DOWN);
  elev_state = go;

	while(1) {

    //Kikker litt på køa
    //queue_print();



    //Greier som skal skje for hver iterasjon, uavhengig av state, utføres her

    //Ser etter stoppsignal
    int stop_signal = elev_get_stop_signal();
		if (stop_signal) {
    	elev_state = stop;
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
    switch(elev_state) {
    	case idle:
        //Idle-state skal ikke gjøre noe som helst. Venter på ny bestilling.
    		//printf("idle");

    		//Gå til go-state hvis det har kommet ny bestilling
        if (queue_count()) {
          elev_state = go;
        }

		    break;

    	case go:
        set_direction(direction);
        //Go-state skal kjøre heisen i retning av en bestilling. Skal stoppe hvis vi passerer etasjer med bestillinger i samme retning.
    		//printf("go");

        //Oppdaterer floor
		    floor = elev_get_floor_sensor_signal();

        //Hvis vi har havnet i en etasje:
        if (floor != -1) {

          //Oppdaterer lastFloor
		    	lastFloor = floor;

          //Sjekker om vi har en bestilling (aka om vi skal stoppe). Går eventuelt til stay-state.
          //printf("floor != -1");
          queue_print();
          printf("qstop %d %d %d %d", queue_stop(0), queue_stop(1), queue_stop(2), queue_stop(3));
          if (queue_stop(floor)) {
            elev_state = stay;
            //printf("stay");
          } else {
            //Bestemme retning på heisen
            //Snu retningen hvis vi er øverst eller nederst
            if (floor == 0) {
              set_direction(DIRN_UP);
            } else if (floor == N_FLOORS-1) {
              set_direction(DIRN_DOWN);
            }
            //Snu retningen hvis det ikke er flere bestillinger lenger fram i kjøreretningen
            printf("direction %d \n", direction);
            if (direction == DIRN_UP && !queue_check_above(floor)) set_direction(DIRN_DOWN);
            if (direction == DIRN_DOWN && !queue_check_below(floor)) set_direction(DIRN_UP);
          }

          //Oppdaterer etasjelys
        	elev_set_floor_indicator(floor);

		    } else {
          //Hvis vi er mellom to etasjer:
          //printf("floor == -1\n");

          //Fortsett i samme retning
        }

		    break;

    	case stay:
        //Stay-state skal passe på alt som skjer i en etasje: åpne/lukke dør, obstruksjon...
    		//printf("stay");

        //Åpne døra og start timeren i det vi kommer inn i stay-state
    		if (doorTimer == -1) {
	        set_direction(DIRN_STOP);
	        door_open();
	        doorTimer = time(NULL);
		    }
        //Lukk døra og resett timeren etter 3 sekunder. Fjerner denne etasjen fra køen. Går til idle etterpå.
        if (time(NULL) - doorTimer > 3) {
        	door_close();
        	doorTimer = -1;
        	queue_update(floor,-1);
        	elev_state = idle;
        }

		    break;

    	case stop:
        //Stop-state aktiveres ved nødstopp. Her skal køen slettes og nye bestillinger skal ikke tas hensyn til.
    		printf("stop");

        //Tømmer køen, stopper motor.
        queue_clear();
        set_direction(DIRN_STOP);

        //Går til idle-state når nødstopp er deaktivert.
        if (!stop_signal) elev_state=idle;

		    break;
    }
	}

  //Evig løkke
  while(1) {};

/*
    elev_set_motor_direction(DIRN_UP);



    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }

    */

    return 0;
}
