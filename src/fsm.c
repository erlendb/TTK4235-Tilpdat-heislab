#include "driver/elev.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "fsm.h"
#include "timer.h"
#include "elevator.h"

#include <stdlib.h>
#include <stdio.h>

static state_t nextState = IDLE;
static state_action stateAction = ENTRY;

state_t get_next_state() {
  return nextState;
}

void fsm_transition(state_t state, state_action action) {
  nextState = state;
  stateAction = action;
}

void fsm_state_start() {
  if (!elev_init()) {
    printf("Beklager virkelig altså, men det ser ut til at heisen har fått influensa. Tror du må ta trappa i dag :(\n");
    exit(1);
  }
	queue_clear_all();
  update_current_floor();
	if (currentFloor == BETWEEN_FLOORS) elevator_set_direction(DIRN_DOWN);
	while (currentFloor == BETWEEN_FLOORS) {
    update_current_floor();
  }
	elevator_set_direction(DIRN_STOP);
	elev_set_floor_indicator(currentFloor);
	fsm_transition(IDLE, ENTRY);
}

void fsm_state_idle() {
  if (queue_get_order(currentFloor) != ORDER_NONE) fsm_transition(STAY, ENTRY);
	else if (queue_count_orders()) fsm_transition(GO, ENTRY);
}

void fsm_state_go() {
  switch(stateAction) {
    case ENTRY:
      if (currentFloor == BETWEEN_FLOORS && currentDirection == DIRN_STOP) {
        if (lastDirectionBeforeStop == DIRN_STOP) lastDirectionBeforeStop = lastDirection;

        //if (queue_check_above(lastFloor - (lastDirectionBeforeStop == DIRN_DOWN))) elevator_set_direction(DIRN_UP);
        if (queue_check_above(lastFloor) || (lastDirectionBeforeStop == DIRN_DOWN && queue_get_order(lastFloor) != ORDER_NONE)) elevator_set_direction(DIRN_UP);
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
      fsm_transition(GO, INTERNAL);

    case INTERNAL:
      if (currentFloor != BETWEEN_FLOORS) {
        if (lastFloor != currentFloor) elev_set_floor_indicator(currentFloor);
        lastFloor = currentFloor;
        if (queue_should_stop(currentFloor, currentDirection)) fsm_transition(STAY, ENTRY);
      }
    break;

    default:
    break;
  }
}

void fsm_state_stay() {
  switch (stateAction) {
    case ENTRY:
      lastDirectionBeforeStop = DIRN_STOP;
      elevator_set_direction(DIRN_STOP);
      lights_clear(currentFloor);
      queue_clear(currentFloor);
      door_open();
      timer_start();
      fsm_transition(STAY, INTERNAL);

    case INTERNAL:
      if (queue_get_order(currentFloor) != ORDER_NONE) fsm_transition(STAY, ENTRY);
      else if (timer_check()) fsm_transition(STAY, EXIT);
    break;

    case EXIT:
      door_close();
      fsm_transition(IDLE, ENTRY);
  }
}

void fsm_state_stop() {
    elevator_set_direction(DIRN_STOP);
    queue_clear_all();
    lights_clear_all();
  	elev_set_stop_lamp(1);
    if (elev_get_floor_sensor_signal() != BETWEEN_FLOORS) door_open();
  	while (elev_get_stop_signal()) {};
    elev_set_stop_lamp(0);
    if (elev_get_floor_sensor_signal() == BETWEEN_FLOORS) fsm_transition(IDLE, ENTRY);
    else fsm_transition(STAY, ENTRY);
}
