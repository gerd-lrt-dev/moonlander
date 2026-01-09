========================
Moonlander – C++ Lunar Lander Simulation
========================

Overview
--------
Moonlander is an educational C++ project inspired by the "Lunar Landing" example from the book "Coding for Fun".
It simulates a lunar landing in one or more dimensions, providing a modular backend and a Qt-based frontend.

The project separates:
- Backend: Physics, simulation, spacecraft state, logging, controller testing
- Frontend: Qt UI, cockpit, landing view, telemetry, real-time user interaction

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
Physics            - Calculates velocity and position under gravity and thrust
Thrust             - Engine model: target thrust, response rate, actual thrust
Output             - Handles backend visualization logic (ASCII cockpit, optional logs) with using ui oboselete
SimControl         - Orchestrates the simulation: validates parameters, steps the simulation
Spacecraft         - State of spacecraft: mass, fuel, position, velocity, thrust, integrity
EnvironmentConfig  - Constants for gravity, timestep, max simulation step, etc.
Spacemath          - Math utilities for vectors, physics calculations
Logger             - Singleton logger for backend debug output (no Qt dependency)

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
- Real-time telemetry displayed in cockpitPage
- Thread-safe simulation loop with Qt signal/slot integration
- Logger captures debug output from backend without Qt
- Initial support for resource-based JSON configuration
- Backend supports testing spacecraft controllers independently of UI

Goals
-----
- Build a solid simulation environment for testing different controllers
- Provide a robust framework for physics and control development
- Enable development of UI separately from backend
- Maintain modular architecture for easy extensions and testing

Planned Extensions
------------------
Short-Term:
  - Stabilize Qt-based telemetry display and cockpit updates
  - Integrate landingView with real spacecraft positions
  - Extend Logger with file output and multiple log levels
  - Improve backend simulation precision and controller hooks

Mid-Term:
  - Implement full 3D spacecraft model
  - Simulate stable lunar orbit before descent
  - Environment simulation to test multiple controllers (PID, adaptive, etc.)
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

Compile (command line):
  g++ -std=c++20 -I -Wall -Wextra -O2 main.cpp physics.cpp output.cpp simcontrol.cpp Thrust.cpp spacecraft.cpp spacemath.cpp jsonConfigReader.cpp Logger.cpp cockpitPage.cpp landingView.cpp homepage.cpp SimulationWorker.cpp UIBuilder.cpp -o moonlander

Run:
  ./moonlander

Future Vision
-------------
Moonlander will evolve into a modular lunar landing simulation framework where:
- Backend handles physics, state, and controller testing independently
- Frontend provides real-time telemetry, user input, and visualization
- Logger centralizes all backend debug messages
- UI development (Qt widgets) continues separately from backend improvements
- Simulation environment allows full testing of different control algorithms
- Ultimately supports 3D dynamics, orbital maneuvers, and environmental effects
