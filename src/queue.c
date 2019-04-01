#include "elev.h"

int lastFloor = -1;
int direction = 0; // -1 = ned, 1 = opp, 0 = stopp

int queue[N_FLOORS] = {-1};

void queue_update(int floor, int type) {
	int queueValue = queue[floor];
	if (type == -1){		//kan sette inn ingen bestilling når vi clearer etasjen
		queue[floor] = -1;
	}
	else if (queueValue == 0 || queueValue == 1){
		if (queueValue != type) {
			queue[floor] = 2;		//bestilling opp og ned er det samme som lugarbestilling
		}
	}
	else if (queueValue != 2){
		queue[floor] = type;	//ingen bestilling fra før, setter bestilling opp eller ned
	}
}

void queue_clear() {
	for (int i=0;i<=3;i++) {
		queue_update(i,-1);
	}
}

//Ny etasje. Skal vi stoppe?
int queue_stop(int floor) {
	int queueValue = queue[floor];
	if (floor == 0 || floor == 3){
		return 1;	//vi er i første eller fjerde etasje
	}
	if (queueValue == 2){	//lugarbestilling eller både opp og ned
		return 1;
	}

	else if (queueValue == 1){ 	//bestilling opp
		if (direction == 1){
			return 1;	//bestilling opp og retning opp
		}
		else if (direction == -1){	//sjekker om det er bestillinger nedenfor etasjen
			for (int i = floor - 1; i == 0; i--){
				if (queue[i] != -1){
					return 0;	//bestilling opp, retning ned og bestillinger nedenfor etasjen
				}
			}
		return 1;	//bestilling opp, retning ned og ingen bestillinger nedenfor etasjen
		}
	}

	else if (queueValue == 0){	//bestilling ned
		if (direction == -1){
			return 1;	//bestilling ned og retning ned
		}
		else if (direction == 1){	//sjekker om det er bestillinger ovenfor etasjen
			for (int i = floor + 1; i == 3; i++){
				if (queue[i] != -1){
					return 0;	//bestilling ned, retning opp og bestillinger ovenfor etasjen
				}
			}
		return 1;	//bestilling ned, retning opp og ingen bestillinger ovenfor etasjen
		}
	}
	return 0;
}
