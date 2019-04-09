#include <comedilib.h>

#include "driver/elev.h"
#include "queue.h"
#include "lights.h"
#include "state.h"
#include "elevator.h"

int main() {

  state_t state = state_start();

  while(1) {
    if (elev_get_stop_signal()) state = STOP_ENTRY;

    int buttonSignals[N_FLOORS][N_BUTTONS];
    elevator_update_button_signals(buttonSignals);
    queue_update(buttonSignals);
    lights_update(buttonSignals);

    switch(state) {
    	case IDLE:
      	state = state_idle_internal();
        break;

    	case GO_ENTRY:
      	state = state_go_entry();
        break;
    	case GO_INTERNAL:
      	state = state_go_internal();
        break;

    	case STAY_ENTRY:
      	state = state_stay_entry();
        break;
    	case STAY_INTERNAL:
      	state = state_stay_internal();
        break;
    	case STAY_EXIT:
      	state = state_stay_exit();
        break;

    	case STOP_ENTRY:
      	state = state_stop_entry();
        break;
    	case STOP_INTERNAL:
      	state = state_stop_internal();
        break;
  	}
	}

  return 0;
}
