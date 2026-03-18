#include "lights.h"
#include "orders.h"
#include "driver/elevio.h"

void lights_update_button_lamps(void){
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int btn = 0; btn < N_BUTTONS; btn++) {
            if (btn == BUTTON_HALL_UP   && floor == N_FLOORS - 1) continue;
            if (btn == BUTTON_HALL_DOWN && floor == 0)             continue;
            int value = orders_get(floor, btn);
            elevio_buttonLamp(floor, (ButtonType)btn, value);
        }
    }
}

void lights_set_floor_indicator(int floor){
    elevio_floorIndicator(floor);
}

void lights_set_door_lamp(int value){
    elevio_doorOpenLamp(value);
}

void lights_set_stop_lamp(int value){
    elevio_stopLamp(value);
}
