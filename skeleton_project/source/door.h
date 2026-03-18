#pragma once
#include "driver/elevio.h"

/**
 * @brief Open the door and start the 3-second timer.
 */
void door_open(void);

/**
 * @brief Close the door.
 */
void door_close(void);

/**
 * @brief Reset the 3-second timer. Call continuously while obstruction is active.
 */
void door_reset_timer(void);

/**
 * @brief Returns 1 if the door is currently open.
 */
int door_is_open(void);

/**
 * @brief Returns 1 if the door is open and the 3-second timer has expired.
 *        The FSM should call door_close() when this returns 1.
 */
int door_is_ready_to_close(void);
