Moonlander – C++ Lunar Lander Simulation

🌐 Project Website: https://aerospace-simulation.dev

Updated from 11.03.2026

Recent Updates
--------------
- Implemented an Adaptive Descent Controller for automated landing.
- The controller uses an energy-based guidance law to compute a safe descent trajectory.
- Introduced brake ratio based descent mode switching for adaptive control behavior.
- Added PD velocity control with gravity compensation and thrust saturation handling.
- Implemented phase-based descent logic enabling stable landing across multiple descent regimes.
- Added a Spacecraft Selection interface allowing users to choose between multiple spacecraft configurations.
- Introduced a JSON-based spacecraft configuration system enabling spacecraft definitions without recompiling the simulation.
- Implemented a centralized ConfigManager responsible for loading and distributing spacecraft configuration data.

Overview
--------
Moonlander is an educational C++ project inspired by the "Lunar Landing" example from the book "Coding for Fun".
It simulates a lunar landing in one or more dimensions, providing a modular backend and a Qt-based frontend.

The project now features full modularization:
- Physics, Integrator, and Sensor models are decoupled
- Orchestrator classes manage high-level operations without implementing raw calculations
- Concrete sensor models provide telemetry (e.g., proper G-load) independent of physics state

Spacecraft configurations are defined via JSON files and loaded at runtime.
This allows new spacecraft variants to be added without modifying the simulation code.

The backend runs in its own thread via SimulationWorker, independent of Qt, while the UI updates in the main thread.
The Logger class allows the backend to report debug messages without Qt dependencies.

Frontend Components
-------------------
cockpitPage              - Qt widget displaying telemetry (time, altitude, speed, thrust, fuel, hull status, autopilot info)
landingView              - Visual representation of the spacecraft and lunar surface
spacecraftSelectionPage  - UI page for selecting spacecraft configurations defined in JSON
homepage                 - Main UI container managing resource loading, configuration, and signals to worker
SimulationWorker         - Worker running the simulation loop in a separate thread, sends state updates via signals
UIBuilder                - Helper class to build consistent Qt UI elements across pages (buttons, LCDs, labels)

Backend Components
------------------

Automation:
  - iautopilot                   - Abstract interface for spacecraft autopilot controllers
  - adaptiveDescentController    - Energy-based landing controller using brake ratio guidance

Control:
  - inputArbiter         - Determines whether manual thrust commands or autopilot control the spacecraft

Controller:
  - iController          - Abstract interface for spacecraft velocity controller
  - pd_controller        - Proportional Derivative controller supporting autopilot

Integrators:
  - IIntegrator          - Interface for all integrators
  - EulerIntegrator      - Example concrete integrator for 1D/ND Euler integration
  - Dynamics             - Physics helper computing acceleration and forces

Optimization:
  - OptimizationModelParams    - Physical model parameters (gravity, Isp, thrust limits)
  - OptimizationStruct         - Base optimization data structure (state, horizon, weights)
  - ThrustOptimizationProblem  - Struct describing optimization problem
  - ThrustOptimizer            - Executes NLopt optimization over thrust profile

Physics:
  - IPhysicsModel          - Abstract interface for physics models
  - BasicMoonGravity       - Implements lunar gravity
  - Physics (orchestrator) - Delegates physics calls to active physics model and integrator

Sensor & Perception:
  - ISensor              - Sensor interface
  - SensorModel          - Computes proper G-load

Thrust & Spacecraft:
  - Thrust                - Engine model (target thrust, actual thrust, response)
  - CustomSpacecraftStruct - Defines spacecraft mass, geometry, fuel, etc.
  - Spacecraft            - Central owner of spacecraft state and dynamics
  - SpacecraftStateStruct - Encapsulates mass, fuel, position, velocity, thrust, integrity
  - StateVectorStruct     - Unified state vector for physics calculations
  - Quaternion            - Orientation representation for future 3D expansion
  - Vector3               - 3D vector math utility

Environment & Utilities:
  - EnvironmentConfig     - Constants for gravity, timestep, max simulation step, moon radius
  - JsonConfigReader      - Reads configuration from JSON
  - ConfigManager         - Loads spacecraft definitions and provides configuration access for the UI
  - Spacemath             - Obsolete math utilities (partially replaced by Vector3/Quaternion)
  - SimDataStruct         - Stores simulation state for frontend visualization
  - SimControl            - Orchestrates simulation, validates parameters, steps simulation
  - Logger                - Singleton logger for backend debug output
  - Output                - Obsolete visualization logic (deprecated)

State Vector & Dynamics
----------------------
- StateVectorStruct is the single source of truth for spacecraft state
- Encapsulates inertial position, inertial velocity, orientation (quaternion), body angular velocity, and total mass
- Only the Spacecraft class has write access; other classes have read-only access
- Physics computations are delegated via IPhysicsModel and IIntegrator interfaces
- Physics orchestrator calls integrators without performing calculations itself
- Proper G-load is computed via SensorModel independently of the physics state

Threading & Signals
------------------
- The backend simulation runs in a dedicated thread (SimulationWorker)
- Signals communicate simulation state (stateUpdated) to the frontend
- The frontend updates cockpitPage, landingView, and other widgets in real time
- Qt timers drive the simulation step frequency
- JSON configuration is loaded in the main thread via ConfigManager and provided to UI components
- Selected spacecraft configuration is forwarded to the backend before simulation start

Current Features
----------------
- Full separation of backend and frontend
- Modular physics with interchangeable models (IPhysicsModel) and integrators (IIntegrator)
- Real-time telemetry displayed in cockpitPage via SensorModel
- Proper G-load calculation using onboard sensor abstraction
- Thread-safe simulation loop with Qt signal/slot integration
- Spacecraft is the sole owner of state and dynamics
- Backend supports testing spacecraft controllers independently of UI
- Legacy variables removed; state vector integrated into SimDataStruct
- Adaptive Descent Controller for automated landing
- Multiple spacecraft configurations selectable via JSON
- ConfigManager providing centralized configuration management

Adaptive Descent Controller
---------------------------
A modular energy-based landing controller has been implemented to guide the spacecraft safely during the final descent phase.

The controller evaluates the spacecraft state and adjusts thrust commands using predictive guidance and feedback control.

Core concepts:

Brake Ratio
-----------
R_brake = h / d_brake

where

d_brake = v² / (2 * a_max)

This ratio determines how aggressively the spacecraft must decelerate.

Descent Phases
--------------
MODE_A  - Energy Dissipation (high altitude)
MODE_B  - Controlled Descent
MODE_C  - Terminal Approach
MODE_D  - Critical Braking

Each phase uses different controller parameters.

Guidance Law
------------
v_target = -sqrt(2 * k_r * a_max * h)

Control Law
-----------
a_cmd = Kp (v_target - v) + Kd (dv/dt)

Thrust command:

T = m (a_cmd + g)

Thrust is limited to engine capability.

Key Characteristics
-------------------
- Energy-based descent planning
- Adaptive gain scheduling
- Gravity compensation
- Thrust saturation handling
- Stable landing behavior across multiple descent regimes

Thrust Optimization (Experimental)
---------------------------------
- 1D thrust optimizer implemented using NLopt
- Goal: minimize fuel consumption while ensuring safe landing
- Forward simulation uses EulerIntegrator
- Cost function includes terminal cost, fuel usage, thrust smoothness, and descent encouragement
- Current status: functional but still under tuning

Goals
-----
- Build a robust simulation environment for testing different controllers
- Provide a modular framework for physics, sensors, and control development
- Enable frontend UI development separately from backend improvements
- Maintain extendable architecture for easy integration of new models

Planned Extensions
------------------

Short-Term:
  - Stabilize Qt telemetry display
  - Extend Logger with file output and log levels
  - Implement additional sensors (gyroscope, altimeter)
  - Improve backend precision
  - Stabilize 1D thrust optimization

Mid-Term:
  - Full 3D spacecraft model
  - Stable lunar orbit simulation
  - Controller testing environment
  - Further modularize frontend widgets

Long-Term:
  - Replay and record simulation sessions
  - Advanced autopilot algorithms
  - Terrain and environmental effects
  - Multi-spacecraft simulations

Build & Run
-----------

Requirements:
  - C++20 or newer
  - Qt6
  - G++ or Clang
  - NLopt library

Linux:
  sudo apt install libnlopt-dev

Compile example:

g++ -std=c++20 -Wall -Wextra -O2 main.cpp physics.cpp BasicMoonGravityModel.cpp EulerIntegrator.cpp SensorModel.cpp Thrust.cpp simcontrol.cpp spacecraft.cpp customSpacecraftStruct.cpp stateVectorStruct.cpp quaternion.cpp vector3.cpp jsonConfigReader.cpp Logger.cpp cockpitPage.cpp landingView.cpp homepage.cpp SimulationWorker.cpp UIBuilder.cpp -o moonlander -lnlopt

Run:

./moonlander

Future Vision
-------------
Moonlander will evolve into a fully modular lunar landing simulation framework where:

- Backend handles physics, state, integrator, sensor telemetry, and optimization
- Frontend provides real-time telemetry, user input, and visualization
- Logger centralizes backend debug output
- UI development continues independently from backend improvements
- Simulation allows testing different control algorithms
- Future support for 3D dynamics, orbital maneuvers, and environmental effects
- Automated landing with fuel-optimal control
