#include <stdio.h>

#include "driver/elev.h"
#include "queue.h"
#include "elevator.h"

int queue[N_FLOORS] = {0};

//Oppdaterer køen. Bør kanskje bytte navn til queue_set()
void queue_add(int floor, int order) {
	int oldOrder = queue[floor];

	if (order == ORDER_NO) queue[floor] = ORDER_NO;
	else if ((oldOrder == ORDER_UP || oldOrder == ORDER_DOWN) && oldOrder != order) queue[floor] = ORDER_ALL;
	else if (oldOrder != ORDER_ALL) queue[floor] = order;
}

//Sletter alle bestillinger i køen
void queue_clear_all() {
	for (int i=0;i<N_FLOORS;i++) {
		queue_add(i,ORDER_NO);
	}
}

void queue_clear(int floor) {
	queue_add(floor, ORDER_NO);
}

//Teller antall etasjer som har bestillinger
int queue_count() {
	int count = 0;
	for (int i = 0; i < N_FLOORS; i++) {
		count += (queue[i] != ORDER_NO);
	}
	return count;
}

//Returnerer hvorvidt vi skal stoppe i gitt etasje
int queue_stop(int floor) {
	order_t order = queue[floor];

	//Bestilling inni heis eller bestilling både opp og ned. Stopper uansett.
	if (order == ORDER_ALL) return 1;
	// Bestilling i samme retning som heisen kjører. Stopper.
	if (lastDirection == DIRN_UP && order == ORDER_UP) return 1;
	if (lastDirection == DIRN_DOWN && order == ORDER_DOWN) return 1;
	// Bestilling motsatt av heisens retning. Stopper hvis det ikke er noen flere bestillinger lenger bort i heisens kjøreretning.
	if (lastDirection == DIRN_UP && order == ORDER_DOWN) return !queue_check_above(floor);
	if (lastDirection == DIRN_DOWN && order == ORDER_UP) return !queue_check_below(floor);
	// Ingen bestilling. Stopper ikke.
	return 0;
}

// Sjekker om det finnes flere bestillinger over gitt etasje. True: det finnes bestillinger. False: ingen bestillinger.
int queue_check_above(int floor) {
	for (int i = floor + 1; i < N_FLOORS; i++) {
		if (queue[i] != ORDER_NO) return 1;
	}
	return 0;
}

//Sjekker om det finnes flere bestillinger under gitt etasje.
int queue_check_below(int floor) {
	for (int i = floor - 1; i >= 0; i--) {
		if (queue[i] != ORDER_NO) return 1;
	}
	return 0;
}

int queue_get(int floor) {
	return queue[floor];
}

void queue_update(int buttonSignals[][N_BUTTONS]) {
	for (int floor=0; floor < N_FLOORS; floor++) {
		for (int button=0; button < N_BUTTONS; button++) {
			if (buttonSignals[floor][button]) queue_add(floor, button);
		}
	}
}
