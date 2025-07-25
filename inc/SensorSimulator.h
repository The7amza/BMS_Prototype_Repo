// inc/SensorSimulator.h
#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

#include <cstdint> // For uint8_t
#include <random>  // For random number generation
#include "../inc/Constants.h" // For simulation ranges

/**
 * @brief Simulates sensor readings for battery cells and pack current.
 * This class provides a hardware-agnostic way to get sensor data,
 * which can be replaced by real drivers later.
 */
class SensorSimulator {
public:
    /**
     * @brief Constructor for SensorSimulator.
     * Initializes the random number generator.
     */
    SensorSimulator();

    /**
     * @brief Reads a simulated voltage for a given cell ID.
     * @param cellId The ID of the cell to read voltage for.
     * @return Simulated voltage in Volts.
     */
    float readVoltage(uint8_t cellId);

    /**
     * @brief Reads a simulated temperature for a given cell ID.
     * @param cellId The ID of the cell to read temperature for.
     * @return Simulated temperature in Celsius.
     */
    float readTemperature(uint8_t cellId);

    /**
     * @brief Reads a simulated total pack current.
     * @return Simulated current in Amperes (positive for charge, negative for discharge).
     */
    float readCurrent();

private:
    std::mt19937 m_rng; // Mersenne Twister random number generator
    std::uniform_real_distribution<float> m_voltageDist; // Distribution for voltage
    std::uniform_real_distribution<float> m_tempDist;    // Distribution for temperature
    std::uniform_real_distribution<float> m_currentDist; // Distribution for current
    std::uniform_real_distribution<float> m_faultDist;   // Distribution for fault probability
};

#endif // SENSOR_SIMULATOR_H
