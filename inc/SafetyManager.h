// inc/SafetyManager.h
#ifndef SAFETY_MANAGER_H
#define SAFETY_MANAGER_H

#include <array>     // For std::array
#include "../inc/BMS_States.h"    // For SystemState enum
#include "../inc/BatteryCell.h"   // For BatteryCell class
#include "../inc/Constants.h"     // For NUM_CELLS and limits

/**
 * @brief Manages the safety state of the BMS based on battery cell parameters and pack current.
 * This class evaluates cell voltages, temperatures, and pack current against defined limits
 * and transitions the system through different safety states (NORMAL, WARNING, CRITICAL, FAULT).
 */
class SafetyManager {
public:
    /**
     * @brief Constructor for SafetyManager.
     * Initializes the system state to NORMAL.
     */
    SafetyManager();

    /**
     * @brief Evaluates the current state of the battery cells and pack current and updates the system state.
     * This is the core logic for determining the BMS's safety status.
     * @param cells An array of BatteryCell objects representing the current battery pack data.
     * @param packCurrent The total current flowing through the battery pack (Amperes).
     * @param stateOfHealth_percent The current estimated State of Health of the battery pack (%).
     */
    void evaluate(const std::array<BatteryCell, NUM_CELLS>& cells, float packCurrent, float stateOfHealth_percent);

    /**
     * @brief Gets the current safety state of the BMS.
     * @return The current SystemState.
     */
    SystemState getCurrentState() const;

private:
    SystemState m_currentState; // The current safety state of the BMS

    /**
     * @brief Checks if a given voltage is within the normal operating range.
     * @param voltage The voltage to check.
     * @return True if voltage is normal, false otherwise.
     */
    bool isVoltageNormal(float voltage) const;

    /**
     * @brief Checks if a given voltage is in the warning range.
     * @param voltage The voltage to check.
     * @return True if voltage is in warning, false otherwise.
     */
    bool isVoltageWarning(float voltage) const;

    /**
     * @brief Checks if a given voltage is in the critical range.
     * @param voltage The voltage to check.
     * @return True if voltage is critical, false otherwise.
     */
    bool isVoltageCritical(float voltage) const;

    /**
     * @brief Checks if a given voltage is in the fault range (e.g., sensor error).
     * @param voltage The voltage to check.
     * @return True if voltage is fault, false otherwise.
     */
    bool isVoltageFault(float voltage) const;

    /**
     * @brief Checks if a given temperature is within the normal operating range.
     * @param temperature The temperature to check.
     * @return True if temperature is normal, false otherwise.
     */
    bool isTemperatureNormal(float temperature) const;

    /**
     * @brief Checks if a given temperature is in the warning range.
     * @param temperature The temperature to check.
     * @return True if temperature is in warning, false otherwise.
     */
    bool isTemperatureWarning(float temperature) const;

    /**
     * @brief Checks if a given temperature is in the critical range.
     * @param temperature The temperature to check.
     * @return True if temperature is critical, false otherwise.
     */
    bool isTemperatureCritical(float temperature) const;

    /**
     * @brief Checks if a given temperature is in the fault range.
     * @param temperature The temperature to check.
     * @return True if temperature is fault, false otherwise.
     */
    bool isTemperatureFault(float temperature) const;

    /**
     * @brief Checks if a given current is within the normal operating range.
     * @param current The current to check.
     * @return True if current is normal, false otherwise.
     */
    bool isCurrentNormal(float current) const;

    /**
     * @brief Checks if a given current is in the warning range.
     * @param current The current to check.
     * @return True if current is in warning, false otherwise.
     */
    bool isCurrentWarning(float current) const;

    /**
     * @brief Checks if a given current is in the critical range.
     * @param current The current to check.
     * @return True if current is critical, false otherwise.
     */
    bool isCurrentCritical(float current) const;

    /**
     * @brief Checks if the State of Health is normal.
     * @param soh The SoH percentage.
     * @return True if SoH is normal, false otherwise.
     */
    bool isSoHNormal(float soh) const;

    /**
     * @brief Checks if the State of Health is in warning.
     * @param soh The SoH percentage.
     * @return True if SoH is in warning, false otherwise.
     */
    bool isSoHWarning(float soh) const;

    /**
     * @brief Checks if the State of Health is critical.
     * @param soh The SoH percentage.
     * @return True if SoH is critical, false otherwise.
     */
    bool isSoHCritical(float soh) const;
};

#endif // SAFETY_MANAGER_H
