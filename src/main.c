#include "elev.h"
#include "door.h"
#include "queue.h"

#include <stdio.h>
#include <comedilib.h>
#include <time.h>

int main() {
  // Initialize hardware
  if (!elev_init()) {
    printf("Unable to initialize elevator hardware!\n");
    return 1;
  }
  printf("Press STOP button to stop elevator and exit program.\n");

  //Heistilstander
  enum state {idle, go, stay, stop};
  int elev_state = go; //Starter i go-state for å kjøre heisen til kjent starttilstand

  //Etasjer
  enum floors {FIRST, SECOND, THIRD, FOURTH};
  int floor;

  //Bestillinger
  enum orders {ORDER_UP, ORDER_DOWN, ORDER_ALL};

  //Timer
  time_t startTime = -1;

  //Kjører heisen til kjent plassering.
  //OBS! Heisen fullfører ikke bestillingen dersom den initialiseres i første eller siste etasje. Queue_stop(3) og queue_stop(0) returnerer nemlig alltid 1.
  queue_update(FIRST, ORDER_ALL); //første etasje, alle retninger
  elev_set_motor_direction(DIRN_DOWN);

	while(1) {
    //Ser etter stoppsignal
    int stop_signal = elev_get_stop_signal();
		if (stop_signal) {
    	elev_state = stop;
		}

		//Ser etter bestillinger


    //Tilstander: idle, go, stay, stop
    switch(elev_state) {
    	case idle:
        //Idle-state skal ikke gjøre noe som helst. Venter på ny bestilling.
    		printf("idle");

    		//Gå til go-state hvis det har kommet ny bestilling

		    break;

    	case go:
        //Go-state skal kjøre i retning av en bestilling. Skal stoppe hvis vi passerer etasjer med bestillinger i samme retning.
    		printf("go");

        //Oppdaterer floor
		    floor = elev_get_floor_sensor_signal();

        //Hvis vi har havnet i en etasje:
        if (floor != -1) {

          //Oppdaterer lastFloor
		    	lastFloor = floor;

          //Sjekker om vi har en bestilling (aka om vi skal stoppe):
          if (queue_stop(floor)) {
            elev_state = stay;
          }

          //Oppdaterer etasjelys
        	elev_set_floor_indicator(floor);

		    } else {
          //Hvis vi er mellom to etasjer:

          //Fortsett i samme retning
        }

		    break;

    	case stay:
        //Stay-state skal passe på alt som skjer i en etasje: åpne/lukke dør, obstruksjon...
    		//printf("stay");

        //Åpne døra og start timeren i det vi kommer inn i stay-state
    		if (startTime == -1) {
	        elev_set_motor_direction(DIRN_STOP);
	        door_open();
	        startTime = time(NULL);
		    }
        //Lukk døra og resett timeren etter 3 sekunder. Går til idle etterpå.
        if (time(NULL) - startTime > 3) {
        	door_close();
        	startTime = -1;
        	elev_state = idle;
        }

		    break;

    	case stop:
        //Stop-state aktiveres ved nødstopp. Her skal køen slettes og nye bestillinger skal ikke tas hensyn til.
    		//printf("stop");

        //Tømmer køen, stopper motor.
        queue_clear();
        elev_set_motor_direction(DIRN_STOP);

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
