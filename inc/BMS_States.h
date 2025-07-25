// inc/BMS_States.h
#ifndef BMS_STATES_H
#define BMS_STATES_H

/**
 * @brief Defines the possible operational states of the BMS.
 *
 * NORMAL: All parameters are within safe operating limits.
 * WARNING: One or more parameters are outside normal limits but not yet critical.
 * Requires attention but system can continue operation.
 * CRITICAL: One or more parameters are at critical levels, requiring immediate action
 * (e.g., severe power limitation, preparation for shutdown).
 * FAULT: A severe, unrecoverable error or hardware malfunction has occurred, or a
 * critical condition persists. System must be shut down or rendered inoperable.
 */
enum class SystemState {
    NORMAL,
    WARNING,
    CRITICAL,
    FAULT
};

#endif // BMS_STATES_H
