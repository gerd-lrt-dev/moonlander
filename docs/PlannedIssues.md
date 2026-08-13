B05 - Include Rotational Contributions in Pilot G-Load Calculation

## Problem

The current G-load calculation only considers translational acceleration.

With the introduction of full 6DoF rigid-body dynamics, this is no longer sufficient to represent the total acceleration experienced by a pilot or onboard payload.

A point inside a rotating spacecraft can experience additional acceleration caused by angular motion, even when the spacecraft center of mass does not undergo corresponding translational acceleration.

The current G-load value therefore underestimates the actual local load acting on the pilot during rotational maneuvers.

---

## Physical Background

The acceleration of a point rigidly attached to a rotating spacecraft can be written as:

a_point =
a_COM
+
alpha × r
+
omega × (omega × r)

where:

- a_COM is the translational acceleration of the spacecraft center of mass
- alpha is the spacecraft angular acceleration
- omega is the spacecraft angular velocity
- r is the position vector from the spacecraft center of mass to the pilot or evaluation point
- alpha × r represents tangential acceleration
- omega × (omega × r) represents centripetal acceleration

For a fixed pilot position in the spacecraft body frame, these rotational terms contribute directly to the local acceleration experienced by the pilot.

---

## Current Behavior

The current G-load calculation is based only on translational acceleration and gravity compensation.

Rotational contributions from:

- angular acceleration
- angular velocity
- pilot offset from the spacecraft center of mass

are not currently considered.

As a result, aggressive rotational maneuvers may produce high angular loads without affecting the reported G-load.

---

## Expected Behavior

The reported pilot G-load should represent the local proper acceleration at a defined point inside the spacecraft.

The calculation should therefore include:

- translational proper acceleration
- tangential acceleration due to angular acceleration
- centripetal acceleration due to angular velocity

The resulting local acceleration should be converted into multiples of standard Earth gravity.

---

## Required Changes

Extend the G-load model to support a configurable evaluation point representing the pilot or another relevant onboard location.

The calculation should include rotational rigid-body acceleration terms in addition to the existing translational contribution.

The implementation must explicitly define the coordinate frame used for:

- pilot position vector
- angular velocity
- angular acceleration
- translational acceleration
- resulting local acceleration

All quantities must be transformed into a common frame before aggregation.

---

## Configuration

Add or prepare support for a configurable pilot position relative to the spacecraft center of mass.

Example:

pilotPositionSBF = [x, y, z]

Unit:

meters [m]

If no pilot position is configured, the default may remain the spacecraft center of mass.

At the center of mass:

r = 0

and therefore rotational acceleration contributions vanish.

---

## Tasks

- [ ] Review the current G-load implementation
- [ ] Define the physical meaning of the reported G-load
- [ ] Add pilot / evaluation-point position relative to spacecraft center of mass
- [ ] Include tangential acceleration from angular acceleration
- [ ] Include centripetal acceleration from angular velocity
- [ ] Verify all required coordinate transformations
- [ ] Preserve existing gravity / proper-acceleration handling
- [ ] Define behavior when the evaluation point is located at the center of mass
- [ ] Add telemetry support if additional load components should be exposed
- [ ] Add regression tests for rotational load cases

---

## Reference Test Cases

### Test 1 — Evaluation Point at Center of Mass

Set:

r = 0

Expected:

Rotational contributions are zero.

The result must match the current translational G-load calculation.

---

### Test 2 — Constant Angular Velocity

Set:

alpha = 0

omega != 0

r != 0

Expected:

The pilot experiences centripetal acceleration:

a_rot = omega × (omega × r)

---

### Test 3 — Angular Acceleration

Set:

omega = 0

alpha != 0

r != 0

Expected:

The pilot experiences tangential acceleration:

a_rot = alpha × r

---

### Test 4 — Combined 6DoF Maneuver

Apply simultaneous translational and rotational acceleration.

Expected:

The reported local G-load reflects the combined proper acceleration acting at the configured pilot position.

---

## Acceptance Criteria

- G-load calculation includes rotational acceleration effects
- pilot position relative to the spacecraft center of mass can be defined
- tangential and centripetal acceleration are included correctly
- all acceleration components are evaluated in a consistent coordinate frame
- the center-of-mass case reproduces the previous translational result
- rotational maneuvers produce physically plausible G-load changes
- the calculation can later be used as an input for maneuver and structural load limits

---

## Future Relevance

A physically consistent local G-load model is required for future:

- pilot load limits
- maneuver limiting
- structural load monitoring
- controller constraints
- human-rated simulation scenarios
- automated safety logic

This bugfix therefore extends the existing 6DoF dynamics into a physically meaningful onboard load model.
