#include <stdio.h>

#include "driver.h"
#include "queue.h"
#include "buttons.h"

#define BETWEEN_FLOORS -1

int queue[N_FLOORS] = {0};

void queue_add_order(int floor, order_t order) {
	order_t oldOrder = queue[floor];

	if (order == ORDER_NONE) queue[floor] = ORDER_NONE;
	else if ((oldOrder == ORDER_UP || oldOrder == ORDER_DOWN) && oldOrder != order) queue[floor] = ORDER_ALL;
	else if (oldOrder != ORDER_ALL) queue[floor] = order;
}

void queue_clear_all() {
	for (int floor = 0; floor < N_FLOORS; floor++) {
		queue_add_order(floor, ORDER_NONE);
	}
}

void queue_clear_floor(int floor) {
	queue_add_order(floor, ORDER_NONE);
}

int queue_any_orders() {
	for (int floor = 0; floor < N_FLOORS; floor++) {
		if (queue[floor] != ORDER_NONE) return 1;
	}
	return 0;
}

int queue_should_stop(int floor, elev_motor_direction_t direction) {
	order_t order = queue[floor];

	if (floor == BETWEEN_FLOORS) return 0;
	else if (order == ORDER_ALL) return 1;
	else if (direction == DIRN_UP && order == ORDER_UP) return 1;
	else if (direction == DIRN_DOWN && order == ORDER_DOWN) return 1;
	else if (direction == DIRN_UP && order == ORDER_DOWN) return !queue_any_orders_above(floor);
	else if (direction == DIRN_DOWN && order == ORDER_UP) return !queue_any_orders_below(floor);
	else return 0;
}

int queue_any_orders_above(int floor) {
	for (int f = floor + 1; f < N_FLOORS; f++) {
		if (queue[f] != ORDER_NONE) return 1;
	}
	return 0;
}

int queue_any_orders_below(int floor) {
	for (int f = floor - 1; f >= 0; f--) {
		if (queue[f] != ORDER_NONE) return 1;
	}
	return 0;
}

order_t queue_get_order(int floor) {
	if (floor == BETWEEN_FLOORS) return ORDER_NONE;
	else return queue[floor];
}

void queue_update_from_button_signals() {
	for (int floor=0; floor < N_FLOORS; floor++) {
		for (int button=0; button < N_BUTTONS; button++) {
			if (buttons_get_signal(floor, button)) queue_add_order(floor, button);
		}
	}
}
