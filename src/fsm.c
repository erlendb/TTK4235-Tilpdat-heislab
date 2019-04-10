#include "driver/elev.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "fsm.h"
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

void fsm_transition(state_t state, state_action action) {
  set_next_state(state);
  set_state_action(action);
}

void fsm_state_start() {
  if (!elev_init()) {
    printf("Beklager virkelig altså, men det ser ut til at heisen har fått influensa. Tror du må ta trappa i dag :(\n");
    exit(1);
  }
	queue_clear_all();
	if (elev_get_floor_sensor_signal() == BETWEEN_FLOORS) elevator_set_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == BETWEEN_FLOORS);
	elevator_set_direction(DIRN_STOP);
	elev_set_floor_indicator(elev_get_floor_sensor_signal());
	fsm_transition(IDLE, ENTRY);
}

void fsm_state_idle() {
    if (queue_get_order(get_current_floor()) != ORDER_NONE) fsm_transition(STAY, ENTRY);
  	else if (queue_count_orders()) fsm_transition(GO, ENTRY);
}

void fsm_state_go() {
  switch(get_state_action()) {
    case ENTRY:
      if (get_current_floor() == BETWEEN_FLOORS && get_current_direction() == DIRN_STOP) {
        if (!get_last_direction_before_stop()) set_last_direction_before_stop(get_last_direction());

        //Vill algoritme som finner riktig retning for stuck heis:
        if (queue_check_above(get_last_floor()-(get_last_direction_before_stop()==DIRN_DOWN))) elevator_set_direction(DIRN_UP);
        else elevator_set_direction(DIRN_DOWN);
      }
      else if (get_current_floor() != BETWEEN_FLOORS) {
        if (get_current_floor() == 0) elevator_set_direction(DIRN_UP);
        else if (get_current_floor() == N_FLOORS - 1) elevator_set_direction(DIRN_DOWN);
        else if (get_last_direction() == DIRN_UP && !queue_check_above(get_current_floor())) elevator_set_direction(DIRN_DOWN);
        else if (get_last_direction() == DIRN_DOWN && !queue_check_below(get_current_floor())) elevator_set_direction(DIRN_UP);
        else if (get_last_direction() == DIRN_STOP && queue_check_above(get_current_floor())) elevator_set_direction(DIRN_UP);
        else if (get_last_direction() == DIRN_STOP) elevator_set_direction(DIRN_DOWN);
        else elevator_set_direction(get_last_direction());
      }
      fsm_transition(GO, INTERNAL);

    case INTERNAL:
      if (get_current_floor() != BETWEEN_FLOORS) {
        elev_set_floor_indicator(get_current_floor());
        set_last_floor(get_current_floor());
        if (queue_should_stop(get_current_floor(), get_current_direction())) fsm_transition(STAY, ENTRY);
      }
    break;

    default:
    break;
  }
}

void fsm_state_stay() {
  switch (get_state_action()) {
    case ENTRY:
      if (get_last_direction_before_stop()) set_last_direction_before_stop(0);
      elevator_set_direction(DIRN_STOP);
      lights_clear(get_current_floor());
      queue_clear(get_current_floor());
      door_open();
      timer_start();
      fsm_transition(STAY, INTERNAL);

    case INTERNAL:
      if (queue_get_order(get_current_floor()) != ORDER_NONE) fsm_transition(STAY, ENTRY);
      if (timer_check()) fsm_transition(STAY, EXIT);
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
