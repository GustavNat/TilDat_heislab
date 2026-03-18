#include "orders.h"
#include "driver/elevio.h"

static int orders[N_FLOORS][N_BUTTONS];



void orders_clear_all(void){
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int btn = 0; btn < N_BUTTONS; btn++) {
            orders[floor][btn] = 0;
        }
    }
}


void orders_set(int floor, ButtonType btn){
    if (floor < 0 || floor >= N_FLOORS) return;
    orders[floor][btn] = 1;
}

void orders_clear_at_floor(int floor, MotorDirection dir){
    orders[floor][BUTTON_CAB] = 0;

    if (dir == DIRN_UP) {
        orders[floor][BUTTON_HALL_UP] = 0;
    } else if (dir == DIRN_DOWN) {
        orders[floor][BUTTON_HALL_DOWN] = 0;
    } else { // DIRN_STOP, at turnaround floors
        orders[floor][BUTTON_HALL_UP] = 0;
        orders[floor][BUTTON_HALL_DOWN] = 0;
    }
}

int orders_has_any(void){
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int btn = 0; btn < N_BUTTONS; btn++) {
            if (orders[floor][btn]) {
                return 1;
            }
        }
    }
    return 0;
}

int orders_above(int floor){
    for (int f = floor + 1; f < N_FLOORS; f++) {
        for (int btn = 0; btn < N_BUTTONS; btn++) {
            if (orders[f][btn]) {
                return 1;
            }
        }
    }
    return 0;
}

int orders_below(int floor){
    for (int f = floor - 1; f >= 0; f--) {
        for (int btn = 0; btn < N_BUTTONS; btn++) {
            if (orders[f][btn]) {
                return 1;
            }
        }
    }
    return 0;
}

int orders_at_floor(int floor, MotorDirection dir){
    if (orders[floor][BUTTON_CAB]) {
        return 1;
    }
    if (dir == DIRN_UP && orders[floor][BUTTON_HALL_UP]) {
        return 1;
    }
    if (dir == DIRN_DOWN && orders[floor][BUTTON_HALL_DOWN]) {
        return 1;
    }
    // At turnaround floors, stop for any hall button
    if (dir == DIRN_STOP && (orders[floor][BUTTON_HALL_UP] || orders[floor][BUTTON_HALL_DOWN])) {
        return 1;
    }
    return 0;
}

int orders_get(int floor, ButtonType btn){
    return orders[floor][btn];
}
