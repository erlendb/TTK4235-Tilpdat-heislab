#include <time.h>

int doorTimer = -1;
int timeLimit = 3;

void timer_start() {
	doorTimer = time(NULL);
}

int timer_check() {
	if (doorTimer == -1) return 0;
	else return (time(NULL) - doorTimer > timeLimit);
}

void timer_deactivate() {
	doorTimer = -1;
}

int timer_is_activated() {
	return (doorTimer != -1);
}
