Modular & Portable Battery Management System (BMS) Prototype
This repository contains an enhanced prototype of a Battery Management System (BMS) written in C++. The design emphasizes modularity, portability, and adherence to good software engineering practices, making it suitable for embedded systems development and a strong foundation for a functional safety showcase.

Project Goals
Modular Design: Components are separated into distinct modules (e.g., sensor interface, safety logic) to promote reusability and maintainability.

Hardware Agnostic: The core logic is decoupled from specific hardware, allowing easy porting to various microcontrollers (STM32, ESP32, etc.) by simply replacing the sensor abstraction layer.

Safety Focus: Implements a safety manager that evaluates battery parameters against predefined limits and transitions through system states (NORMAL, WARNING, CRITICAL, FAULT).

Simulated Environment: Includes a simulated sensor layer for initial development and testing without requiring physical hardware.

Good Software Engineering Practices: Follows a clear folder structure, includes a readable README, proper .gitignore, and a portable Makefile.

Showcase: Intended as a showcase for embedded software development and functional safety skills.

Features
4-Cell Lithium Battery Simulation: Simulates voltage and temperature readings for a 4-cell lithium battery pack.

Pack Current Monitoring: Simulates and monitors the total current flowing through the battery pack (charge/discharge).

State-of-Charge (SoC) Estimation: Implements a basic Coulomb counting algorithm to estimate the battery's charge level.

State-of-Health (SoH) Estimation: Includes a simplified cycle counting mechanism to estimate battery degradation over time.

Simulated Sensor Layer: SensorSimulator class provides random, yet realistic, data, with occasional fault injection for testing state transitions.

Safety Manager: Evaluates individual cell voltages, temperatures, pack current, and overall SoH against configurable limits.

System State Management: Transitions the BMS through NORMAL, WARNING, CRITICAL, and FAULT states based on parameter violations and severity.

Basic Fault Handling: Includes a placeholder handleFault function for demonstrating how critical issues would be addressed.

Basic Logging: Features a logEvent function for printing important system messages and state changes.

Power Management Awareness: Determines if the battery is currently charging or discharging based on current readings.

Main Application Loop: Continuously reads data, updates the BMS state, and prints system status to the console.

Folder Structure
BMS_Prototype/
├── inc/                  # Header files (.h)
│   ├── BMS.h
│   ├── BatteryCell.h
│   ├── BMS_States.h
│   ├── Constants.h
│   ├── SafetyManager.h
│   └── SensorSimulator.h
├── src/                  # Source files (.cpp)
│   ├── BMS.cpp
│   ├── BatteryCell.cpp
│   ├── SafetyManager.cpp
│   ├── SensorSimulator.cpp
│   └── main.cpp
├── .gitignore            # Specifies intentionally untracked files to ignore
├── Makefile              # Build automation script
└── README.md             # This file

Building the Project
To build this prototype, you will need a C++ compiler (like g++). The Makefile is configured for Linux/macOS environments.

Navigate to the project root:

cd BMS_Prototype

Run make:

make

This will compile the source files and create the executable in the bin/ directory.

Running the Project
After a successful build, you can run the executable:

./bin/bms_prototype

The application will print simulated sensor readings, BMS state transitions, SoC, SoH, and charging status to your console every second. You will occasionally see "Fault Injected!" messages, demonstrating the state transition logic.

Future Enhancements (Roadmap)
This prototype provides a robust foundation. Future steps could include:

Real Hardware Drivers: Replace SensorSimulator with actual ADC and temperature/current sensor drivers for a specific microcontroller (e.g., STM32, ESP32).

Advanced SoC/SoH Algorithms: Implement more sophisticated algorithms (e.g., Kalman filters for SoC, impedance tracking for SoH).

Active/Passive Balancing: Implement cell balancing strategies.

Robust Fault Handling and Recovery: Detailed fault codes, non-volatile logging, and specific recovery procedures.

Communication Interfaces: Add support for CAN, UART, I2C, or SPI for external communication with other vehicle systems or a diagnostic tool.

Power Management Control: Integrate logic for controlling charging/discharging relays or contactors.

Formal State Machine Implementation: Use a dedicated state machine library or design pattern for more complex and verifiable state transitions.

Unit and Integration Testing: Develop comprehensive unit tests for individual modules and integration tests for the system using frameworks like Google Test.

CMake Build System: Transition from Makefile to CMake for more complex, cross-platform builds and easier integration with IDEs.

Functional Safety Patterns: Incorporate specific functional safety patterns (e.g., redundancy for critical sensors, self-tests, diverse software, hardware diagnostics).

Memory Management: Optimize for embedded memory constraints.

Real-time Operating System (RTOS) Integration: Adapt the main loop to run as a task within an RTOS for better task scheduling and real-time performance.

## Detailed Documentation

For a comprehensive technical overview, system architecture, component design, and future extension plans, please refer to the [Full Technical Documentation](TECHNICAL_DOCUMENTATION.md).

Feel free to contribute, fork, or adapt this project for your own needs!