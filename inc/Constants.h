// inc/Constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint> // For uint8_t, float

// Define the number of cells in the battery pack
const uint8_t NUM_CELLS = 4;

// --- Battery Pack Characteristics ---
// Nominal capacity of the battery pack in milliampere-hours (mAh)
const float NOMINAL_CAPACITY_MAH = 3000.0f;
// Efficiency of charging (e.g., 0.95 means 95% efficient)
const float CHARGE_EFFICIENCY = 0.98f;
// Threshold for considering battery fully charged for cycle counting
const float SOC_FULL_THRESHOLD_PERCENT = 98.0f;
// Threshold for considering battery fully discharged for cycle counting
const float SOC_EMPTY_THRESHOLD_PERCENT = 10.0f;

// --- Voltage Limits (Volts) ---
// Minimum safe voltage for a single cell
const float MIN_VOLTAGE_NORMAL = 3.00f;
// Voltage below which a WARNING state is triggered
const float MIN_VOLTAGE_WARNING = 2.80f;
// Voltage below which a CRITICAL state is triggered
const float MIN_VOLTAGE_CRITICAL = 2.50f;
// Voltage below which a FAULT state is triggered (e.g., sensor error, dead cell)
const float MIN_VOLTAGE_FAULT = 1.00f;

// Maximum safe voltage for a single cell
const float MAX_VOLTAGE_NORMAL = 4.20f;
// Voltage above which a WARNING state is triggered
const float MAX_VOLTAGE_WARNING = 4.30f;
// Voltage above which a CRITICAL state is triggered
const float MAX_VOLTAGE_CRITICAL = 4.40f;
// Voltage above which a FAULT state is triggered (e.g., sensor error)
const float MAX_VOLTAGE_FAULT = 4.80f;

// --- Temperature Limits (Celsius) ---
// Minimum safe temperature for a single cell
const float MIN_TEMP_NORMAL = 0.0f;
// Temperature below which a WARNING state is triggered
const float MIN_TEMP_WARNING = -5.0f;
// Temperature below which a CRITICAL state is triggered
const float MIN_TEMP_CRITICAL = -10.0f;
// Temperature below which a FAULT state is triggered
const float MIN_TEMP_FAULT = -20.0f;

// Maximum safe temperature for a single cell
const float MAX_TEMP_NORMAL = 45.0f;
// Temperature above which a WARNING state is triggered
const float MAX_TEMP_WARNING = 50.0f;
// Temperature above which a CRITICAL state is triggered
const float MAX_TEMP_CRITICAL = 60.0f;
// Temperature above which a FAULT state is triggered
const float MAX_TEMP_FAULT = 70.0f;

// --- Current Limits (Amperes) ---
// Current below which the battery is considered idle (no charging/discharging)
const float IDLE_CURRENT_THRESHOLD_A = 0.05f; // Amperes
// Maximum continuous discharge current (Amperes)
const float MAX_DISCHARGE_CURRENT_NORMAL_A = 10.0f;
// Discharge current above which a WARNING state is triggered
const float MAX_DISCHARGE_CURRENT_WARNING_A = 15.0f;
// Discharge current above which a CRITICAL state is triggered
const float MAX_DISCHARGE_CURRENT_CRITICAL_A = 20.0f;

// Maximum continuous charge current (Amperes)
const float MAX_CHARGE_CURRENT_NORMAL_A = 2.0f;
// Charge current above which a WARNING state is triggered
const float MAX_CHARGE_CURRENT_WARNING_A = 3.0f;
// Charge current above which a CRITICAL state is triggered
const float MAX_CHARGE_CURRENT_CRITICAL_A = 4.0f;

// --- State of Health (SoH) Limits (%) ---
const float SOH_THRESHOLD_WARNING = 80.0f; // SoH below this triggers WARNING
const float SOH_THRESHOLD_CRITICAL = 60.0f; // SoH below this triggers CRITICAL

// --- Simulation Parameters ---
// Delay in milliseconds between BMS updates in the main loop
const uint32_t BMS_UPDATE_INTERVAL_MS = 1000; // 1 second

// Sensor simulation ranges
const float SIM_VOLTAGE_MIN = 2.00f; // Extended range for fault simulation
const float SIM_VOLTAGE_MAX = 4.60f; // Extended range for fault simulation
const float SIM_TEMP_MIN = -15.0f;   // Extended range for fault simulation
const float SIM_TEMP_MAX = 65.0f;    // Extended range for fault simulation
const float SIM_CURRENT_MIN = -25.0f; // Negative for discharge, Amperes
const float SIM_CURRENT_MAX = 5.0f;   // Positive for charge, Amperes

// Probability (0.0 to 1.0) of a simulated fault occurring
const float SIM_FAULT_PROBABILITY = 0.02f; // 2% chance of a fault

#endif // CONSTANTS_H
