#include "driver/elev.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "state.h"
#include "timer.h"

#include <stdio.h>

state_t state_start() {
  // Initialiserer heishardvaren
  if (!elev_init()) {
    printf("Beklager virkelig altså, men det ser ut til at heisen har fått influensa. Tror du må ta trappa i dag :(\n");
    return 1;
  }

	//Initialiserer køen.
	queue_clear_all();

	//Kjører heisen ned til nærmeste etasje. Sender så heisen inn i idle-state
	if (elev_get_floor_sensor_signal() == -1) set_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == -1);
	set_direction(DIRN_STOP);
	elev_set_floor_indicator(elev_get_floor_sensor_signal());

	return idle;
}

state_t state_idle(){
	if (queue_count()) return go;
	else return idle;
}

state_t state_go() {
  //Oppdaterer etasjevariabel
	int currentFloor = elev_get_floor_sensor_signal();

  //Sender heisen i riktig retning ut fra stop-state
  if (currentFloor == -1 && direction == DIRN_STOP) {
    //Vill algoritme som finner riktig retning for stuck heis:
    if (queue_check_above(lastFloor-(lastDirection==DIRN_DOWN))) set_direction(DIRN_UP);
    else set_direction(DIRN_DOWN);
  }

  //Hvis vi har havnet i en etasje:
  if (currentFloor != -1) {
    //Oppdaterer etasjelys og lastFloor
  	elev_set_floor_indicator(currentFloor);
  	lastFloor = currentFloor;

    //Sjekker om vi har en bestilling (aka om vi skal stoppe). Går eventuelt til stay-state.
    if (queue_stop(currentFloor)) {
      return stay;
    } else {
      //Bestemme retning på heisen
      //Snu retningen hvis vi er øverst eller nederst
      if (currentFloor == 0) set_direction(DIRN_UP);
      else if (currentFloor == N_FLOORS-1) set_direction(DIRN_DOWN);

      //Snu retningen hvis det ikke er flere bestillinger lenger fram i kjøreretningen. Gå mot bestilling hvis vi kommer fra start-state, Ellers fortsett i samme retning.
      if (lastDirection == DIRN_UP && !queue_check_above(currentFloor)) set_direction(DIRN_DOWN);
      else if (lastDirection == DIRN_DOWN && !queue_check_below(currentFloor)) set_direction(DIRN_UP);
      else if (lastDirection == DIRN_STOP && queue_check_above(currentFloor)) set_direction(DIRN_UP);
      else if (lastDirection == DIRN_STOP) set_direction(DIRN_DOWN);
      else set_direction(lastDirection);
    }
  }
	return go;
}

state_t state_stay(){

	int currentFloor = elev_get_floor_sensor_signal();

  //Stopper heisen, skrur av lys, fjerner denne etasjen fra køen, åpner døra og starter timeren i det vi kommer inn i stay-state
	if (!timer_is_activated()) {
    set_direction(DIRN_STOP);
    elev_set_button_lamp(BUTTON_COMMAND, currentFloor, 0);
    if (currentFloor != N_FLOORS -1) elev_set_button_lamp(BUTTON_CALL_UP, currentFloor, 0);
    if (currentFloor != 0) elev_set_button_lamp(BUTTON_CALL_DOWN, currentFloor, 0);
    queue_update(currentFloor,-1);
    door_open();
    timer_start();
  }

  //Resetter timeren (holder døra oppe litt til), skrur av lys og fjerner fra kø hvis det dukker opp bestillinger i samme etasje som vi står i
  if (queue_get(currentFloor) != -1) {
    timer_deactivate();
  }
  //Lukk døra og resett timeren etter 3 sekunder. Går til idle etterpå.
  if (timer_check()) {
  	door_close();
    timer_deactivate();
  	return idle;
  }
	return stay;
}

state_t state_stop() {
  if (direction != DIRN_STOP) set_direction(DIRN_STOP);

  queue_clear_all();
  lights_clear_all();
	elev_set_stop_lamp(1);

	int currentFloor = elev_get_floor_sensor_signal();

  //Åpner døra hvis vi er i en etasje
  if (currentFloor != -1) {
		door_open();
	}

	while (elev_get_stop_signal());

	elev_set_stop_lamp(0);

  if (currentFloor == -1) {
     return idle;
  } else {
		timer_start();
    return stay;
	}

	return stop;
}
