#include <stdio.h>
#include <comedilib.h>
#include <time.h>

#include "driver/elev.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "state.h"

int main() {


  // Initialize hardware
  if (!elev_init()) {
   printf("Unable to initialize elevator hardware!\n");
    return 1;
  }

  //Dørklokke
  door_timer_deactivate();

  //KJør start-state
  state_t state = state_start();

  while(1) {
  //Greier som skal skje for hver iterasjon, uavhengig av state, utføres her

  //Ser etter stoppsignal. Går til stop-state hvis nødstopp aktiveres.
	if (elev_get_stop_signal()) {
	state = stop;
	}
	if (state != stop) elev_set_stop_lamp(0);

	//Itererer over alle knapper i alle etasjer for å tenne lykter og stappe bestillinger inn i køen
    queue_check_buttons();

    //Tilstander: idle, go, stay, stop
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
