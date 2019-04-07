#include "driver/elev.h"
#include "door.h"
#include "queue.h"
#include "lights.h"
#include "state.h"
#include "timer.h"

#include <stdio.h>

int stopped = NOT_STOPPED;

state_t state_start(){
  // Initialiserer heishardvaren
  if (!elev_init()) {
    printf("Unable to initialize elevator hardware!\n");
    return 1;
  }

	//Initialiserer køen.
	queue_clear();

	//Kjører heisen ned til nærmeste etasje. Sender så heisen inn i idle-state
	set_direction(DIRN_DOWN);

	while (elev_get_floor_sensor_signal() == -1);

	set_direction(DIRN_STOP);
	//Setter etasjelampe i riktig etasje
	elev_set_floor_indicator(elev_get_floor_sensor_signal());

	//Sender heisen til idle-state
	return idle;
}

state_t state_idle(){
	//Idle-state skal ikke gjøre noe som helst. Sender rett videre til go-state hvis det finnes noe i køen. Alternativt venter på ny bestilling.

		//Gå til go-state hvis det finnes bestilling i køen
	if (queue_count()) {
	  return go;
	}
	return idle;
}

state_t state_go() {
  //Oppdaterer etasjevariabel
	currentFloor = elev_get_floor_sensor_signal();

    //Sender heisen i riktig retning ut fra idle-state og stop-state
    if (currentFloor == -1 && direction == DIRN_STOP) {
      //Vill algoritme som finner riktig retning for stuck heis:
      if (queue_check_above(lastFloor-(lastDirection==DIRN_DOWN))) set_direction(DIRN_UP);
      else set_direction(DIRN_DOWN);

    } else if (queue_get(currentFloor) == -1) {
      set_direction(lastDirection);
    }
    //Go-state skal kjøre heisen i retning av en bestilling. Skal stoppe hvis vi passerer etasjer med bestillinger i samme retning.


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
        if (currentFloor == 0) {
          set_direction(DIRN_UP);
        } else if (currentFloor == N_FLOORS-1) {
          set_direction(DIRN_DOWN);
        }
        //Snu retningen hvis det ikke er flere bestillinger lenger fram i kjøreretningen
        if (lastDirection == DIRN_UP && !queue_check_above(currentFloor)) set_direction(DIRN_DOWN);
        if (lastDirection == DIRN_DOWN && !queue_check_below(currentFloor)) set_direction(DIRN_UP);
      }

    } else {
  		//Hvis vi er mellom to etasjer skal heisen bare surre videre i samme retning :)
	}
	return go;
}

state_t state_stay(){

	int currentFloor = elev_get_floor_sensor_signal();

    //Stay-state skal passe på alt som skjer i en etasje: åpne/lukke dør, obstruksjon...

    //Stopper heisen, skrur av lys, fjerner denne etasjen fra køen, åpner døra og starter timeren i det vi kommer inn i stay-state
	if (!timer_is_activated()) {
        set_direction(DIRN_STOP);
        elev_set_button_lamp(BUTTON_COMMAND, currentFloor, 0);
        if (currentFloor != N_FLOORS -1) elev_set_button_lamp(BUTTON_CALL_UP, currentFloor, 0);
        if (currentFloor != 0)			elev_set_button_lamp(BUTTON_CALL_DOWN, currentFloor, 0);
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

  queue_clear();
  lights_clear();
	elev_set_stop_lamp(1);

	currentFloor = elev_get_floor_sensor_signal();

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
/*
state_t state_stop() {
	currentFloor = elev_get_floor_sensor_signal();
    //Stop-state aktiveres ved nødstopp. Her skal køen slettes og nye bestillinger skal ikke tas hensyn til.

    //Stopper motor, setter stopplampa, resetter dørtimer og evt. åpner døra når vi går inn i nødstopp
    if (!stopped) {
      set_direction(DIRN_STOP);
      timer_deactivate(); //Denne er viktig i tilfelle nødstopp inntreffer i stay-state mens døra er åpen.

      //Åpner døra hvis vi er i en etasje
      if (currentFloor != -1) door_open();
    }
    //Setter stoppflagg
    stopped = STOPPED;
	//Hver gang knappen trykkes skrus lyset på og dørtimeren blir satt på nytt
	if (elev_get_stop_signal()){
		timer_start();
		elev_set_stop_lamp(1);
	}
    //Tømmer køen og hindrer nye Bestillinger
    queue_clear();
    lights_clear();

    //Nødstopp deaktivert
    if (!elev_get_stop_signal()) {
      //Skrur av stopplys
      elev_set_stop_lamp(0);

      //Nellom to etasjer: sett stoppflagg og gå til idle-state
      if (currentFloor == -1) {
        //Endrer stoppflagget hvis heisen står mellom to etasjer så det kan brukes av go-state
         stopped = NOT_STOPPED;
         return idle;
      }
      //I en etasje: starter dørklokka
      else {
        if (!timer_is_activated()) {
          timer_start();
        }
      }
    }

    //Når døra har vært åpen lenge nok endrer vi stoppflagget og går til idle-state
    if (currentFloor != -1) {
      if (timer_check()) {
        timer_deactivate();
        door_close();
        //Fjerner stoppflagget hvis vi står i en etasje
        stopped = NOT_STOPPED;
        return idle;
      }
    }

	return stop;
}*/
