#include "driver/elev.h"

void door_open(){
	elev_set_door_open_lamp(1);
}

void door_close(){
	elev_set_door_open_lamp(0);
}
