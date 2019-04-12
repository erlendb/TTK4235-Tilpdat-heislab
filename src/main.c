#include <comedilib.h>
#include "driver.h"
#include "queue.h"
#include "lights.h"
#include "fsm.h"
#include "buttons.h"

int main() {

  fsm_state_initialize();

  while(1) {
    if (elev_get_stop_signal()) fsm_transition(EMERGENCY, ENTRY);

    buttons_fetch_signals();
    queue_update_from_button_signals();
    lights_update_from_button_signals();

    switch(fsm_get_next_state()) {
    	case IDLE:
      	fsm_state_idle();
        break;
    	case GO:
      	fsm_state_go();
        break;
    	case STAY:
      	fsm_state_stay();
        break;
    	case EMERGENCY:
      	fsm_state_emergency();
        break;
  	}
	}
  return 0;
}
