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
