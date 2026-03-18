#include "fsm.h"
#include "orders.h"
#include "lights.h"
#include "door.h"
#include "driver/elevio.h"

typedef enum {
    STATE_INIT,
    STATE_IDLE,
    STATE_MOVING,
    STATE_DOOR_OPEN,
    STATE_STOP
} ElevatorState;

static ElevatorState  state = STATE_INIT;
static MotorDirection current_direction = DIRN_STOP;
static int last_floor = -1;


static void fsm_handle_idle(void);
static void fsm_handle_moving(void);
static void fsm_handle_door_open(void);
static void fsm_handle_stop(void);
static MotorDirection fsm_choose_direction(void);

static void poll_buttons(void){
    int any_new = 0;
    for (int f = 0; f < N_FLOORS; f++) {
        for (int b = 0; b < N_BUTTONS; b++) {
            if (elevio_callButton(f, b)) {
                orders_set(f, b);
                any_new = 1;
            }
        }
    }
    if (any_new) {
        lights_update_button_lamps();
    }
}

void fsm_init(void){
    orders_clear_all();
    lights_update_button_lamps();
    lights_set_stop_lamp(0);
    lights_set_door_lamp(0);

    if (elevio_floorSensor() == -1) {
        elevio_motorDirection(DIRN_DOWN);
        while (elevio_floorSensor() == -1);
    }

    last_floor = elevio_floorSensor();
    current_direction = DIRN_STOP;
    elevio_motorDirection(DIRN_STOP);
    lights_set_floor_indicator(last_floor);

    state = STATE_IDLE;
}

void fsm_update(void){
    if (elevio_stopButton() && state != STATE_STOP) {
        elevio_motorDirection(DIRN_STOP);
        orders_clear_all();
        lights_update_button_lamps();
        lights_set_stop_lamp(1);

        if (elevio_floorSensor() != -1) {
            door_open();
            lights_set_door_lamp(1);
        }
        state = STATE_STOP;
        return;
    }

    switch (state) {
        case STATE_IDLE:      fsm_handle_idle();      break;
        case STATE_MOVING:    fsm_handle_moving();    break;
        case STATE_DOOR_OPEN: fsm_handle_door_open(); break;
        case STATE_STOP:      fsm_handle_stop();      break;
        default:              break;
    }
}

static void fsm_handle_idle(void){
    poll_buttons();

    if (!orders_has_any()) return;

    if (orders_at_floor(last_floor, current_direction)) {
        orders_clear_at_floor(last_floor, current_direction);
        lights_update_button_lamps();
        door_open();
        lights_set_door_lamp(1);
        state = STATE_DOOR_OPEN;
        return;
    }

    current_direction = fsm_choose_direction();
    if (current_direction == DIRN_STOP) {
        state = STATE_IDLE;
        return;
    }
    elevio_motorDirection(current_direction);
    state = STATE_MOVING;
}

static void fsm_handle_moving(void){
    poll_buttons();

    int floor = elevio_floorSensor();
    if (floor != -1) {
        last_floor = floor;
        lights_set_floor_indicator(last_floor);

        // Boundary guard — stop if no orders beyond this floor
        if (floor == 0 && current_direction == DIRN_DOWN && !orders_below(floor)) {
            elevio_motorDirection(DIRN_STOP);
            current_direction = DIRN_STOP;
            if (orders_at_floor(floor, DIRN_STOP)) {
                orders_clear_at_floor(floor, DIRN_STOP);
                lights_update_button_lamps();
                door_open();
                lights_set_door_lamp(1);
                state = STATE_DOOR_OPEN;
            } else {
                state = STATE_IDLE;
            }
            return;
        }
        if (floor == N_FLOORS - 1 && current_direction == DIRN_UP && !orders_above(floor)) {
            elevio_motorDirection(DIRN_STOP);
            current_direction = DIRN_STOP;
            if (orders_at_floor(floor, DIRN_STOP)) {
                orders_clear_at_floor(floor, DIRN_STOP);
                lights_update_button_lamps();
                door_open();
                lights_set_door_lamp(1);
                state = STATE_DOOR_OPEN;
            } else {
                state = STATE_IDLE;
            }
            return;
        }
    }   
    if (floor != -1 && orders_at_floor(floor, current_direction)) {
        elevio_motorDirection(DIRN_STOP);
        orders_clear_at_floor(floor, current_direction);
        lights_update_button_lamps();
        door_open();
        lights_set_door_lamp(1);
        state = STATE_DOOR_OPEN;
    }
}

static void fsm_handle_door_open(void){
    poll_buttons();

    if (elevio_obstruction()) {
        door_reset_timer();
    }

    if (!door_is_ready_to_close()) return;

    door_close();
    lights_set_door_lamp(0);

    if (orders_has_any()) {
        current_direction = fsm_choose_direction();
        elevio_motorDirection(current_direction);
        state = STATE_MOVING;
    } else {
        state = STATE_IDLE;
    }
}

static void fsm_handle_stop(void){
    lights_set_stop_lamp(elevio_stopButton());

    if (door_is_open() && elevio_obstruction()) {
        door_reset_timer();
    }

    if (elevio_stopButton()) return;

    lights_set_stop_lamp(0);

    if (door_is_open()) {
        door_reset_timer();
        state = STATE_DOOR_OPEN;
    } else {
        state = STATE_IDLE;
    }
}

static MotorDirection fsm_choose_direction(void){
    if (current_direction == DIRN_UP) {
        if (orders_above(last_floor)) return DIRN_UP;
        if (orders_below(last_floor)) return DIRN_DOWN;
    }
    if (current_direction == DIRN_DOWN) {
        if (orders_below(last_floor)) return DIRN_DOWN;
        if (orders_above(last_floor)) return DIRN_UP;
    }
    if (orders_above(last_floor)) return DIRN_UP;
    if (orders_below(last_floor)) return DIRN_DOWN;
    return DIRN_STOP;
}
