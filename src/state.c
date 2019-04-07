#include "driver/elev.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "state.h"
#include "timer.h"
#include "elevator.h"

#include <stdio.h>

state_t state_start() {
  if (!elev_init()) {
    printf("Beklager virkelig altså, men det ser ut til at heisen har fått influensa. Tror du må ta trappa i dag :(\n");
    return 1;
  }

	queue_clear_all();

	if (elev_get_floor_sensor_signal() == -1) elevator_set_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == -1);
	elevator_set_direction(DIRN_STOP);
	elev_set_floor_indicator(elev_get_floor_sensor_signal());

	return idle;
}

state_t state_idle(){
	if (queue_count()) return go;
	else return idle;
}

state_t state_go() {
	int currentFloor = elev_get_floor_sensor_signal();

  if (currentFloor == -1 && currentDirection == DIRN_STOP) {
    //Vill algoritme som finner riktig retning for stuck heis:
    if (queue_check_above(lastFloor-(lastDirection==DIRN_DOWN))) elevator_set_direction(DIRN_UP);
    else elevator_set_direction(DIRN_DOWN);
  }

  if (currentFloor != -1) {
  	elev_set_floor_indicator(currentFloor);
  	lastFloor = currentFloor;

    if (queue_stop(currentFloor, currentDirection)) {
      return stay;
    } else {
      if (currentFloor == 0) elevator_set_direction(DIRN_UP);
      else if (currentFloor == N_FLOORS-1) elevator_set_direction(DIRN_DOWN);
      else if (lastDirection == DIRN_UP && !queue_check_above(currentFloor)) elevator_set_direction(DIRN_DOWN);
      else if (lastDirection == DIRN_DOWN && !queue_check_below(currentFloor)) elevator_set_direction(DIRN_UP);
      else if (lastDirection == DIRN_STOP && queue_check_above(currentFloor)) elevator_set_direction(DIRN_UP);
      else if (lastDirection == DIRN_STOP) elevator_set_direction(DIRN_DOWN);
      else elevator_set_direction(lastDirection);
    }
  }
	return go;
}

state_t state_stay(){
	int currentFloor = elev_get_floor_sensor_signal();

	if (!timer_is_activated()) {
    elevator_set_direction(DIRN_STOP);
    lights_clear(currentFloor);
    queue_clear(currentFloor);
    door_open();
    timer_start();
  }

  if (queue_get_order(currentFloor) != ORDER_NONE) timer_deactivate();

  if (timer_check()) {
  	door_close();
    timer_deactivate();
  	return idle;
  }
	return stay;
}

state_t state_stop() {
  elevator_set_direction(DIRN_STOP);
  queue_clear_all();
  lights_clear_all();
  timer_deactivate();
	elev_set_stop_lamp(1);

	int currentFloor = elev_get_floor_sensor_signal();

  if (currentFloor != -1) door_open();

	while (elev_get_stop_signal());

	elev_set_stop_lamp(0);

  if (currentFloor == -1) return idle;
  else return stay;
}
