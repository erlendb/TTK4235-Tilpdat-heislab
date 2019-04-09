#include <comedilib.h>

#include "driver/elev.h"
#include "queue.h"
#include "lights.h"
#include "state.h"
#include "elevator.h"

int main() {

  state_t state = state_start();
  if (state == -1) return 0;

  while(1) {
    if (elev_get_stop_signal()) state = STOP;

    int buttonSignals[N_FLOORS][N_BUTTONS];
    elevator_update_button_signals(buttonSignals);
    queue_update(buttonSignals);
    lights_update(buttonSignals);

    switch(state) {
    	case IDLE:
      	state = state_idle();
        break;

    	case GO:
			  state = state_go();
		    break;

    	case STAY:
        state = state_stay();
		    break;

    	case STOP:
        state = state_stop();
		    break;
    	}
	}

  return 0;
}
