
#include <time.h>

int doorTimer = -1;

void timer_start() {
	doorTimer = time(NULL);
}

int timer_exceeds_threshold() {
	return (time(NULL) - doorTimer > 3);
}

void timer_deactivate() {
	doorTimer = -1;
}

int timer_is_activated() {
	return (doorTimer != -1);
}
