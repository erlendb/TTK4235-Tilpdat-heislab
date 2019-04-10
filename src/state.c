#include "driver/elev.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "state.h"
#include "timer.h"
#include "elevator.h"

#include <stdlib.h>
#include <stdio.h>

state_t nextState = IDLE;
state_t get_next_state() { return nextState; }
void set_next_state(state_t state) { nextState = state; }

state_action stateAction = ENTRY;
state_action get_state_action() { return stateAction; }
void set_state_action(state_action action) { stateAction = action; }

elev_motor_direction_t lastDirectionBeforeStop = DIRN_STOP;
elev_motor_direction_t get_last_direction_before_stop() { return lastDirectionBeforeStop; }
void set_last_direction_before_stop(elev_motor_direction_t dirn) { lastDirectionBeforeStop = dirn; }



void state_transition(state_t state, state_action action) {
  set_next_state(state);
  set_state_action(action);
}

void state_start() {
  if (!elev_init()) {
    printf("Beklager virkelig altså, men det ser ut til at heisen har fått influensa. Tror du må ta trappa i dag :(\n");
    exit(1);
  }
	queue_clear_all();
	if (elev_get_floor_sensor_signal() == BETWEEN_FLOORS) elevator_set_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == BETWEEN_FLOORS);
	elevator_set_direction(DIRN_STOP);
	elev_set_floor_indicator(elev_get_floor_sensor_signal());
	state_transition(IDLE, ENTRY);
}

void state_idle() {
    if (queue_get_order(currentFloor) != ORDER_NONE) state_transition(STAY, ENTRY);
  	else if (queue_count_orders()) state_transition(GO, ENTRY);
}

void state_go() {
  switch(get_state_action()) {
    case ENTRY:
      if (currentFloor == BETWEEN_FLOORS && currentDirection == DIRN_STOP) {
        if (!get_last_direction_before_stop()) set_last_direction_before_stop(lastDirection);

        //Vill algoritme som finner riktig retning for stuck heis:
        if (queue_check_above(lastFloor-(get_last_direction_before_stop()==DIRN_DOWN))) elevator_set_direction(DIRN_UP);
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
      state_transition(GO, INTERNAL);

    case INTERNAL:
      if (currentFloor != BETWEEN_FLOORS) {
        elev_set_floor_indicator(currentFloor);
        lastFloor = currentFloor;
        if (queue_should_stop(currentFloor, currentDirection)) state_transition(STAY, ENTRY);
      }
    break;

    default:
    break;
  }
}

void state_stay() {
  switch (get_state_action()) {
    case ENTRY:
      if (get_last_direction_before_stop()) set_last_direction_before_stop(0);
      elevator_set_direction(DIRN_STOP);
      lights_clear(currentFloor);
      queue_clear(currentFloor);
      door_open();
      timer_start();
      state_transition(STAY, INTERNAL);

    case INTERNAL:
      if (queue_get_order(currentFloor) != ORDER_NONE) state_transition(STAY, ENTRY);
      if (timer_check()) state_transition(STAY, EXIT);
    break;

    case EXIT:
      door_close();
      state_transition(IDLE, ENTRY);
  }
}

void state_stop() {
    elevator_set_direction(DIRN_STOP);
    queue_clear_all();
    lights_clear_all();
  	elev_set_stop_lamp(1);
    if (elev_get_floor_sensor_signal() != BETWEEN_FLOORS) door_open();

  	while (elev_get_stop_signal()) {};

    elev_set_stop_lamp(0);
    if (elev_get_floor_sensor_signal() == BETWEEN_FLOORS) state_transition(IDLE, ENTRY);
    else state_transition(STAY, ENTRY);
}
