// src/BMS.cpp
#include "../inc/BMS.h"
#include <iostream> // For printing to console
#include <iomanip>  // For formatting output
#include <numeric>  // For std::accumulate (if needed for average voltage/temp)

/**
 * @brief Constructor for the BMS.
 * Initializes the sensor simulator and safety manager.
 */
BMS::BMS()
    : m_packCurrent(0.0f),
      m_accumulatedCharge_mAh(NOMINAL_CAPACITY_MAH * 0.5f), // Start at 50% SoC for simulation
      m_stateOfCharge_percent(50.0f),
      m_stateOfHealth_percent(100.0f),
      m_chargeCycles(0.0f),
      m_wasFull(false),
      m_wasEmpty(false),
      m_isChargingFlag(false)
{
    // Initialize BatteryCell objects in the array
    for (uint8_t i = 0; i < NUM_CELLS; ++i) {
        m_cells[i] = BatteryCell(i, 0.0f, 0.0f); // Initialize with ID and dummy values
    }
}

/**
 * @brief Initializes the BMS.
 * Performs any necessary setup for the system.
 */
void BMS::init() {
    logEvent("BMS initialized with " + std::to_string(NUM_CELLS) + " cells.");
    logEvent("Initial state: NORMAL");
    logEvent("Initial SoC: " + std::to_string(static_cast<int>(m_stateOfCharge_percent)) + "%");
    logEvent("Initial SoH: " + std::to_string(static_cast<int>(m_stateOfHealth_percent)) + "%");
}

/**
 * @brief Updates the State of Charge (SoC) using Coulomb counting.
 * @param deltaTime_s The time elapsed since the last update in seconds.
 */
void BMS::updateSoC(float deltaTime_s) {
    // Current is in Amperes, convert to milliamperes (mA)
    float current_mA = m_packCurrent * 1000.0f;

    // Calculate charge change in mAh
    // Q = I * t (mAh = mA * hours)
    // deltaTime_s is in seconds, convert to hours by dividing by 3600
    float charge_change_mAh = current_mA * (deltaTime_s / 3600.0f);

    // Apply charge efficiency if charging
    if (current_mA > IDLE_CURRENT_THRESHOLD_A * 1000.0f) { // If charging
        charge_change_mAh *= CHARGE_EFFICIENCY;
    }

    m_accumulatedCharge_mAh += charge_change_mAh;

    // Clamp accumulated charge to nominal capacity (representing 0% to 100% physically)
    if (m_accumulatedCharge_mAh > NOMINAL_CAPACITY_MAH) {
        m_accumulatedCharge_mAh = NOMINAL_CAPACITY_MAH;
    } else if (m_accumulatedCharge_mAh < 0.0f) {
        m_accumulatedCharge_mAh = 0.0f;
    }

    // Calculate SoC percentage
    m_stateOfCharge_percent = (m_accumulatedCharge_mAh / NOMINAL_CAPACITY_MAH) * 100.0f;

    // Ensure SoC is within 0-100%
    if (m_stateOfCharge_percent > 100.0f) m_stateOfCharge_percent = 100.0f;
    if (m_stateOfCharge_percent < 0.0f) m_stateOfCharge_percent = 0.0f;
}

/**
 * @brief Updates the State of Health (SoH) using a simplified cycle count.
 * A full cycle is counted when the battery goes from below SOC_EMPTY_THRESHOLD to above SOC_FULL_THRESHOLD.
 */
void BMS::updateSoH() {
    if (m_stateOfCharge_percent >= SOC_FULL_THRESHOLD_PERCENT) {
        m_wasFull = true;
    }
    if (m_stateOfCharge_percent <= SOC_EMPTY_THRESHOLD_PERCENT) {
        m_wasEmpty = true;
    }

    // If it was full and now it's empty, or vice-versa, it's a full cycle
    if (m_wasFull && m_wasEmpty) {
        m_chargeCycles += 0.5f; // Count half a cycle for each transition
        m_wasFull = false;
        m_wasEmpty = false;
        logEvent("Charge cycle incremented. Total cycles: " + std::to_string(m_chargeCycles));
    }

    // Simplified SoH degradation: 0.1% degradation per full cycle
    // In a real system, this would be much more complex (e.g., based on temperature, current, depth of discharge)
    m_stateOfHealth_percent = 100.0f - (m_chargeCycles * 0.1f);

    // Clamp SoH to 0-100%
    if (m_stateOfHealth_percent > 100.0f) m_stateOfHealth_percent = 100.0f;
    if (m_stateOfHealth_percent < 0.0f) m_stateOfHealth_percent = 0.0f;
}

/**
 * @brief Logs an event or message to the console.
 * In a real system, this would write to a log file or send over a comms bus.
 * @param message The message to log.
 */
void BMS::logEvent(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}

/**
 * @brief Handles a detected fault.
 * In a real system, this would trigger specific safety actions (e.g., shutdown, isolation).
 * @param faultDescription A description of the fault.
 */
void BMS::handleFault(const std::string& faultDescription) {
    std::cerr << "[FAULT] " << faultDescription << " - Immediate action required!" << std::endl;
    // In a real system:
    // - Trigger hardware shutdown
    // - Isolate battery pack
    // - Store detailed fault logs in non-volatile memory
    // - Activate warning lights/buzzers
    // - Notify external system (e.g., vehicle ECU)
}

/**
 * @brief Updates the BMS state.
 * This method reads sensor data, evaluates safety, and updates the system state.
 * It should be called periodically in the main application loop.
 * @param deltaTime_s The time elapsed since the last update in seconds.
 */
void BMS::update(float deltaTime_s) {
    // 1. Read sensor data for each cell and pack current
    std::cout << "\n--- Reading Sensor Data ---" << std::endl;
    for (uint8_t i = 0; i < NUM_CELLS; ++i) {
        float voltage = m_sensorSimulator.readVoltage(i);
        float temperature = m_sensorSimulator.readTemperature(i);

        m_cells[i].setVoltage(voltage);
        m_cells[i].setTemperature(temperature);

        std::cout << "Cell " << (int)i << ": Voltage = "
                  << std::fixed << std::setprecision(3) << voltage << "V, Temperature = "
                  << std::fixed << std::setprecision(1) << temperature << "C" << std::endl;
    }
    m_packCurrent = m_sensorSimulator.readCurrent();
    std::cout << "Pack Current: " << std::fixed << std::setprecision(2) << m_packCurrent << "A" << std::endl;

    // Determine charging state
    if (m_packCurrent > IDLE_CURRENT_THRESHOLD_A) {
        m_isChargingFlag = true;
    } else if (m_packCurrent < -IDLE_CURRENT_THRESHOLD_A) {
        m_isChargingFlag = false; // Discharging
    }
    // If current is near zero (idle), m_isChargingFlag retains its last state or could be set to false

    // 2. Update SoC and SoH
    updateSoC(deltaTime_s);
    updateSoH();

    // 3. Evaluate safety based on current cell data, pack current, and SoH
    m_safetyManager.evaluate(m_cells, m_packCurrent, m_stateOfHealth_percent);

    // 4. Handle state-specific actions
    SystemState currentState = m_safetyManager.getCurrentState();
    switch (currentState) {
        case SystemState::NORMAL:
            logEvent("BMS operating normally.");
            // No specific actions needed, perhaps enable full power
            break;
        case SystemState::WARNING:
            logEvent("BMS in WARNING state. Check parameters!");
            // Reduce power output, send warning to user/system
            break;
        case SystemState::CRITICAL:
            logEvent("BMS in CRITICAL state. Prepare for shutdown or severe limitation!");
            // Severely limit power, prepare for emergency shutdown, log critical event
            break;
        case SystemState::FAULT:
            handleFault("BMS entered FAULT state due to critical sensor reading or persistent issue.");
            // Trigger immediate shutdown, isolate battery
            break;
    }

    // 5. Print current system status
    std::cout << "Current BMS State: ";
    switch (currentState) {
        case SystemState::NORMAL:   std::cout << "NORMAL"; break;
        case SystemState::WARNING:  std::cout << "WARNING"; break;
        case SystemState::CRITICAL: std::cout << "CRITICAL"; break;
        case SystemState::FAULT:    std::cout << "FAULT"; break;
    }
    std::cout << " | SoC: " << std::fixed << std::setprecision(1) << m_stateOfCharge_percent << "%";
    std::cout << " | SoH: " << std::fixed << std::setprecision(1) << m_stateOfHealth_percent << "%";
    std::cout << " | Charging: " << (m_isChargingFlag ? "YES" : "NO") << std::endl;
}

/**
 * @brief Gets the current safety state of the BMS.
 * @return The current SystemState.
 */
SystemState BMS::getCurrentState() const {
    return m_safetyManager.getCurrentState();
}

/**
 * @brief Gets the current estimated State of Charge (SoC).
 * @return SoC in percentage (0.0 to 100.0).
 */
float BMS::getSoC() const {
    return m_stateOfCharge_percent;
}

/**
 * @brief Gets the current estimated State of Health (SoH).
 * @return SoH in percentage (0.0 to 100.0).
 */
float BMS::getSoH() const {
    return m_stateOfHealth_percent;
}

/**
 * @brief Gets the current total pack current.
 * @return Current in Amperes (positive for charge, negative for discharge).
 */
float BMS::getPackCurrent() const {
    return m_packCurrent;
}

/**
 * @brief Checks if the battery is currently charging.
 * @return True if charging, false otherwise.
 */
bool BMS::isCharging() const {
    return m_isChargingFlag;
}
