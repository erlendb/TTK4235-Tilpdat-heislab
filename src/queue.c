#include <stdio.h>

#include "driver/elev.h"
#include "queue.h"
#include "elevator.h"

int queue[N_FLOORS] = {0};

void queue_add(int floor, int order) {
	int oldOrder = queue[floor];

	if (order == ORDER_NONE) queue[floor] = ORDER_NONE;
	else if ((oldOrder == ORDER_UP || oldOrder == ORDER_DOWN) && oldOrder != order) queue[floor] = ORDER_ALL;
	else if (oldOrder != ORDER_ALL) queue[floor] = order;
}

void queue_clear_all() {
	for (int i=0;i<N_FLOORS;i++) {
		queue_add(i,ORDER_NONE);
	}
}

void queue_clear(int floor) {
	queue_add(floor, ORDER_NONE);
}

int queue_count() {
	int count = 0;
	for (int i = 0; i < N_FLOORS; i++) {
		count += (queue[i] != ORDER_NONE);
	}
	return count;
}

int queue_stop(int floor, int direction) {
	int order = queue[floor];

	if (order == ORDER_ALL) return 1;
	else if (direction == DIRN_UP && order == ORDER_UP) return 1;
	else if (direction == DIRN_DOWN && order == ORDER_DOWN) return 1;
	else if (direction == DIRN_UP && order == ORDER_DOWN) return !queue_check_above(floor);
	else if (direction == DIRN_DOWN && order == ORDER_UP) return !queue_check_below(floor);
	else return 0;
}

int queue_check_above(int floor) {
	for (int i = floor + 1; i < N_FLOORS; i++) {
		if (queue[i] != ORDER_NONE) return 1;
	}
	return 0;
}

int queue_check_below(int floor) {
	for (int i = floor - 1; i >= 0; i--) {
		if (queue[i] != ORDER_NONE) return 1;
	}
	return 0;
}

int queue_get_order(int floor) {
	return queue[floor];
}

void queue_update(int buttonSignals[][N_BUTTONS]) {
	for (int floor=0; floor < N_FLOORS; floor++) {
		for (int button=0; button < N_BUTTONS; button++) {
			if (buttonSignals[floor][button]) queue_add(floor, button);
		}
	}
}
