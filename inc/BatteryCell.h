// inc/BatteryCell.h
#ifndef BATTERY_CELL_H
#define BATTERY_CELL_H

#include <cstdint> // For uint8_t, float

/**
 * @brief Represents the data for a single battery cell.
 */
class BatteryCell {
public:
    /**
     * @brief Default constructor for BatteryCell.
     * Initializes the cell with default/dummy values.
     */
    BatteryCell(); // <-- This is the missing declaration!

    /**
     * @brief Constructor for BatteryCell.
     * @param id Unique identifier for the cell.
     * @param voltage Initial voltage of the cell.
     * @param temperature Initial temperature of the cell.
     */
    BatteryCell(uint8_t id, float voltage, float temperature);

    /**
     * @brief Gets the unique identifier of the cell.
     * @return The cell ID.
     */
    uint8_t getId() const;

    /**
     * @brief Gets the current voltage of the cell.
     * @return The cell voltage in Volts.
     */
    float getVoltage() const;

    /**
     * @brief Sets the voltage of the cell.
     * @param voltage The new voltage value.
     */
    void setVoltage(float voltage);

    /**
     * @brief Gets the current temperature of the cell.
     * @return The cell temperature in Celsius.
     */
    float getTemperature() const;

    /**
     * @brief Sets the temperature of the cell.
     * @param temperature The new temperature value.
     */
    void setTemperature(float temperature);

private:
    uint8_t m_id;
    float m_voltage;
    float m_temperature;
};

#endif // BATTERY_CELL_H
