// inc/BMS.h
#ifndef BMS_H
#define BMS_H

#include <array>    // For std::array
#include <string>   // For std::string
#include "../inc/BatteryCell.h"   // For BatteryCell class
#include "../inc/SensorSimulator.h" // For SensorSimulator class
#include "../inc/SafetyManager.h"   // For SafetyManager class
#include "../inc/Constants.h"       // For NUM_CELLS

/**
 * @brief Main Battery Management System class.
 * This class orchestrates the reading of sensor data,
 * evaluation of safety limits, and management of the system state.
 * It's designed to be hardware-agnostic by using an abstract sensor layer.
 */
class BMS {
public:
    /**
     * @brief Constructor for the BMS.
     * Initializes the sensor simulator and safety manager.
     */
    BMS();

    /**
     * @brief Initializes the BMS.
     * Performs any necessary setup for the system.
     */
    void init();

    /**
     * @brief Updates the BMS state.
     * This method reads sensor data, evaluates safety, and updates the system state.
     * It should be called periodically in the main application loop.
     * @param deltaTime_s The time elapsed since the last update in seconds.
     */
    void update(float deltaTime_s);

    /**
     * @brief Gets the current safety state of the BMS.
     * @return The current SystemState.
     */
    SystemState getCurrentState() const;

    /**
     * @brief Gets the current estimated State of Charge (SoC).
     * @return SoC in percentage (0.0 to 100.0).
     */
    float getSoC() const;

    /**
     * @brief Gets the current estimated State of Health (SoH).
     * @return SoH in percentage (0.0 to 100.0).
     */
    float getSoH() const;

    /**
     * @brief Gets the current total pack current.
     * @return Current in Amperes (positive for charge, negative for discharge).
     */
    float getPackCurrent() const;

    /**
     * @brief Checks if the battery is currently charging.
     * @return True if charging, false otherwise.
     */
    bool isCharging() const;

private:
    SensorSimulator m_sensorSimulator;      // Object for simulating sensor readings
    SafetyManager m_safetyManager;          // Object for managing safety states
    std::array<BatteryCell, NUM_CELLS> m_cells; // Array to hold data for each battery cell

    float m_packCurrent;                // Total current of the battery pack (Amperes)
    float m_accumulatedCharge_mAh;      // Accumulated charge in mAh for SoC calculation
    float m_stateOfCharge_percent;      // Estimated State of Charge (%)
    float m_stateOfHealth_percent;      // Estimated State of Health (%)
    float m_chargeCycles;               // Number of full charge cycles
    bool m_wasFull;                     // Flag for SoH cycle counting (was full in previous cycle)
    bool m_wasEmpty;                    // Flag for SoH cycle counting (was empty in previous cycle)
    bool m_isChargingFlag;              // Flag indicating if the battery is currently charging

    /**
     * @brief Updates the State of Charge (SoC) using Coulomb counting.
     * @param deltaTime_s The time elapsed since the last update in seconds.
     */
    void updateSoC(float deltaTime_s);

    /**
     * @brief Updates the State of Health (SoH) using a simplified cycle count.
     */
    void updateSoH();

    /**
     * @brief Logs an event or message to the console.
     * In a real system, this would write to a log file or send over a comms bus.
     * @param message The message to log.
     */
    void logEvent(const std::string& message);

    /**
     * @brief Handles a detected fault.
     * In a real system, this would trigger specific safety actions (e.g., shutdown, isolation).
     * @param faultDescription A description of the fault.
     */
    void handleFault(const std::string& faultDescription);
};

#endif // BMS_H
