#include <comedilib.h>

#include "driver/elev.h"
#include "queue.h"
#include "lights.h"
#include "fsm.h"
#include "elevator.h"

int main() {

  state_start();

  while(1) {
    if (elev_get_stop_signal()) set_next_state(STOP);

    int buttonSignals[N_FLOORS][N_BUTTONS];
    elevator_update_button_signals(buttonSignals);
    queue_update(buttonSignals);
    lights_update(buttonSignals);

    currentFloor = elev_get_floor_sensor_signal();

    switch(get_next_state()) {
    	case IDLE:
      	state_idle();
        break;

    	case GO:
      	state_go();
        break;

    	case STAY:
      	state_stay();
        break;

    	case STOP:
      	state_stop();
        break;
  	}
	}

  return 0;
}
