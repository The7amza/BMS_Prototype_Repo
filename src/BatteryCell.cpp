// src/BatteryCell.cpp
#include "../inc/BatteryCell.h"

/**
 * @brief Default constructor for BatteryCell.
 * Initializes the cell with default/dummy values.
 */
BatteryCell::BatteryCell()
    : m_id(0), m_voltage(0.0f), m_temperature(0.0f) {}

/**
 * @brief Constructor for BatteryCell.
 * @param id Unique identifier for the cell.
 * @param voltage Initial voltage of the cell.
 * @param temperature Initial temperature of the cell.
 */
BatteryCell::BatteryCell(uint8_t id, float voltage, float temperature)
    : m_id(id), m_voltage(voltage), m_temperature(temperature) {}

/**
 * @brief Gets the unique identifier of the cell.
 * @return The cell ID.
 */
uint8_t BatteryCell::getId() const {
    return m_id;
}

/**
 * @brief Gets the current voltage of the cell.
 * @return The cell voltage in Volts.
 */
float BatteryCell::getVoltage() const {
    return m_voltage;
}

/**
 * @brief Sets the voltage of the cell.
 * @param voltage The new voltage value.
 */
void BatteryCell::setVoltage(float voltage) {
    m_voltage = voltage;
}

/**
 * @brief Gets the current temperature of the cell.
 * @return The cell temperature in Celsius.
 */
float BatteryCell::getTemperature() const {
    return m_temperature;
}

/**
 * @brief Sets the temperature of the cell.
 * @param temperature The new temperature value.
 */
void BatteryCell::setTemperature(float temperature) {
    m_temperature = temperature;
}
