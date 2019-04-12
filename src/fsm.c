#include "driver.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "fsm.h"
#include "timer.h"
#include "buttons.h"

#include <stdlib.h>
#include <stdio.h>

static state_t nextState = IDLE;
static state_action_t nextAction = ENTRY;

int currentFloor = BETWEEN_FLOORS;
int lastFloor = BETWEEN_FLOORS;
elev_motor_direction_t currentDirection = DIRN_STOP;
elev_motor_direction_t lastDirection = DIRN_STOP;
elev_motor_direction_t lastDirectionBeforeStop = DIRN_STOP;

void fsm_update_current_floor() {
  currentFloor = elev_get_floor_sensor_signal();
}

void fsm_set_elevator_direction(elev_motor_direction_t dirn) {
	elev_set_motor_direction(dirn);
	if (dirn != DIRN_STOP) lastDirection = dirn;
	currentDirection = dirn;
}

state_t fsm_get_next_state() {
  return nextState;
}

void fsm_transition(state_t state, state_action_t action) {
  nextState = state;
  nextAction = action;
}

void fsm_state_initialize() {
  if (!elev_init()) {
    printf("Beklager virkelig altså, men det ser ut til at heisen har fått influensa. Tror du må ta trappa i dag :(\n");
    exit(1);
  }
	queue_clear_all();
  fsm_update_current_floor();
	if (currentFloor == BETWEEN_FLOORS) fsm_set_elevator_direction(DIRN_DOWN);
	while (currentFloor == BETWEEN_FLOORS) {
    fsm_update_current_floor();
  }
	fsm_set_elevator_direction(DIRN_STOP);
	elev_set_floor_indicator(currentFloor);
	fsm_transition(IDLE, ENTRY);
}

void fsm_state_idle() {
  if (queue_get_order(currentFloor) != ORDER_NONE) fsm_transition(STAY, ENTRY);
	else if (queue_any_orders()) fsm_transition(GO, ENTRY);
}

void fsm_state_go() {
  switch(nextAction) {
    case ENTRY:
      if (currentFloor == BETWEEN_FLOORS && currentDirection == DIRN_STOP) {
        if (lastDirectionBeforeStop == DIRN_STOP) lastDirectionBeforeStop = lastDirection;
        if (queue_any_orders_above(lastFloor) || (lastDirectionBeforeStop == DIRN_DOWN && queue_get_order(lastFloor) != ORDER_NONE)) fsm_set_elevator_direction(DIRN_UP);
        else fsm_set_elevator_direction(DIRN_DOWN);
      }
      else {
        if (currentFloor == 0) fsm_set_elevator_direction(DIRN_UP);
        else if (currentFloor == N_FLOORS - 1) fsm_set_elevator_direction(DIRN_DOWN);
        else if (lastDirection == DIRN_UP && !queue_any_orders_above(currentFloor)) fsm_set_elevator_direction(DIRN_DOWN);
        else if (lastDirection == DIRN_DOWN && !queue_any_orders_below(currentFloor)) fsm_set_elevator_direction(DIRN_UP);
        else if (lastDirection == DIRN_STOP && queue_any_orders_above(currentFloor)) fsm_set_elevator_direction(DIRN_UP);
        else if (lastDirection == DIRN_STOP) fsm_set_elevator_direction(DIRN_DOWN);
        else fsm_set_elevator_direction(lastDirection);
      }
      fsm_transition(GO, INTERNAL);

    case INTERNAL:
      fsm_update_current_floor();
      if (currentFloor != BETWEEN_FLOORS) {
        lastDirectionBeforeStop = DIRN_STOP;
        if (currentFloor != lastFloor) {
          elev_set_floor_indicator(currentFloor);
          lastFloor = currentFloor;
        }
        if (queue_should_stop(currentFloor, currentDirection)) fsm_transition(STAY, ENTRY);
      }
    break;

    default:
    break;
  }
}

void fsm_state_stay() {
  switch (nextAction) {
    case ENTRY:
      fsm_set_elevator_direction(DIRN_STOP);
      lights_clear_floor(currentFloor);
      queue_clear_floor(currentFloor);
      door_open();
      timer_start();
      fsm_transition(STAY, INTERNAL);

    case INTERNAL:
      if (queue_get_order(currentFloor) != ORDER_NONE) {
        lights_clear_floor(currentFloor);
        queue_clear_floor(currentFloor);
        timer_start();
      }
      else if (timer_expired()) fsm_transition(STAY, EXIT);
    break;

    case EXIT:
      door_close();
      fsm_transition(IDLE, ENTRY);
  }
}

void fsm_state_emergency() {
  fsm_set_elevator_direction(DIRN_STOP);
  queue_clear_all();
  lights_clear_all();
	elev_set_stop_lamp(1);
  fsm_update_current_floor();
  if (currentFloor != BETWEEN_FLOORS) door_open();
	while (elev_get_stop_signal()) {};
  elev_set_stop_lamp(0);
  if (currentFloor == BETWEEN_FLOORS) fsm_transition(IDLE, ENTRY);
  else fsm_transition(STAY, ENTRY);
}
