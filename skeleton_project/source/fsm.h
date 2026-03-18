#pragma once

/**
 * @brief Initialize the elevator.
 *        Drives down to the nearest floor if between floors,
 *        clears all orders, sets floor indicator, then enters IDLE.
 */
void fsm_init(void);

/**
 * @brief Run one iteration of the elevator state machine.
 *        Call this in a tight loop from main().
 *        Handles button polling, motor control, door logic,
 *        stop button, and obstruction in all states.
 */
void fsm_update(void);
