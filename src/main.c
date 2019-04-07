#include <stdio.h>
#include <comedilib.h>
#include <time.h>

#include "driver/elev.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "state.h"
#include "timer.h"
#include "elevator.h"

int main() {

  //Initialiserer heisen
  state_t state = state_start();

  while(1) {
    //Ser etter stoppsignal. Går til stop-state hvis nødstopp aktiveres.
    if (elev_get_stop_signal()) state = stop;

    //Itererer over alle knapper i alle etasjer for å tenne lykter og stappe bestillinger inn i køen

    int buttonSignals[N_FLOORS][N_BUTTONS];
    elevator_update_button_signals(buttonSignals);
    queue_update(buttonSignals);
    lights_update(buttonSignals);

    switch(state) {
    	case idle:
      	state = state_idle();
        break;

    	case go:
			  state = state_go();
		    break;

    	case stay:
        state = state_stay();
		    break;

    	case stop:
        state = state_stop();
		    break;
    	}
	}

  return 0;
}
