#include "door.h"
#include "driver/elev.h"
#include <time.h>

int doorTimer = -1;

void door_open(){
	elev_set_door_open_lamp(1);
}

void door_close(){
	elev_set_door_open_lamp(0);
}

void door_timer_start() {
	doorTimer = time(NULL);
}

int door_timer_exceeds_threshold() {
	return (time(NULL) - doorTimer > 3);
}

void door_timer_deactivate() {
	doorTimer = -1;
}

int door_is_timer_activated() {
	return (doorTimer != -1);
}
