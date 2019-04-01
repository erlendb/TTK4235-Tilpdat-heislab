#include <stdio.h>

#include "driver/elev.h"
#include "queue.h"

int lastFloor = -1;
elev_motor_direction_t direction = DIRN_STOP; // -1 = ned, 1 = opp, 0 = stopp
int queue[N_FLOORS] = {0};

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

void queue_clear() {
	for (int i=0;i<=3;i++) {
		queue_update(i,-1);
	}
}

int queue_count() {
	int count = 0;
	for (int i = 0; i < N_FLOORS; i++) {
		count += (queue[i] != -1);
	}
	return count;
}

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

// Sjekker om det finnes flere bestillinger over eller under gitt etasje. True: det finnes bestillinger. False: ingen bestillinger.
int queue_check_above(int floor) {
	for (int i = floor + 1; i < N_FLOORS; i++) {
		if (queue[i] != -1) return 1;
	}
	return 0;
}

int queue_check_below(int floor) {
	for (int i = floor - 1; i >= 0; i--) {
		if (queue[i] != -1) return 1;
	}
	return 0;
}

void set_direction(elev_motor_direction_t dirn) {
	elev_set_motor_direction(dirn);
	 //direction får kun verdiene DIRN_UP eller DIRN_DOWN. Ved stopp vil direction inneholde sist kjente heisretning.
	if (dirn != DIRN_STOP) direction = dirn;
}

void queue_print() {
	printf("\nQueue: %d %d %d %d\n", queue[0], queue[1], queue[2], queue[3]);
}
