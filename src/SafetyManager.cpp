// src/SafetyManager.cpp
#include "../inc/SafetyManager.h"
#include <iostream> // For printing state transitions

/**
 * @brief Constructor for SafetyManager.
 * Initializes the system state to NORMAL.
 */
SafetyManager::SafetyManager() : m_currentState(SystemState::NORMAL) {}

/**
 * @brief Checks if a given voltage is within the normal operating range.
 * @param voltage The voltage to check.
 * @return True if voltage is normal, false otherwise.
 */
bool SafetyManager::isVoltageNormal(float voltage) const {
    return voltage >= MIN_VOLTAGE_NORMAL && voltage <= MAX_VOLTAGE_NORMAL;
}

/**
 * @brief Checks if a given voltage is in the warning range.
 * @param voltage The voltage to check.
 * @return True if voltage is in warning, false otherwise.
 */
bool SafetyManager::isVoltageWarning(float voltage) const {
    return (voltage >= MIN_VOLTAGE_WARNING && voltage < MIN_VOLTAGE_NORMAL) ||
           (voltage > MAX_VOLTAGE_NORMAL && voltage <= MAX_VOLTAGE_WARNING);
}

/**
 * @brief Checks if a given voltage is in the critical range.
 * @param voltage The voltage to check.
 * @return True if voltage is critical, false otherwise.
 */
bool SafetyManager::isVoltageCritical(float voltage) const {
    return (voltage >= MIN_VOLTAGE_CRITICAL && voltage < MIN_VOLTAGE_WARNING) ||
           (voltage > MAX_VOLTAGE_WARNING && voltage <= MAX_VOLTAGE_CRITICAL);
}

/**
 * @brief Checks if a given voltage is in the fault range (e.g., sensor error).
 * @param voltage The voltage to check.
 * @return True if voltage is fault, false otherwise.
 */
bool SafetyManager::isVoltageFault(float voltage) const {
    return voltage < MIN_VOLTAGE_FAULT || voltage > MAX_VOLTAGE_FAULT;
}

/**
 * @brief Checks if a given temperature is within the normal operating range.
 * @param temperature The temperature to check.
 * @return True if temperature is normal, false otherwise.
 */
bool SafetyManager::isTemperatureNormal(float temperature) const {
    return temperature >= MIN_TEMP_NORMAL && temperature <= MAX_TEMP_NORMAL;
}

/**
 * @brief Checks if a given temperature is in the warning range.
 * @param temperature The temperature to check.
 * @return True if temperature is in warning, false otherwise.
 */
bool SafetyManager::isTemperatureWarning(float temperature) const {
    return (temperature >= MIN_TEMP_WARNING && temperature < MIN_TEMP_NORMAL) ||
           (temperature > MAX_TEMP_NORMAL && temperature <= MAX_TEMP_WARNING);
}

/**
 * @brief Checks if a given temperature is in the critical range.
 * @param temperature The temperature to check.
 * @return True if temperature is critical, false otherwise.
 */
bool SafetyManager::isTemperatureCritical(float temperature) const {
    return (temperature >= MIN_TEMP_CRITICAL && temperature < MIN_TEMP_WARNING) ||
           (temperature > MAX_TEMP_WARNING && temperature <= MAX_TEMP_CRITICAL);
}

/**
 * @brief Checks if a given temperature is in the fault range.
 * @param temperature The temperature to check.
 * @return True if temperature is fault, false otherwise.
 */
bool SafetyManager::isTemperatureFault(float temperature) const {
    return temperature < MIN_TEMP_FAULT || temperature > MAX_TEMP_FAULT;
}

/**
 * @brief Checks if a given current is within the normal operating range.
 * @param current The current to check.
 * @return True if current is normal, false otherwise.
 */
bool SafetyManager::isCurrentNormal(float current) const {
    if (current > IDLE_CURRENT_THRESHOLD_A) { // Charging
        return current <= MAX_CHARGE_CURRENT_NORMAL_A;
    } else if (current < -IDLE_CURRENT_THRESHOLD_A) { // Discharging
        return current >= -MAX_DISCHARGE_CURRENT_NORMAL_A;
    } else { // Idle
        return true;
    }
}

/**
 * @brief Checks if a given current is in the warning range.
 * @param current The current to check.
 * @return True if current is in warning, false otherwise.
 */
bool SafetyManager::isCurrentWarning(float current) const {
    if (current > IDLE_CURRENT_THRESHOLD_A) { // Charging
        return current > MAX_CHARGE_CURRENT_NORMAL_A && current <= MAX_CHARGE_CURRENT_WARNING_A;
    } else if (current < -IDLE_CURRENT_THRESHOLD_A) { // Discharging
        return current < -MAX_DISCHARGE_CURRENT_NORMAL_A && current >= -MAX_DISCHARGE_CURRENT_WARNING_A;
    }
    return false; // Idle current is not a warning
}

/**
 * @brief Checks if a given current is in the critical range.
 * @param current The current to check.
 * @return True if current is critical, false otherwise.
 */
bool SafetyManager::isCurrentCritical(float current) const {
    if (current > IDLE_CURRENT_THRESHOLD_A) { // Charging
        return current > MAX_CHARGE_CURRENT_WARNING_A && current <= MAX_CHARGE_CURRENT_CRITICAL_A;
    } else if (current < -IDLE_CURRENT_THRESHOLD_A) { // Discharging
        return current < -MAX_DISCHARGE_CURRENT_WARNING_A && current >= -MAX_DISCHARGE_CURRENT_CRITICAL_A;
    }
    return false;
}

/**
 * @brief Checks if the State of Health is normal.
 * @param soh The SoH percentage.
 * @return True if SoH is normal, false otherwise.
 */
bool SafetyManager::isSoHNormal(float soh) const {
    return soh >= SOH_THRESHOLD_WARNING;
}

/**
 * @brief Checks if the State of Health is in warning.
 * @param soh The SoH percentage.
 * @return True if SoH is in warning, false otherwise.
 */
bool SafetyManager::isSoHWarning(float soh) const {
    return soh >= SOH_THRESHOLD_CRITICAL && soh < SOH_THRESHOLD_WARNING;
}

/**
 * @brief Checks if the State of Health is critical.
 * @param soh The SoH percentage.
 * @return True if SoH is critical, false otherwise.
 */
bool SafetyManager::isSoHCritical(float soh) const {
    return soh < SOH_THRESHOLD_CRITICAL;
}

/**
 * @brief Evaluates the current state of the battery cells and pack current and updates the system state.
 * This is the core logic for determining the BMS's safety status.
 * @param cells An array of BatteryCell objects representing the current battery pack data.
 * @param packCurrent The total current flowing through the battery pack (Amperes).
 * @param stateOfHealth_percent The current estimated State of Health of the battery pack (%).
 */
void SafetyManager::evaluate(const std::array<BatteryCell, NUM_CELLS>& cells, float packCurrent, float stateOfHealth_percent) {
    SystemState proposedState = SystemState::NORMAL;

    // Check for FAULT conditions first (most severe)
    for (const auto& cell : cells) {
        if (isVoltageFault(cell.getVoltage()) || isTemperatureFault(cell.getTemperature())) {
            proposedState = SystemState::FAULT;
            break; // Immediate FAULT, no need to check further
        }
    }

    if (proposedState != SystemState::FAULT) {
        // Check for CRITICAL conditions
        if (isCurrentCritical(packCurrent) || isSoHCritical(stateOfHealth_percent)) {
            proposedState = SystemState::CRITICAL;
        } else {
            for (const auto& cell : cells) {
                if (isVoltageCritical(cell.getVoltage()) || isTemperatureCritical(cell.getTemperature())) {
                    proposedState = SystemState::CRITICAL;
                    break;
                }
            }
        }
    }

    if (proposedState == SystemState::NORMAL) { // Only check for WARNING if not already CRITICAL or FAULT
        // Check for WARNING conditions
        if (isCurrentWarning(packCurrent) || isSoHWarning(stateOfHealth_percent)) {
            proposedState = SystemState::WARNING;
        } else {
            for (const auto& cell : cells) {
                if (isVoltageWarning(cell.getVoltage()) || isTemperatureWarning(cell.getTemperature())) {
                    proposedState = SystemState::WARNING;
                    break;
                }
            }
        }
    }

    // Update state and print transition if changed
    if (proposedState != m_currentState) {
        std::cout << "--- BMS STATE TRANSITION: ";
        switch (m_currentState) {
            case SystemState::NORMAL: std::cout << "NORMAL"; break;
            case SystemState::WARNING: std::cout << "WARNING"; break;
            case SystemState::CRITICAL: std::cout << "CRITICAL"; break;
            case SystemState::FAULT: std::cout << "FAULT"; break;
        }
        std::cout << " -> ";
        switch (proposedState) {
            case SystemState::NORMAL: std::cout << "NORMAL"; break;
            case SystemState::WARNING: std::cout << "WARNING"; break;
            case SystemState::CRITICAL: std::cout << "CRITICAL"; break;
            case SystemState::FAULT: std::cout << "FAULT"; break;
        }
        std::cout << " ---" << std::endl;
        m_currentState = proposedState;
    }
}

/**
 * @brief Gets the current safety state of the BMS.
 * @return The current SystemState.
 */
SystemState SafetyManager::getCurrentState() const {
    return m_currentState;
}
