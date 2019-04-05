
#include <time.h>

int doorTimer = -1;
int timeLimit = 3;

void timer_start() {
	doorTimer = time(NULL);
}

int timer_check() {
	return (time(NULL) - doorTimer > timeLimit);
}

void timer_deactivate() {
	doorTimer = -1;
}

int timer_is_activated() {
	return (doorTimer != -1);
}
