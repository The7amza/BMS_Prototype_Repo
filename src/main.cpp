// src/main.cpp
#include "../inc/BMS.h"
#include "../inc/Constants.h" // For BMS_UPDATE_INTERVAL_MS
#include <iostream>
#include <thread>  // For std::this_thread::sleep_for
#include <chrono>  // For std::chrono::milliseconds

/**
 * @brief Main entry point of the BMS prototype application.
 * Initializes the BMS and runs its update loop.
 */
int main() {
    // Create an instance of the BMS
    BMS myBMS;

    // Initialize the BMS
    myBMS.init();

    // Calculate delta time in seconds for SoC updates
    float deltaTime_s = static_cast<float>(BMS_UPDATE_INTERVAL_MS) / 1000.0f;

    // Main application loop
    while (true) {
        // Update the BMS state (read sensors, evaluate safety, etc.)
        myBMS.update(deltaTime_s);

        // In a real embedded system, this would be a hardware-specific delay
        // or a task scheduler. For simulation, we use std::this_thread::sleep_for.
        std::this_thread::sleep_for(std::chrono::milliseconds(BMS_UPDATE_INTERVAL_MS));

        // In a real application, if the state becomes FAULT, you might break the loop
        // or enter a recovery/shutdown routine. For this prototype, we keep running.
        if (myBMS.getCurrentState() == SystemState::FAULT) {
            std::cout << "BMS in FAULT state. Simulation continuing for demonstration, but real system would halt." << std::endl;
            // Potentially add a short delay or user input prompt here before continuing
        }
    }

    return 0;
}
