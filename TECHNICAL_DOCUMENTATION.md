Battery Management System (BMS) Prototype: Technical Documentation & System Design
1. High-Level Description

1.1 Purpose of the BMS
The Battery Management System (BMS) Prototype is a foundational software component designed to monitor, protect, and manage lithium-ion battery packs. Its primary purpose is to ensure the safe, reliable, and efficient operation of the battery by continuously assessing its critical parameters, estimating its state, and reacting appropriately to abnormal conditions. 

1.2 Key Features
The current prototype, along with planned extensions, encompasses the following key features:

Battery Parameter Monitoring: Real-time acquisition and monitoring of individual cell voltages and temperatures, as well as overall pack current.

Safety Management: A robust safety manager evaluates all monitored parameters against configurable thresholds, transitioning the system through defined safety states (NORMAL, WARNING, CRITICAL, FAULT).

State Estimation: Basic State-of-Charge (SoC) estimation using Coulomb counting and simplified State-of-Health (SoH) estimation via cycle counting.

Fault Handling: Detection of out-of-bounds conditions (voltage, temperature, current, SoH) and initiation of appropriate responses, including logging and a placeholder for critical actions like shutdown.

Modular & Portable Design: Clear separation of concerns (sensor abstraction, safety logic, core BMS) to enable easy adaptation to different hardware and future feature additions.

Simulated Environment: A SensorSimulator module allows for comprehensive testing and development without the need for physical battery hardware, including the injection of simulated faults.

Basic Logging: Provides console output for system status, sensor readings, and state transitions, serving as a foundation for more advanced logging.

Power Management Awareness: Identifies whether the battery pack is currently charging or discharging based on current flow.

2. System Architecture
2.1 High-Level Block Diagram (Conceptual)
+-------------------------------------------------------------------------------------------+
|                                 BMS_Prototype System                                      |
|                                                                                           |
|  +---------------------+        +---------------------+        +----------------------+   |
|  |   Sensor Layer      |        |   Core BMS Logic    |        |   Safety & State     |   |
|  | (SensorSimulator /  | <----->| (BMS Class)         |<------>|   Management         |   |
|  |   Hardware Drivers) |        |                     |        | (SafetyManager Class)|   |
|  +---------------------+        +---------------------+        +---------------------+    |
|          ^      |                      ^      |                      ^      |             |
|          |      |                      |      |                      |      |             |
|          |      V                      |      V                      |      V             |
|  +---------------------+        +---------------------+        +-----------------------+  |
|  | Battery Cell Data   |        | SoC/SoH Estimation  |        | System State (NORMAL, |  |
|  | (Voltage, Temp)     |        | (Coulomb Counting,  |        | WARNING, CRITICAL,    |  |
|  | Pack Current        |        | Cycle Counting)     |        | FAULT)                |  |
|  +---------------------+        +---------------------+        +-----------------------+  |
|                                                                                           |
|  +-------------------------------------------------------------------------------+        |
|  | External Interfaces (Future)                                                  |        |
|  |   - Communication (CAN, UART, I2C)                                            |        |
|  |   - Power Control (Relays, Contactors)                                        |        |
|  |   - User Interface (LEDs, Display)                                            |        |
+-------------------------------------------------------------------------------------------+

2.2 Description of Modules
Constants.h:

Purpose: Centralized repository for all configurable system constants, including battery pack characteristics (e.g., NUM_CELLS, NOMINAL_CAPACITY_MAH), safety thresholds (voltage, temperature, current limits for NORMAL, WARNING, CRITICAL, FAULT states), and simulation parameters.

Responsibility: Defines system-wide parameters to ensure consistency and ease of tuning.

BMS_States.h:

Purpose: Defines the SystemState enumeration, which represents the overall operational safety status of the BMS.

Responsibility: Provides a clear, enumerated type for system states, enhancing readability and maintainability of state-based logic.

BatteryCell.h/BatteryCell.cpp:

Purpose: Encapsulates the fundamental data and basic operations for a single battery cell.

Responsibility: Stores cell-specific data (ID, voltage, temperature) and provides methods to access and update these values. Designed to be a simple data container.

SensorSimulator.h/SensorSimulator.cpp:

Purpose: Provides a hardware-agnostic abstraction layer for acquiring sensor data. In the prototype, it simulates realistic voltage, temperature, and current readings, including configurable fault injection.

Responsibility: Generates simulated sensor data. This module is the primary candidate for replacement with actual hardware drivers (e.g., ADC readings, thermistor interfaces) when porting to a real MCU.

SafetyManager.h/SafetyManager.cpp:

Purpose: Implements the core safety logic of the BMS. It evaluates all incoming sensor data (cell voltages, temperatures, pack current) and estimated states (SoH) against predefined safety limits.

Responsibility: Determines and manages the overall SystemState (NORMAL, WARNING, CRITICAL, FAULT) based on the most severe detected condition. It handles state transitions and reports changes.

BMS.h/BMS.cpp:

Purpose: The central orchestrator of the BMS. It initializes the system, periodically reads sensor data (via SensorSimulator), updates state estimations (SoC, SoH), and invokes the SafetyManager to evaluate the system's safety status. It also contains basic logging and fault handling mechanisms.

Responsibility: Coordinates data flow between modules, performs state estimation, manages the main update loop, and triggers high-level actions based on the determined system state.

main.cpp:

Purpose: The entry point of the application. It instantiates the BMS object, initializes it, and runs the continuous update loop.

Responsibility: Provides the main execution context for the BMS application. In an RTOS environment, this might become a task entry function.

2.3 Data Flow and Responsibilities
Initialization (main.cpp -> BMS::init()):

main creates BMS object.

BMS constructor initializes SensorSimulator, SafetyManager, and BatteryCell array.

BMS::init() performs initial setup and logging.

Periodic Update Loop (main.cpp -> BMS::update(deltaTime_s)):

main calls BMS::update() at a fixed interval (BMS_UPDATE_INTERVAL_MS).

Sensor Reading (BMS -> SensorSimulator): BMS requests latest voltage and temperature for each cell, and the total pack current from SensorSimulator.

Data Storage (BMS -> BatteryCell): BMS updates the BatteryCell objects with the new readings.

State Estimation (BMS): BMS uses the current and previous data to update m_accumulatedCharge_mAh, m_stateOfCharge_percent, m_chargeCycles, and m_stateOfHealth_percent.

Safety Evaluation (BMS -> SafetyManager): BMS passes the updated BatteryCell array, packCurrent, and stateOfHealth_percent to SafetyManager::evaluate().

State Transition (SafetyManager): SafetyManager assesses all parameters against thresholds, determines the new SystemState, and logs any state changes.

Action & Logging (BMS): BMS retrieves the SystemState from SafetyManager and performs state-specific actions (e.g., logEvent, handleFault). It then prints the overall system status.

2.4 Extensibility for Real Hardware or Additional Modules
The design uses dependency inversion and abstraction to achieve extensibility:

Sensor Abstraction: The BMS class depends on an abstract interface (implicitly, through SensorSimulator's public methods). To integrate real hardware, you would create a new class (e.g., HardwareSensor) that implements the same public methods (readVoltage, readTemperature, readCurrent) but interacts with actual ADC channels and sensor ICs. The BMS class itself would require minimal or no changes, simply instantiating HardwareSensor instead of SensorSimulator.

Modular Responsibilities: Each module (e.g., SafetyManager, BMS) has a well-defined, single responsibility. This allows for:

Adding New Features: New modules (e.g., BalancingManager, CommunicationManager) can be added without significantly altering existing core logic.

Replacing Algorithms: The SoC/SoH estimation in BMS.cpp can be replaced with more advanced algorithms (e.g., Kalman filters, neural networks) by modifying only the updateSoC and updateSoH methods, or by introducing new estimation classes.

Scalability: Adapting to more cells would primarily involve changing NUM_CELLS in Constants.h and ensuring the sensor reading loop covers all cells.

3. Component Design
3.1 UML-style Class/Interface Descriptions
Class: BatteryCell

Purpose: Represents a single cell in the battery pack.

Attributes:

- m_id: uint8_t (Unique identifier for the cell)

- m_voltage: float (Current voltage in Volts)

- m_temperature: float (Current temperature in Celsius)

Methods:

+ BatteryCell() (Default Constructor)

+ BatteryCell(id: uint8_t, voltage: float, temperature: float) (Parameterized Constructor)

+ getId() const: uint8_t

+ getVoltage() const: float

+ setVoltage(voltage: float): void

+ getTemperature() const: float

+ setTemperature(temperature: float): void

Class: SensorSimulator

Purpose: Simulates sensor readings (voltage, temperature, current) with optional fault injection.

Attributes:

- m_rng: std::mt19937 (Random number generator)

- m_voltageDist: std::uniform_real_distribution<float>

- m_tempDist: std::uniform_real_distribution<float>

- m_currentDist: std::uniform_real_distribution<float>

- m_faultDist: std::uniform_real_distribution<float>

Methods:

+ SensorSimulator() (Constructor)

+ readVoltage(cellId: uint8_t): float

+ readTemperature(cellId: uint8_t): float

+ readCurrent(): float

Class: SafetyManager

Purpose: Evaluates battery parameters and manages the system's safety state.

Attributes:

- m_currentState: SystemState (Current safety state of the BMS)

Methods:

+ SafetyManager() (Constructor)

+ evaluate(cells: const std::array<BatteryCell, NUM_CELLS>&, packCurrent: float, stateOfHealth_percent: float): void

+ getCurrentState() const: SystemState

- isVoltageNormal(voltage: float) const: bool (Private helper)

- isVoltageWarning(voltage: float) const: bool (Private helper)

- isVoltageCritical(voltage: float) const: bool (Private helper)

- isVoltageFault(voltage: float) const: bool (Private helper)

- isTemperatureNormal(temperature: float) const: bool (Private helper)

- isTemperatureWarning(temperature: float) const: bool (Private helper)

- isTemperatureCritical(temperature: float) const: bool (Private helper)

- isTemperatureFault(temperature: float) const: bool (Private helper)

- isCurrentNormal(current: float) const: bool (Private helper)

- isCurrentWarning(current: float) const: bool (Private helper)

- isCurrentCritical(current: float) const: bool (Private helper)

- isSoHNormal(soh: float) const: bool (Private helper)

- isSoHWarning(soh: float) const: bool (Private helper)

- isSoHCritical(soh: float) const: bool (Private helper)

Class: BMS

Purpose: Orchestrates the BMS operations.

Associations:

--o SensorSimulator (Composition)

--o SafetyManager (Composition)

--o BatteryCell (Composition - array of cells)

Attributes:

- m_sensorSimulator: SensorSimulator

- m_safetyManager: SafetyManager

- m_cells: std::array<BatteryCell, NUM_CELLS>

- m_packCurrent: float

- m_accumulatedCharge_mAh: float

- m_stateOfCharge_percent: float

- m_stateOfHealth_percent: float

- m_chargeCycles: float

- m_wasFull: bool

- m_wasEmpty: bool

- m_isChargingFlag: bool

Methods:

+ BMS() (Constructor)

+ init(): void

+ update(deltaTime_s: float): void

+ getCurrentState() const: SystemState

+ getSoC() const: float

+ getSoH() const: float

+ getPackCurrent() const: float

+ isCharging() const: bool

- updateSoC(deltaTime_s: float): void (Private helper)

- updateSoH(): void (Private helper)

- logEvent(message: const std::string&): void (Private helper)

- handleFault(faultDescription: const std::string&): void (Private helper)

3.2 State Machine for Safety Status Transitions
The SafetyManager class implements a hierarchical state machine logic to determine the overall SystemState. The transitions are based on the severity of violations detected across all monitored parameters (voltage, temperature, current, SoH).

States:

NORMAL: All monitored parameters are within their normal operating ranges.

WARNING: One or more parameters are outside their normal limits but within the warning thresholds. The system can continue operation but requires attention.

CRITICAL: One or more parameters have crossed critical thresholds. Immediate action is required (e.g., severe power limitation, preparation for shutdown).

FAULT: A severe, unrecoverable error (e.g., sensor malfunction, extreme out-of-bounds reading) or a persistent critical condition has occurred. The system must be shut down or rendered inoperable.

Transition Logic (Prioritized):

Any FAULT condition detected (voltage/temperature outside _FAULT limits):

Always transitions immediately to FAULT state, regardless of current state. This is the highest priority.

Any CRITICAL condition detected (voltage/temperature outside _CRITICAL limits, current outside _CRITICAL_A limits, SoH below SOH_THRESHOLD_CRITICAL):

If current state is NORMAL or WARNING, transitions to CRITICAL.

If current state is already CRITICAL or FAULT, remains in that state (or transitions to FAULT if a new FAULT condition is met).

Any WARNING condition detected (voltage/temperature outside _WARNING limits, current outside _WARNING_A limits, SoH below SOH_THRESHOLD_WARNING):

If current state is NORMAL, transitions to WARNING.

If current state is already CRITICAL or FAULT, remains in that state (WARNING is less severe).

All parameters within NORMAL limits:

If current state is WARNING, transitions to NORMAL.

If current state is CRITICAL or FAULT, it does not automatically transition back to NORMAL or WARNING without explicit reset/recovery logic (not fully implemented in this prototype for CRITICAL/FAULT for safety reasons, as these often require manual intervention or specific recovery sequences).

4. Project Structure Explanation
The project follows a standard and widely accepted directory structure for C++ projects, especially common in embedded development.

BMS_Prototype/ (Root Directory):

The top-level directory containing all project files.

inc/:

Purpose: Stores all header files (.h). These files contain class declarations, function prototypes, enum definitions, and constant declarations. They define the interfaces of your modules.

Reasoning: Separating headers makes it clear what interfaces are available and helps manage dependencies. Only necessary headers are included in source files, reducing compilation times and potential circular dependencies.

src/:

Purpose: Stores all source files (.cpp). These files contain the actual implementations (definitions) of the classes and functions declared in the header files.

Reasoning: Separating source files from headers promotes modularity and allows for independent compilation of modules.

obj/:

Purpose: This directory is created by the Makefile and stores all object files (.o). These are intermediate files generated by the compiler from each .cpp file.

Reasoning: Keeps intermediate build artifacts separate from source code, maintaining a clean project directory.

bin/:

Purpose: This directory is created by the Makefile and stores the final executable binary.

Reasoning: Provides a clear location for the compiled application.

.gitignore:

Purpose: A text file used by Git (version control system) to specify intentionally untracked files and directories that Git should ignore.

Reasoning: Prevents unnecessary or transient files (like object files, executables, build directories, IDE-specific files) from being committed to the repository, keeping the repository clean and focused on source code.

Makefile:

Purpose: A build automation script that defines rules for compiling source code, linking object files, and creating the final executable.

Reasoning: Provides a portable and consistent way to build the project across different development environments, automating the compilation process.

README.md:

Purpose: A markdown file that serves as the primary documentation for the project. It provides a high-level overview, setup instructions, features, and future plans.

Reasoning: Essential for anyone (including yourself in the future) to quickly understand what the project is, how to build/run it, and its current status.

4.1 Build System (Makefile)
The provided Makefile is a simple yet effective build system for this C++ project.

CXX = g++: Specifies the C++ compiler.

CXXFLAGS: Compiler flags for warnings (-Wall, -Wextra), C++ standard (-std=c++17), include paths (-Iinc), and optimization (-O2).

LDFLAGS = -pthread: Linker flags, specifically -pthread is crucial for enabling std::thread and std::chrono functionalities used for time delays in main.cpp.

SRCS = $(wildcard $(SRC_DIR)/*.cpp): Automatically finds all .cpp files in the src/ directory. This makes the Makefile scalable as you add new source files.

OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS)): Generates the corresponding object file names for each source file, placing them in the obj/ directory.

all target: The default target, which ensures obj/ and bin/ directories exist, then links all object files into the final bms_prototype executable.

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp: A pattern rule that tells make how to compile any .cpp file in src/ into a .o file in obj/.

clean target: A utility target to remove all generated object files, the executable, and the build directories, allowing for a fresh build.

4.2 GitHub Setup
For a production-grade project on GitHub, consider these practices:

.gitignore: The provided .gitignore is standard for C++ projects and will prevent compiled binaries, object files, and IDE-specific files from being committed. This keeps your repository clean and focused on source code.

README.md: The comprehensive README.md is crucial. It acts as the project's landing page, explaining its purpose, features, how to build/run, and future plans. Keep it updated.

Branching Strategy (e.g., Git Flow or GitHub Flow):

main (or master) branch: Should always contain stable, production-ready code.

develop branch: For ongoing development and integration of new features.

Feature branches: Create a new branch for each new feature or significant bug fix (e.g., feature/soc-kalman, bugfix/temp-sensor-drift). Merge these into develop once complete.

Release branches: For preparing new releases.

Hotfix branches: For urgent fixes on the main branch.

Commit Messages: Use clear, concise, and descriptive commit messages that explain what was changed and why.

Pull Requests (PRs): All code changes should go through PRs, allowing for code reviews before merging.
