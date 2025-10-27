# 🌓 Moonlander – C++ Lunar Lander Simulation

**Moonlander** is a fun and educational C++ project inspired by the *"Lunar Landing"* example from the book **Coding for Fun**.  
The goal is to reimplement the classic lunar landing simulation and gradually extend it with modern features, including a visual representation using **Qt**.

---

## 🚀 Project Description

The current version simulates the lunar landing with simplified physics:

- **Gravity**, velocity, and altitude  
- **Controllable thrust** (Thrust class)  
- **Fuel consumption tied to engine usage
- **Real-time simulation** using `std::chrono`  
- **Threaded user input, allowing control while simulation Requirements
- **ASCII-based cockpit** displaying telemetry data  

The project is fully object-oriented and uses modern C++ features such as **smart pointers** and **encapsulation**.

---

## 🧩 Class Structure

| Class        | Purpose |
|--------------|---------|
| `Physics`      | Calculates velocity and height under the influence of gravity |
| `Thrust`       | Models engine thrust with reaction speed and target value |
| `Output`       | ASCII-based cockpit visualization |
| `SimControl`   | Main simulation controller (loop, timing, parameter validation) |
| `Spacecraft`   | Models spacecraft state (mass, fuel, thrust, and integrity) |
| `EnvironmentConfig` | Stores constants such as gravity and simulation timestep
| `Spacemath`    | Utility library providing mathematical helpers

---

## 🧠 Goals

This project is designed to **practice C++ fundamentals and modern programming techniques** through a clear and interactive example:

The current milestone focuses on a one-dimensional lunar landing simulation, where:
- The user can adjust thrust during descent (currently via console, later via Qt GUI).
- The simulation updates altitude, velocity, and fuel in real time.
- The system detects crash landings based on descent velocity.
- This serves as a foundation for extending the simulation to 2D or 3D later on.

---

## 🧰 Planned Extensions

🪶 Short-Term
- Integrate Qt GUI for user input (thrust control) and cockpit visualization
- Replace console input with real-time slider or button controls
- Improve physics precision and fuel model

🌗 Mid-Term
- Add 2D/3D visualization (Qt Quick or OpenGL)
- Implement lateral control and rotation dynamics
- Modularize communication for ROS node integration

🌕 Long-Term
- Record and replay simulation data
- Add autopilot and control algorithms (PID control)
- Include atmospheric and terrain effects

---

## ⚙️ Build & Run

### Requirements
- **C++20** or newer  
- G++ or Clang  
- Optional: **Qt6** for later visualization  

### Compile (command line)
```bash
g++ -std=c++20 -I /usr/include/eigen3 -Wall -Wextra -O2 main.cpp physics.cpp output.cpp simcontrol.cpp Thrust.cpp spacecraft.cpp spacemath.cpp -o moonlander

🧭 Future Vision

Moonlander will evolve into a modular simulation framework, where:
- The core physics engine runs independently of visualization
- The Qt interface acts as a listener and control layer
- ROS nodes can later be used for distributed simulation and telemetry exchange

This design allows independent development of simulation, visualization, and control — following modern software engineering principles.