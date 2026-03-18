#pragma once
#include "driver/elevio.h"

/**
 * @brief Sync all button lamps with the current order queue.
 *        Call after any order is added or cleared.
 *        Note: BUTTON_HALL_DOWN lamp is never lit on floor 0,
 *              BUTTON_HALL_UP lamp is never lit on the top floor.
 */
void lights_update_button_lamps(void);

/**
 * @brief Set the floor indicator to the given floor.
 */
void lights_set_floor_indicator(int floor);

/**
 * @brief Set the door open lamp (1 = on, 0 = off).
 */
void lights_set_door_lamp(int value);

/**
 * @brief Set the stop lamp (1 = on, 0 = off).
 */
void lights_set_stop_lamp(int value);
