#include <time.h>

#define TIME_LIMIT 3

int doorTimer = -1;

void timer_start() {
	doorTimer = time(NULL);
}

int timer_exceeds_limit() {
	if (doorTimer == -1) return 0;
	else return (time(NULL) - doorTimer > TIME_LIMIT);
}

void timer_clear() {
	doorTimer = -1;
}

int timer_is_started() {
	return (doorTimer != -1);
}
