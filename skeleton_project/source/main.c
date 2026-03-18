#include <stdlib.h>
#include <time.h>
#include "driver/elevio.h"
#include "fsm.h"

int main(void) {
    elevio_init();
    fsm_init();

    while (1) {
        fsm_update();   
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}