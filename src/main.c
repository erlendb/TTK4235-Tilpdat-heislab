#include <comedilib.h>

#include "driver/elev.h"
#include "queue.h"
#include "lights.h"
#include "fsm.h"
#include "elevator.h"

int main() {

  fsm_state_start();

  while(1) {
    if (elev_get_stop_signal()) fsm_transition(STOP, ENTRY);

    elevator_update_button_signals();
    queue_update();
    lights_update();

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
      	fsm_state_stop();
        break;
  	}
	}
  return 0;
}
