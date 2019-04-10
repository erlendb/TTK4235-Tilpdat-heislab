#include <comedilib.h>

#include "driver/elev.h"
#include "queue.h"
#include "lights.h"
#include "fsm.h"
#include "elevator.h"

int main() {

  fsm_state_initialize();

  while(1) {
    if (elev_get_stop_signal()) fsm_transition(STOP, ENTRY);

    elevator_fetch_button_signals();
    queue_update_from_button_signals();
    lights_update_from_button_signals();

    switch(get_next_state()) {
    	case IDLE:
      	fsm_state_idle();
        break;
    	case GO:
      	fsm_state_go();
        break;
    	case STAY:
      	fsm_state_stay();
        break;
    	case STOP:
      	fsm_state_emergency();
        break;
  	}
	}
  return 0;
}
