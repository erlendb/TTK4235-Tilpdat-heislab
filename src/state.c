#include "driver/elev.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "state.h"
#include "timer.h"
#include "elevator.h"

#include <stdlib.h>
#include <stdio.h>

elev_motor_direction_t lastDirectionBeforeStop = 0;

state_t state_start() {
  if (!elev_init()) {
    printf("Beklager virkelig altså, men det ser ut til at heisen har fått influensa. Tror du må ta trappa i dag :(\n");
    exit(1);
  }

	queue_clear_all();
	if (elev_get_floor_sensor_signal() == BETWEEN_FLOORS) elevator_set_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == BETWEEN_FLOORS);
	elevator_set_direction(DIRN_STOP);
	elev_set_floor_indicator(elev_get_floor_sensor_signal());

	return IDLE;
}

state_t state_idle() {
  if (queue_get_order(elev_get_floor_sensor_signal()) != ORDER_NONE) return STAY_ENTRY;
	else if (queue_count_orders()) return GO_ENTRY;
	else return IDLE;
}

state_t state_go_entry() {
	int currentFloor = elev_get_floor_sensor_signal();

  if (currentFloor == BETWEEN_FLOORS && currentDirection == DIRN_STOP) {
    if (!lastDirectionBeforeStop) lastDirectionBeforeStop=lastDirection;

    //Vill algoritme som finner riktig retning for stuck heis:
    if (queue_check_above(lastFloor-(lastDirectionBeforeStop==DIRN_DOWN))) elevator_set_direction(DIRN_UP);
    else elevator_set_direction(DIRN_DOWN);
  }
  else if (currentFloor != BETWEEN_FLOORS) {
    if (currentFloor == 0) elevator_set_direction(DIRN_UP);
    else if (currentFloor == N_FLOORS - 1) elevator_set_direction(DIRN_DOWN);
    else if (lastDirection == DIRN_UP && !queue_check_above(currentFloor)) elevator_set_direction(DIRN_DOWN);
    else if (lastDirection == DIRN_DOWN && !queue_check_below(currentFloor)) elevator_set_direction(DIRN_UP);
    else if (lastDirection == DIRN_STOP && queue_check_above(currentFloor)) elevator_set_direction(DIRN_UP);
    else if (lastDirection == DIRN_STOP) elevator_set_direction(DIRN_DOWN);
    else elevator_set_direction(lastDirection);
  }
  return GO_INTERNAL;
}

state_t state_go_internal() {
  int currentFloor = elev_get_floor_sensor_signal();
  if (currentFloor != BETWEEN_FLOORS) {
    elev_set_floor_indicator(currentFloor);
    lastFloor = currentFloor;
    if (queue_should_stop(currentFloor, currentDirection)) return STAY_ENTRY;
  }
  return GO_INTERNAL;
}

state_t state_stay_entry() {
	int currentFloor = elev_get_floor_sensor_signal();
  if (lastDirectionBeforeStop) lastDirectionBeforeStop=0;
  elevator_set_direction(DIRN_STOP);
  lights_clear(currentFloor);
  queue_clear(currentFloor);
  door_open();
  timer_start();
  return STAY_INTERNAL;
}

state_t state_stay_internal(){
  int currentFloor = elev_get_floor_sensor_signal();
  if (queue_get_order(currentFloor) != ORDER_NONE) return STAY_ENTRY;
  if (timer_check()) return STAY_EXIT;
	else return STAY_INTERNAL;
}

state_t state_stay_exit() {
  door_close();
  return IDLE;
}

state_t state_stop() {
  elevator_set_direction(DIRN_STOP);
  queue_clear_all();
  lights_clear_all();
	elev_set_stop_lamp(1);
  if (elev_get_floor_sensor_signal() != BETWEEN_FLOORS) door_open();

	while (elev_get_stop_signal()) {};

  elev_set_stop_lamp(0);
  if (elev_get_floor_sensor_signal() == BETWEEN_FLOORS) return IDLE;
  else return STAY_ENTRY;
}
