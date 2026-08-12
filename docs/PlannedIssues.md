# B04 - Fix Missing Attitude Coupling of Propulsion Forces

## Problem

With the introduction of rotational rigid-body dynamics, the spacecraft attitude is now propagated as part of the 6DoF state.

However, propulsion forces are currently not correctly coupled to the spacecraft attitude.

The issue is clearly observable for the main engine:

The spacecraft can rotate significantly while the main-engine thrust direction remains unchanged relative to the MCI frame.

This is physically incorrect for an engine rigidly attached to the spacecraft.

---

## Physical Background

Engine thrust directions are defined relative to the spacecraft and therefore belong to the Spacecraft Body-Fixed Frame (SBF).

A thrust vector

\[
\mathbf{F}_{SBF}
\]

must be transformed into the inertial frame using the current spacecraft attitude:

\[
\mathbf{F}_{MCI}
=
\mathbf{R}_{MCI \leftarrow SBF}(q)
\mathbf{F}_{SBF}
\]

where:

- \(\mathbf{F}_{SBF}\) is the thrust vector expressed in SBF
- \(\mathbf{F}_{MCI}\) is the thrust vector expressed in MCI
- \(q\) is the current spacecraft attitude quaternion
- \(\mathbf{R}_{MCI \leftarrow SBF}(q)\) is the attitude-dependent rotation from SBF to MCI

The thrust direction must remain fixed relative to the spacecraft, but rotate relative to MCI as the spacecraft attitude changes.

---

## Current Behavior

The rotational state of the spacecraft changes correctly and significant spacecraft rotation can be observed.

However, the main-engine thrust direction remains effectively fixed in MCI.

Therefore, rotational and translational dynamics are currently not fully coupled.

The current behavior is effectively:

\[
\mathbf{F}_{MCI}
\neq
\mathbf{R}_{MCI \leftarrow SBF}(q)
\mathbf{F}_{SBF}
\]

or the current spacecraft attitude is not being applied at the appropriate point in the propulsion-to-physics pipeline.

---

## Expected Behavior

For every spacecraft-fixed engine:

- thrust direction is defined in SBF
- thrust direction remains constant relative to the spacecraft
- current spacecraft attitude determines the corresponding direction in MCI
- spacecraft rotation therefore changes the inertial propulsion-force direction
- translational acceleration responds to the rotated propulsion vector

For the main engine this means that a tilted spacecraft must accelerate in the corresponding tilted direction.

The same principle must apply to RCS forces.

---

## Required Changes

Investigate and correct the propulsion-force transformation pipeline.

Expected data flow:

SBF Engine Direction

→ SBF Thrust Vector

→ Current Spacecraft Attitude

→ SBF-to-MCI Transformation

→ MCI Force Aggregation

→ Translational Acceleration

→ Numerical Integration

The current spacecraft quaternion must be applied before spacecraft-fixed propulsion forces contribute to the inertial translational dynamics.

---

## Tasks

- Identify the current reference frame of main-engine thrust output
- Identify the current reference frame of RCS thrust output
- Ensure engine directions are treated as spacecraft-fixed SBF quantities
- Apply the current spacecraft attitude to transform propulsion forces from SBF to MCI
- Verify transformation direction and quaternion convention
- Update main-engine force handling
- Update RCS force handling where required
- Ensure force aggregation occurs in a consistent reference frame
- Ensure torque calculations remain expressed in the intended SBF frame
- Prevent visualization-specific coordinate transformations from entering the physics pipeline
- Add diagnostic output for SBF and MCI propulsion vectors
- Add regression tests for known spacecraft attitudes

---

## Regression Tests

### Identity Attitude

For identity attitude:

\[
q = [1,0,0,0]
\]

the transformed thrust vector must correspond to the expected initial SBF/MCI alignment.

---

### 90° Rotation

Apply a known 90° spacecraft rotation around a principal axis.

The MCI thrust vector must rotate by the corresponding amount.

---

### Main Engine During Rotation

Run the main engine while the spacecraft has a non-zero angular velocity.

Expected:

- \(\mathbf{F}_{SBF}\) remains constant
- \(\mathbf{F}_{MCI}\) changes continuously with attitude
- MCI acceleration changes accordingly

---

### RCS at Non-Zero Attitude

Activate an individual RCS thruster while the spacecraft has a known non-identity attitude.

Both the resulting translational force and rotational moment must remain physically consistent with the same thruster geometry.

---

## Acceptance Criteria

- Main-engine thrust follows spacecraft attitude correctly
- RCS thrust vectors follow spacecraft attitude correctly
- SBF propulsion vectors are transformed into MCI using the current attitude
- Translational acceleration responds correctly to spacecraft orientation
- Force aggregation uses explicitly defined and consistent reference frames
- Torque calculations remain frame-consistent
- Known-angle transformation tests produce the analytically expected results
- No spacecraft-fixed propulsion force remains artificially fixed in MCI during spacecraft rotation

---

## Milestone Relevance

This bug blocks completion of:

**M1 - 6DoF Core Simulation**

The rotational and translational dynamics currently exist, but they are not yet fully coupled through spacecraft attitude.

Resolving this bug establishes the required physical chain:

Attitude

→ Spacecraft-Fixed Propulsion Direction

→ MCI Force Vector

→ Translational Acceleration

and is therefore required for a physically consistent 6DoF spacecraft simulation.

_________________________________________________________________________________________________________

# D30 - Develop 3D Attitude View

## Objective

Implement a dedicated 3D attitude visualization for the spacecraft cockpit.

The current cockpit can display rotational state numerically, but arbitrary spacecraft orientation cannot be represented intuitively through scalar attitude values alone.

The new 3D Attitude View shall visualize the spacecraft orientation directly from the quaternion-based 6DoF state.

The visualization must remain a pure frontend component and must not introduce independent physics or state propagation.

---

## Background

SDF now propagates a complete rotational spacecraft state including:

- angular velocity
- angular acceleration
- quaternion-based attitude

The cockpit can expose these quantities numerically, but the current 2D/2.5D Landing View is not suitable for displaying arbitrary three-dimensional spacecraft orientation.

A dedicated 3D attitude representation is therefore required.

The first implementation should remain intentionally small and isolated. The goal is not to replace the existing Landing View or develop a complete 3D landing environment.

---

## Scope

The initial 3D Attitude View shall provide:

- a fixed 3D camera
- a visible spacecraft or asymmetric test body
- quaternion-driven orientation
- real-time updates from cockpit telemetry
- clear visual reference for spacecraft roll, pitch, and yaw behavior
- consistent mapping between SDF coordinate conventions and the rendering coordinate system

The first implementation does not require:

- lunar terrain
- spacecraft translation
- landing target visualization
- trajectory rendering
- engine plume effects
- RCS plume effects
- free camera movement
- external 3D scene physics

These capabilities may be introduced in a later full 3D Landing View issue.

---

## Architecture

The intended data flow is:

SDF Backend State

→ TelemetryDTO

→ Cockpit Frontend

→ Attitude View

→ 3D Renderer

The authoritative spacecraft orientation remains the backend quaternion.

The frontend shall only transform the quaternion into the representation required by the rendering framework.

No attitude integration or physical calculation shall be performed by the 3D view.

---

## Rendering Technology

Evaluate and integrate Qt Quick 3D within the existing Qt Widgets cockpit.

The preferred architecture is:

Cockpit QWidget

→ AttitudeView wrapper

→ QQuickWidget

→ QML View3D

→ spacecraft model

This allows 3D rendering to be introduced without migrating the complete cockpit frontend to QML.

---

## Implementation Tasks

- [ ] Add required Qt Quick / Qt Quick 3D dependencies
- [ ] Create a minimal QML-based 3D scene
- [ ] Embed the QML scene into the existing QWidget cockpit
- [ ] Create an `AttitudeView` frontend wrapper
- [ ] Render a simple asymmetric test body
- [ ] Verify static 3D rendering
- [ ] Verify manual test rotations around all three axes
- [ ] Convert the SDF attitude quaternion to the Qt quaternion representation
- [ ] Connect cockpit telemetry to the 3D view
- [ ] Apply the live quaternion directly to the rendered spacecraft node
- [ ] Verify real-time spacecraft attitude updates
- [ ] Document the mapping between SDF and renderer coordinate frames
- [ ] Correct axis or handedness differences only inside the visualization layer
- [ ] Replace the test body with a spacecraft model if the basic pipeline is stable

---

## Coordinate System Verification

The 3D rendering coordinate system must be compared explicitly with the SDF spacecraft body-fixed frame.

Verify:

- [ ] SDF +X rotation
- [ ] SDF -X rotation
- [ ] SDF +Y rotation
- [ ] SDF -Y rotation
- [ ] SDF +Z rotation
- [ ] SDF -Z rotation
- [ ] positive and negative rotation direction
- [ ] spacecraft forward direction
- [ ] spacecraft up direction
- [ ] quaternion multiplication / orientation convention
- [ ] renderer handedness

Any required visualization-frame transformation must remain inside the frontend.

The physics state must not be modified to compensate for rendering conventions.

---

## Reference Test Cases

### Test 1 — Identity Attitude

Input:

`q = [1, 0, 0, 0]`

Expected:

- spacecraft displayed in defined neutral orientation
- no unintended rotation

---

### Test 2 — Single-Axis Rotation

Apply known spacecraft rotations independently around X, Y, and Z.

Expected:

- correct rendered axis
- correct rotation direction
- no axis swapping

---

### Test 3 — Continuous Rotation

Provide a continuously changing quaternion from the simulation.

Expected:

- smooth visual attitude propagation
- no independent frontend integration
- no discontinuities caused by Euler-angle conversion

---

### Test 4 — Translational RCS Torque

Operate existing translational RCS thrusters that generate off-center torque.

Expected:

- visible spacecraft rotation consistent with backend attitude
- observed tumbling corresponds to the simulated quaternion state

---

## Acceptance Criteria

The issue is complete when:

- a 3D attitude widget is integrated into the existing Qt cockpit,
- the rendered spacecraft orientation is driven directly by the backend attitude quaternion,
- no Euler-angle conversion is required for the visual orientation pipeline,
- all three rotational axes are displayed correctly,
- sign and axis conventions match the SDF coordinate definitions,
- the frontend contains no independent rotational physics,
- the visualization updates in real time with cockpit telemetry,
- the component can later serve as the technical basis for a full 3D Landing View.

---

## Result

SDF provides a dedicated real-time 3D visualization of spacecraft attitude.

The new component makes the rotational 6DoF state directly observable and provides both an engineering visualization tool and the foundation for future full 3D spacecraft simulation views.
