#include <comedilib.h>

#include "driver/elev.h"
#include "queue.h"
#include "lights.h"
#include "state.h"
#include "elevator.h"

int main() {

  state_t state = state_start();

  while(1) {
    if (elev_get_stop_signal()) state = stop;

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
