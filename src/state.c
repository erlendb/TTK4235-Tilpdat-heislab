#include "driver/elev.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "state.h"
#include "timer.h"
#include "elevator.h"

#include <stdlib.h>
#include <stdio.h>

elev_motor_direction_t lastDirectionBeforeStop = DIRN_STOP;
state_action stateAction = ENTRY;

state_t state_transition(state_t state, state_action action) {
  stateAction = action;
  return state;
}

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
	return state_transition(IDLE, ENTRY);
}

state_t state_idle() {
  switch(stateAction) {
    default:
      if (queue_get_order(elev_get_floor_sensor_signal()) != ORDER_NONE) return state_transition(STAY, ENTRY);
    	else if (queue_count_orders()) return state_transition(GO, ENTRY);
    	else return state_transition(IDLE, ENTRY);
  }
  return ERROR_STATE;
}

state_t state_go() {
  switch(stateAction) {
    case ENTRY:
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
      return state_transition(GO, INTERNAL);

    case INTERNAL:
      if (currentFloor != BETWEEN_FLOORS) {
        elev_set_floor_indicator(currentFloor);
        lastFloor = currentFloor;
        if (queue_should_stop(currentFloor, currentDirection)) return state_transition(STAY, ENTRY);
      }
      return state_transition(GO, INTERNAL);

    default:
    break;
  }
  return ERROR_STATE;
}

state_t state_stay() {
  switch (stateAction) {
    case ENTRY:
      if (lastDirectionBeforeStop) lastDirectionBeforeStop=0;
      elevator_set_direction(DIRN_STOP);
      lights_clear(currentFloor);
      queue_clear(currentFloor);
      door_open();
      timer_start();
      return state_transition(STAY, INTERNAL);

    case INTERNAL:
      if (queue_get_order(currentFloor) != ORDER_NONE) return state_transition(STAY, ENTRY);
      if (timer_check()) return state_transition(STAY, EXIT);
    	else return state_transition(STAY, INTERNAL);

    case EXIT:
      door_close();
      return state_transition(IDLE, ENTRY);
  }
  return ERROR_STATE;
}

state_t state_stop() {
  switch(stateAction) {
    default:
      elevator_set_direction(DIRN_STOP);
      queue_clear_all();
      lights_clear_all();
    	elev_set_stop_lamp(1);
      if (elev_get_floor_sensor_signal() != BETWEEN_FLOORS) door_open();

    	while (elev_get_stop_signal()) {};

      elev_set_stop_lamp(0);
      if (elev_get_floor_sensor_signal() == BETWEEN_FLOORS) return state_transition(IDLE, ENTRY);
      else return state_transition(STAY, ENTRY);
  }
  return ERROR_STATE;
}
