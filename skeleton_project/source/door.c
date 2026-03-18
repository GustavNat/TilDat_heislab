#include "door.h"
#include <time.h>

static int door_opened = 0;
static struct timespec timer_started;

void door_open(void){
    door_opened = 1;
    clock_gettime(CLOCK_MONOTONIC, &timer_started);
}

void door_close(void) {
    door_opened = 0;
}

void door_reset_timer(void){
    if (door_opened) {
        clock_gettime(CLOCK_MONOTONIC, &timer_started);
    }
}

int door_is_open(void){
    return door_opened;
}

int door_is_ready_to_close(void){
    if (!door_opened) {
        return 0;
    }
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    double elapsed = (now.tv_sec - timer_started.tv_sec)
                   + (now.tv_nsec - timer_started.tv_nsec) / 1e9;
    return elapsed >= 3.0;
}
