// src/SensorSimulator.cpp
#include "../inc/SensorSimulator.h"
#include <chrono>   // For seeding the random number generator
#include <iostream> // For printing simulation messages

/**
 * @brief Constructor for SensorSimulator.
 * Initializes the random number generator with a time-based seed.
 */
SensorSimulator::SensorSimulator()
    : m_rng(std::chrono::high_resolution_clock::now().time_since_epoch().count()),
      m_voltageDist(SIM_VOLTAGE_MIN, SIM_VOLTAGE_MAX),
      m_tempDist(SIM_TEMP_MIN, SIM_TEMP_MAX),
      m_currentDist(SIM_CURRENT_MIN, SIM_CURRENT_MAX),
      m_faultDist(0.0f, 1.0f) {}

/**
 * @brief Reads a simulated voltage for a given cell ID.
 * Introduces occasional out-of-bounds readings for fault simulation.
 * @param cellId The ID of the cell to read voltage for.
 * @return Simulated voltage in Volts.
 */
float SensorSimulator::readVoltage(uint8_t cellId) {
    float voltage = m_voltageDist(m_rng);

    // Introduce a fault sometimes
    if (m_faultDist(m_rng) < SIM_FAULT_PROBABILITY) {
        float fault_val = m_faultDist(m_rng);
        if (fault_val < 0.33f) { // Low critical
            voltage = MIN_VOLTAGE_CRITICAL - (m_faultDist(m_rng) * 0.2f);
            std::cout << "[SIM] Cell " << (int)cellId << " - Low Voltage Fault Injected (Critical)!" << std::endl;
        } else if (fault_val < 0.66f) { // High critical
            voltage = MAX_VOLTAGE_CRITICAL + (m_faultDist(m_rng) * 0.2f);
            std::cout << "[SIM] Cell " << (int)cellId << " - High Voltage Fault Injected (Critical)!" << std::endl;
        } else { // Extreme fault (e.g., sensor disconnect)
            voltage = (m_faultDist(m_rng) < 0.5f) ? MIN_VOLTAGE_FAULT - 0.1f : MAX_VOLTAGE_FAULT + 0.1f;
            std::cout << "[SIM] Cell " << (int)cellId << " - Extreme Voltage Fault Injected (Sensor Error)!" << std::endl;
        }
    }
    return voltage;
}

/**
 * @brief Reads a simulated temperature for a given cell ID.
 * Introduces occasional out-of-bounds readings for fault simulation.
 * @param cellId The ID of the cell to read temperature for.
 * @return Simulated temperature in Celsius.
 */
float SensorSimulator::readTemperature(uint8_t cellId) {
    float temperature = m_tempDist(m_rng);

    // Introduce a fault sometimes
    if (m_faultDist(m_rng) < SIM_FAULT_PROBABILITY) {
        float fault_val = m_faultDist(m_rng);
        if (fault_val < 0.33f) { // Low critical
            temperature = MIN_TEMP_CRITICAL - (m_faultDist(m_rng) * 5.0f);
            std::cout << "[SIM] Cell " << (int)cellId << " - Low Temperature Fault Injected (Critical)!" << std::endl;
        } else if (fault_val < 0.66f) { // High critical
            temperature = MAX_TEMP_CRITICAL + (m_faultDist(m_rng) * 5.0f);
            std::cout << "[SIM] Cell " << (int)cellId << " - High Temperature Fault Injected (Critical)!" << std::endl;
        } else { // Extreme fault
            temperature = (m_faultDist(m_rng) < 0.5f) ? MIN_TEMP_FAULT - 1.0f : MAX_TEMP_FAULT + 1.0f;
            std::cout << "[SIM] Cell " << (int)cellId << " - Extreme Temperature Fault Injected (Sensor Error)!" << std::endl;
        }
    }
    return temperature;
}

/**
 * @brief Reads a simulated total pack current.
 * Introduces occasional out-of-bounds readings for fault simulation.
 * @return Simulated current in Amperes (positive for charge, negative for discharge).
 */
float SensorSimulator::readCurrent() {
    float current = m_currentDist(m_rng);

    // Introduce a fault sometimes
    if (m_faultDist(m_rng) < SIM_FAULT_PROBABILITY) {
        float fault_val = m_faultDist(m_rng);
        if (fault_val < 0.33f) { // High discharge critical
            current = -(MAX_DISCHARGE_CURRENT_CRITICAL_A + (m_faultDist(m_rng) * 5.0f));
            std::cout << "[SIM] Pack - High Discharge Current Fault Injected (Critical)!" << std::endl;
        } else if (fault_val < 0.66f) { // High charge critical
            current = MAX_CHARGE_CURRENT_CRITICAL_A + (m_faultDist(m_rng) * 1.0f);
            std::cout << "[SIM] Pack - High Charge Current Fault Injected (Critical)!" << std::endl;
        } else { // Extreme current (e.g., sensor error)
            current = (m_faultDist(m_rng) < 0.5f) ? -50.0f : 10.0f; // Very large positive/negative
            std::cout << "[SIM] Pack - Extreme Current Fault Injected (Sensor Error)!" << std::endl;
        }
    }
    return current;
}
