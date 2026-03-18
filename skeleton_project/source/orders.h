#pragma once

#include "driver/elevio.h"

/**
 * @brief Clear all orders in the queue.
 */
void orders_clear_all(void);

/**
 * @brief Register a new order at the given floor and button type.
 */
void orders_set(int floor, ButtonType btn);

/**
 * @brief Clear relevant orders when stopping at a floor.
 *        Always clears BUTTON_CAB. Clears BUTTON_HALL_UP when going up,
 *        BUTTON_HALL_DOWN when going down. At top/bottom floor clears both hall buttons.
 */
void orders_clear_at_floor(int floor, MotorDirection dir);

/**
 * @brief Returns 1 if there are any orders in the queue, 0 otherwise.
 */
int orders_has_any(void);

/**
 * @brief Returns 1 if there is an order above the given floor.
 */
int orders_above(int floor);

/**
 * @brief Returns 1 if there is an order below the given floor.
 */
int orders_below(int floor);

/**
 * @brief Returns 1 if the elevator should stop at this floor given current direction.
 *        Always stops for BUTTON_CAB. Stops for BUTTON_HALL_UP if going up,
 *        BUTTON_HALL_DOWN if going down. At turnaround floors stops for any hall button.
 */
int orders_at_floor(int floor, MotorDirection dir);

/**
 * @brief Returns the raw order value (0 or 1) for a specific floor and button.
 *        Used by lights module to sync button lamps.
 */
int orders_get(int floor, ButtonType btn);
