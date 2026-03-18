# Adaptive Descent Controller

Moonlander implements an energy-based landing controller designed to guide the spacecraft safely during the final descent phase.

The controller evaluates altitude, velocity, and thrust limits to compute safe thrust commands.


---

# Control Strategy

The controller is based on an energy dissipation strategy.

The goal is to reduce kinetic and potential energy such that the spacecraft reaches the surface with near-zero velocity.


---

# Brake Ratio

A key metric used by the controller is the brake ratio.

R_brake = h / d_brake

where

d_brake = v² / (2 * a_max)

Variables:

h = current altitude  
v = current vertical velocity  
a_max = maximum deceleration capability


The brake ratio indicates whether the spacecraft has sufficient altitude remaining to safely brake.


---

# Descent Phases

The controller operates in several descent modes.

MODE_A – Energy Dissipation  
High altitude phase where excess velocity is reduced.

MODE_B – Controlled Descent  
The spacecraft follows a guided velocity profile.

MODE_C – Terminal Approach  
The controller prepares for the final landing phase.

MODE_D – Critical Braking  
Maximum braking effort is applied if necessary.

Each mode uses different controller parameters.


---

# Guidance Law

The target velocity during descent is defined by

v_target = -sqrt(2 * k_r * a_max * h)

where

k_r is a tuning parameter.

This produces a descent velocity profile that ensures safe energy dissipation.


---

# Control Law

The commanded acceleration is generated using a PD controller.

a_cmd = Kp (v_target - v) + Kd (dv/dt)

where

Kp = proportional gain  
Kd = derivative gain


---

# Thrust Command

The thrust command is calculated as

T = m (a_cmd + g)

where

m = spacecraft mass  
g = gravitational acceleration


Thrust is limited to the engine capability.


---

# Controller Characteristics

The controller includes several mechanisms to ensure stable landing behavior:

- adaptive gain scheduling
- gravity compensation
- thrust saturation handling
- phase-based descent logic


---

# Future Improvements

Possible improvements include:

- 3D landing guidance
- terrain-aware landing
- optimal control based descent
- reinforcement learning based autopilot
