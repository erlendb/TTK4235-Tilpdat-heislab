#include <time.h>

#define TIME_LIMIT 3

int doorTimer = -1;

void timer_start() {
	doorTimer = time(NULL);
}

int timer_check() {
	if (doorTimer == -1) return 0;
	else return (time(NULL) - doorTimer > TIME_LIMIT);
}
