# Moonlander Architecture

Moonlander is designed as a modular spacecraft simulation framework written in C++.  
The architecture separates simulation logic, control systems, sensors, and visualization in order to make experimentation with guidance algorithms easier.

The system follows several design principles:

- modular components
- interface driven architecture
- separation of simulation backend and user interface
- extensibility for new physics models and controllers


---

# Core System Structure

The simulation is organized into several major subsystems:

- Simulation Core
- Physics Models
- Integrators
- Control Systems
- Sensors
- Spacecraft Model
- Frontend UI


---

# Simulation Core

The simulation core coordinates all subsystems and controls the simulation loop.

Main responsibilities:

- managing the spacecraft state
- executing physics updates
- invoking control algorithms
- providing telemetry to the UI

The main orchestrator is:

`SimControl`

The simulation itself runs inside a worker thread using:

`SimulationWorker`

This ensures the graphical user interface remains responsive while the simulation is running.


---

# State Representation

The spacecraft state is stored in the structure:

`StateVectorStruct`

The state vector includes:

- inertial position
- inertial velocity
- orientation (quaternion)
- body angular velocity
- total mass

Only the `Spacecraft` class modifies the state.  
Other systems access the state in read-only form.

This prevents inconsistent state modifications across subsystems.


---

# Physics Layer

Physics calculations are abstracted through the interface:

`IPhysicsModel`

This allows multiple physics environments to be implemented.

Example implementation:

`BasicMoonGravity`

Physics models compute environmental forces such as:

- gravitational acceleration
- thrust forces
- mass flow effects


---

# Integrators

Time integration is abstracted through the interface:

`IIntegrator`

Integrators advance the spacecraft state according to system dynamics.

Example implementation:

`EulerIntegrator`

Future integrators could include:

- Runge-Kutta methods
- adaptive time-step integrators


---

# Control System

Controllers generate thrust commands based on spacecraft state.

Controller interface:

`iController`

Example controller:

`pd_controller`

Controllers may operate:

- directly through manual control
- through an autopilot system


---

# Autopilot System

Autopilot algorithms implement the interface:

`iautopilot`

Example implementation:

`adaptiveDescentController`

The autopilot generates thrust commands based on landing guidance logic.


---

# Sensors

Sensors provide telemetry derived from the spacecraft state.

Sensor interface:

`ISensor`

Example implementation:

`SensorModel`

Currently implemented telemetry:

- proper G-load

Future sensors may include:

- gyroscope
- altimeter
- accelerometer


---

# Spacecraft Model

The spacecraft system encapsulates all spacecraft related properties.

Important components:

- `Spacecraft`
- `Thrust`
- `CustomSpacecraftStruct`

These components manage:

- spacecraft mass
- fuel state
- thrust output
- structural integrity


---

# Configuration System

Spacecraft configurations are defined using JSON files.

These configurations include:

- mass parameters
- fuel capacity
- thrust limits
- geometry parameters

Configurations are loaded at runtime using:

`ConfigManager`

This allows spacecraft variants to be added without recompiling the simulation.


---

# Threading Model

The simulation backend runs in a dedicated worker thread.

Communication between backend and UI occurs through Qt signals such as:

`stateUpdated`

The UI receives these updates and refreshes telemetry displays.


---

# Frontend UI

The graphical interface is implemented using Qt.

Main UI components include:

- `cockpitPage`
- `landingView`
- `spacecraftSelectionPage`
- `homepage`

A helper class named `UIBuilder` creates consistent UI elements.


---

# Future Architecture Extensions

Planned extensions include:

- full 3D spacecraft dynamics
- orbital mechanics simulation
- terrain modelling
- simulation replay system
