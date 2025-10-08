# 🌓 Moonlander – C++ Lunar Lander Simulation

**Moonlander** is a fun and educational C++ project inspired by the *"Lunar Landing"* example from the book **Coding for Fun**.  
The goal is to reimplement the classic lunar landing simulation and gradually extend it with modern features, including a visual representation using **Qt**.

---

## 🚀 Project Description

The current version simulates the lunar landing with simplified physics:

- **Gravity**, velocity, and altitude  
- **Controllable thrust** (Thrust class)  
- **Real-time simulation** using `std::chrono`  
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

---

## 🧠 Goals

This project is designed to **practice C++ fundamentals and modern programming techniques** through a clear and interactive example:

- Physics thinking → simulating simple motion equations  
- Object-oriented design → clear separation of logic and display  
- Extensibility → preparation for graphical representation with **Qt**  

---

## 🧰 Planned Extensions

- Qt-based 2D/3D visualization of the lunar landing  
- User control via keyboard or GUI sliders  
- Particle effects and animations  
- Advanced physics (fuel consumption, inertia, autopilot)  
- Saving/loading of flights and statistics  

---

## ⚙️ Build & Run

### Requirements
- **C++20** or newer  
- G++ or Clang  
- Optional: **Qt6** for later visualization  

### Compile (command line)
```bash
g++ -std=c++20 -Wall -Wextra -O2 main.cpp physics.cpp output.cpp simcontrol.cpp Thrust.cpp spacecraft.cpp spacemath.cpp -o moonlander
