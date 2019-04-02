#include <stdio.h>

#include "driver/elev.h"
#include "queue.h"

int lastFloor = -1;
elev_motor_direction_t direction = DIRN_STOP; // -1 = ned, 1 = opp, 0 = stopp
int queue[N_FLOORS] = {0};

//Oppdaterer køen. Bør kanskje bytte navn til queue_set()
void queue_update(int floor, int order) {
	int queueValue = queue[floor];
	if (order == -1){		//kan sette inn ingen bestilling når vi clearer etasjen
		queue[floor] = -1;
		//printf("ny qverdi %d for etasje %d",queue[floor], floor);
	}
	else if (queueValue == 0 || queueValue == 1){
		if (queueValue != order) {
			queue[floor] = 2;		//bestilling opp og ned er det samme som lugarbestilling
		}
	}
	else if (queueValue != 2){
		queue[floor] = order;	//ingen bestilling fra før, setter bestilling opp eller ned
	}
}

//Sletter alle bestillinger i køen
void queue_clear() {
	for (int i=0;i<=3;i++) {
		queue_update(i,-1);
	}
}

//Teller antall etasjer som har bestillinger
int queue_count() {
	int count = 0;
	for (int i = 0; i < N_FLOORS; i++) {
		count += (queue[i] != -1);
	}
	return count;
}

//Returnerer hvorvidt vi skal stoppe i gitt etasje
int queue_stop(int floor) {
	order_t order = queue[floor];

	//Bestilling inni heis eller bestilling både opp og ned. Stopper uansett.
	if (order == ORDER_ALL) {
		return 1;
	}
	// Bestilling i samme retning som heisen kjører. Stopper.
	if (direction == DIRN_UP && order == ORDER_UP) {
		return 1;
	}
	if (direction == DIRN_DOWN && order == ORDER_DOWN) {
		return 1;
	}
	// Bestilling motsatt av heisens retning. Stopper hvis det ikke er noen flere bestillinger lenger bort i heisens kjøreretning.
	if (direction == DIRN_UP && order == ORDER_DOWN) {
		return !queue_check_above(floor);
	}
	if (direction == DIRN_DOWN && order == ORDER_UP) {
		return !queue_check_below(floor);
	}
	// Ingen bestilling. Stopper ikke.
	return 0;
}

// Sjekker om det finnes flere bestillinger over gitt etasje. True: det finnes bestillinger. False: ingen bestillinger.
int queue_check_above(int floor) {
	for (int i = floor + 1; i < N_FLOORS; i++) {
		if (queue[i] != -1) return 1;
	}
	return 0;
}

//Sjekker om det finnes flere bestillinger under gitt etasje.
int queue_check_below(int floor) {
	for (int i = floor - 1; i >= 0; i--) {
		if (queue[i] != -1) return 1;
	}
	return 0;
}

int queue_get(int floor) {
	return queue[floor];
}

//Setter retning på motoren, samt lagrer siste bevegelsesretning i direction-variabelen
// Denne har ikke noe i kømodulen å gjøre
void set_direction(elev_motor_direction_t dirn) {
	elev_set_motor_direction(dirn);
	 //direction får kun verdiene DIRN_UP eller DIRN_DOWN. Ved stopp vil direction inneholde sist kjente heisretning.
	if (dirn != DIRN_STOP) direction = dirn;
}

void queue_check_buttons() {
	for (int f = 0; f < N_FLOORS; f++) {

	    //Henter signaler fra knapper
		int button_command;
		int button_call_up = 0;
		int button_call_down = 0;

	    button_command = elev_get_button_signal(BUTTON_COMMAND,f);
	    if (f != N_FLOORS - 1)	 button_call_up = elev_get_button_signal(BUTTON_CALL_UP, f);
		if (f != 0)				 button_call_down = elev_get_button_signal(BUTTON_CALL_DOWN, f);


	    //Setter tilsvarende bestillingslys
	    if (button_command) elev_set_button_lamp(BUTTON_COMMAND,f,1);
	    if (button_call_up) elev_set_button_lamp(BUTTON_CALL_UP,f,1);
	    if (button_call_down) elev_set_button_lamp(BUTTON_CALL_DOWN,f,1);

	    //Finner bestillinger og oppdaterer køen
	    if (queue_get(f) != ORDER_ALL) {

		      if (button_command
		      || (button_call_up && button_call_down)
		      || (button_call_up && queue_get(f) == ORDER_DOWN)
		      || (button_call_down && queue_get(f) == ORDER_UP)
		      ) {
		        queue_update(f,ORDER_ALL);
		      }
		      else if (button_call_up) queue_update(f, ORDER_UP);
		      else if (button_call_down) queue_update(f, ORDER_DOWN);
      }
    }
}
