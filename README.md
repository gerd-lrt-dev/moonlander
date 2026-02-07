Moonlander – C++ Lunar Lander Simulation

Updated from 07.02.2026

Overview
--------
Moonlander is an educational C++ project inspired by the "Lunar Landing" example from the book "Coding for Fun".
It simulates a lunar landing in one or more dimensions, providing a modular backend and a Qt-based frontend.

The project now features **full modularization**:
- Physics, Integrator, and Sensor models are decoupled
- Orchestrator classes manage high-level operations without implementing raw calculations
- Concrete sensor models provide telemetry (e.g., proper G-load) independent of physics state

The backend runs in its own thread via `SimulationWorker`, independent of Qt, while the UI updates in the main thread.
The Logger class allows the backend to report debug messages without Qt dependencies.

Frontend Components
-------------------
cockpitPage        - Qt widget displaying telemetry (time, altitude, speed, thrust, fuel, hull status)
landingView        - Visual representation of the spacecraft and lunar surface
homepage           - Main UI container, manages resource loading, configuration, and signals to worker
SimulationWorker   - Worker running the simulation loop in a separate thread, sends state updates via signals
UIBuilder          - Helper class to build consistent Qt UI elements across pages (buttons, LCDs, labels)

Backend Components
------------------
Integrators:
  - IIntegrator          - Interface for all integrators
  - EulerIntegrator      - Example concrete integrator for 1D/ND Euler integration
  - Dynamics             - Physics-specific helper for acceleration, forces

Optimization:
  - OptimizationModelParams    - Physical model parameters (gravity, Isp, thrust limits)
  - OptimizationStruct          - Base optimization data structure (state, horizon, weights)
  - ThrustOptimizationProblem  - Struct encapsulating optimization problem (initial state, constraints, cost weights)
  - ThrustOptimizer            - Executes NLopt optimization over thrust profile

Physics:
  - IPhysicsModel        - Abstract interface for physics models
  - BasicMoonGravity     - Implements lunar gravity, inherits IPhysicsModel
  - Physics (orchestrator) - Delegates physics calls to active physics model and integrator

Sensor & Perception:
  - ISensor              - Sensor interface
  - SensorModel          - Computes proper G-load, inherits ISensor

Thrust & Spacecraft:
  - Thrust               - Engine model (target thrust, actual thrust, response)
  - CustomSpacecraftStruct - Defines spacecraft mass, geometry, fuel, etc.
  - Spacecraft           - Central owner of spacecraft state and dynamics
  - SpacecraftStateStruct - Encapsulates mass, fuel, position, velocity, thrust, integrity
  - StateVectorStruct    - Unified state vector for all physics calculations
  - Quaternion           - Orientation representation for 3D expansion
  - Vector3              - 3D vector math utility

Environment & Utilities:
  - EnvironmentConfig    - Constants for gravity, timestep, max simulation step, moon radius
  - JsonConfigReader     - Reads configuration from JSON
  - Spacemath            - Obsolete math utilities (partially replaced by Vector3/Quaternion)
  - SimDataStruct        - Stores simulation state for frontend visualization
  - SimControl           - Orchestrates simulation, validates parameters, steps simulation
  - Logger               - Singleton logger for backend debug output
  - Output               - Obsolete visualization logic (ASCII cockpit/logs, deprecated)

State Vector & Dynamics
----------------------
- `StateVectorStruct` is the single source of truth for spacecraft state
- Encapsulates: inertial position, inertial velocity, orientation (quaternion), body angular velocity, total mass
- Only the `Spacecraft` class has write access; other classes have read-only access
- Physics computations are delegated via `IPhysicsModel` and `IIntegrator` interfaces
- `Physics` orchestrator calls integrators without performing calculations itself
- Proper G-load is computed via `SensorModel`, independent of the physics state

Threading & Signals
------------------
- The backend simulation is executed in a dedicated thread (`SimulationWorker`)
- Signals communicate simulation state (`stateUpdated`) to the frontend
- The frontend updates cockpitPage, landingView, and other widgets in real time
- Qt timers drive the simulation step frequency, replacing previous std::chrono loops
- JSON configuration is loaded in the main thread (homepage) and passed as string to the worker

Current Features
----------------
- Full separation of backend and frontend
- Modular physics with interchangeable models (`IPhysicsModel`) and integrators (`IIntegrator`)
- Real-time telemetry displayed in cockpitPage via `SensorModel`
- Proper G-load calculation using onboard sensor abstraction
- Thread-safe simulation loop with Qt signal/slot integration
- Spacecraft is the sole owner of state and dynamics
- Backend supports testing spacecraft controllers independently of UI
- Legacy variables removed; state vector integrated into `SimDataStruct` for frontend

Thrust Optimization (Experimental)
---------------------------------
- A 1D lunar landing thrust optimizer has been implemented in the backend.
- OptimizationGoal: Minimize fuel consumption while ensuring a safe landing (target altitude and velocity).
- Forward simulation uses `EulerIntegrator` to propagate spacecraft state under candidate thrust profiles.
- Cost function includes:
  - Terminal cost (deviation from target height and safe landing velocity)
  - Fuel consumption
  - Thrust smoothness
  - Linear descent encouragement
- Current status:
  - Optimizer runs and evaluates thrust sequences but is **not yet fully stable**.
  - Some simulation steps show minor mass fluctuations due to mass floor enforcement (mass cannot physically increase).
  - Descent trajectories are plausible, but landing precision and fuel optimization are still under tuning.
- Next steps:
  - Correct mass handling to prevent non-physical mass increases.
  - Adjust cost weights to prioritize terminal conditions over smoothness/descent incentives.
  - Integrate thrust normalization to ensure candidate thrust profiles are within physical limits.
  - Stabilize optimizer convergence for consistent landing trajectories.
- This module serves as a **testbed for future fuel-optimal automated landing algorithms** and is integrated into the current modular backend architecture.

Goals
-----
- Build a robust simulation environment for testing different controllers
- Provide a modular framework for physics, sensors, and control development
- Enable frontend UI development separately from backend improvements
- Maintain extendable architecture for easy integration of new physics models, sensors, and integrators

Planned Extensions
------------------
Short-Term:
  - Stabilize Qt-based telemetry display and cockpit updates
  - Extend Logger with file output and multiple log levels
  - Implement additional sensors (gyroscope, altimeter) using SensorModel base
  - Improve backend simulation precision and controller hooks
  - Implement and stabilize 1D thrust optimization routines for automated landing
  - Adjust cost functions and weightings to enforce physically plausible trajectories

Mid-Term:
  - Implement full 3D spacecraft model
  - Simulate stable lunar orbit before descent
  - Environment simulation to test multiple controllers (PID, adaptive, optimal, etc.)
  - Modularize frontend widgets (UIBuilder) for consistent design

Long-Term:
  - Replay and record simulation sessions
  - Implement advanced autopilot and control algorithms
  - Extend environment with terrain and atmospheric effects
  - Multi-spacecraft scenarios and orbital dynamics

Build & Run
-----------
Requirements:
  - C++20 or newer
  - Qt6 (for frontend UI)
  - G++ or Clang compiler
  - NLopt library (for 1D thrust optimization)
    - Linux: `sudo apt install libnlopt-dev`
    - Windows: download prebuilt binaries or build from source
    - Include NLopt headers in project: `#include <nlopt.hpp>`

Compile (command line):
  g++ -std=c++20 -I -Wall -Wextra -O2 main.cpp physics.cpp BasicMoonGravityModel.cpp EulerIntegrator.cpp SensorModel.cpp Thrust.cpp simcontrol.cpp spacecraft.cpp customSpacecraftStruct.cpp stateVectorStruct.cpp quaternion.cpp vector3.cpp jsonConfigReader.cpp Logger.cpp cockpitPage.cpp landingView.cpp homepage.cpp SimulationWorker.cpp UIBuilder.cpp -o moonlander -lnlopt

Run:
  ./moonlander

Future Vision
-------------
Moonlander will evolve into a fully modular lunar landing simulation framework where:
- Backend handles physics, state, integrator, sensor telemetry, and optimization independently
- Frontend provides real-time telemetry, user input, and visualization
- Logger centralizes all backend debug messages
- UI development (Qt widgets) continues separately from backend improvements
- Simulation environment allows full testing of different control algorithms
- Ultimately supports 3D dynamics, orbital maneuvers, and environmental effects
- Automated landing with fuel-optimal control
