# Contributing to Moonlander

Thank you for your interest in contributing to Moonlander.

Moonlander is an open-source simulation framework for spacecraft physics,
guidance algorithms and control systems. The goal of the project is to
build a modular and extensible environment for experimenting with
spacecraft landing, control algorithms and simulation models.

Contributions are welcome from developers, students and researchers.

---

# Project Philosophy

Moonlander is designed as a modular engineering simulation.

The long-term goal is a scalable simulation framework where different
models, controllers and physics modules can be developed independently.

Every contributor directly influences how scalable and maintainable
the project becomes.

Please keep this in mind when proposing changes.

---

# Core Architecture Principles

## Modularity

All systems should follow a modular architecture.

Simulation components should be separated into clearly defined modules:

- physics models
- controllers
- sensors
- spacecraft models
- integrators
- visualization
- optimization models

Avoid tightly coupled implementations.

---

## Single Responsibility / Single Point of Use

Functions and classes should ideally follow a single responsibility principle.

A component should:

- have one clearly defined purpose
- avoid duplicated logic
- avoid hidden side effects

If functionality is used in multiple places, it should be implemented once
in a shared module.

---

## Interface Driven Design

Whenever possible, systems should be implemented behind interfaces.

Examples already used in the project include:

- IPhysicsModel
- IIntegrator
- ISensor
- IAutopilot

This allows models to be exchanged without modifying the simulation core.

---

# Units and Physical Consistency

All physical quantities must be expressed in SI units.

Examples:

Length → meters (m)  
Velocity → m/s  
Acceleration → m/s²  
Mass → kg  
Force → Newton (N)  
Time → seconds (s)

Units should always be clear from variable names or documentation.

---

# Documentation

Simulation software requires clear documentation.

## Doxygen

All public classes and important functions should contain Doxygen comments.

AI-assisted documentation generation is welcome as long as it remains
technically accurate.

---

## Architecture Documentation

When introducing new architectural components, contributors should:

1. briefly describe the design
2. explain the purpose of the component
3. open an issue so the architecture documentation can be updated

---

## Mathematical Models

When introducing new mathematical models:

- document the equations
- describe the variables
- explain the model assumptions

If necessary, open an issue to ensure the equations are documented on the project website.

---

# Issue Based Development

Moonlander follows a strict issue-based development process.

## Core Rule

NO CODE WITHOUT AN ISSUE.

Every development activity must be associated with a GitHub issue.

Reasons:

- development decisions remain traceable
- architectural changes are documented
- collaboration becomes easier

---

# Feature Development Workflow

If a contributor wants to implement a new feature or system, the following process must be used:

1. Create a GitHub Issue
2. Describe the idea or required change
3. Create a feature branch
4. Implement the solution based on the issue
5. Submit a Pull Request referencing the issue

Example:

Issue: Add gyroscope sensor model  
Branch: feature/gyroscope-sensor

Development should always happen on the issue branch.

---

# Commit Message Convention

To keep the development history structured, each commit must begin with
an issue identifier.

Format:

<KEY><NUMBER> - Issue description

Example:

D14 - Implement thrust saturation handling

---

# Issue Key Definitions

D  - Developer task  
     Implementation, code changes or small features.

M  - Mathematical model  
     Used when introducing or modifying formulas, guidance laws,
     optimization models or physics equations.

A  - Architecture design  
     Structural changes such as new modules, interfaces
     or major refactoring.

W  - Web / documentation task  
     Tasks affecting the project website or documentation repository.

P  - Project management  
     Planning, coordination, roadmap updates or repository organization.

F  - Feature proposal  
     Larger functional extensions.

B  - Bugfix  
     Fixing incorrect behaviour or runtime errors.

R  - Refactoring  
     Code restructuring without functional changes.

T  - Tests / validation  
     Testing models, controllers or simulation behaviour.

---

# Example Commit Messages

D21 - Implement gyroscope sensor model

M05 - Add energy based descent equation

A12 - Introduce integrator interface abstraction

W03 - Update controller documentation on website

P02 - Define roadmap for 3D simulation architecture

R08 - Refactor thrust control module

---

# Contribution Workflow

Typical workflow:

1. Fork the repository
2. Create a feature branch

git checkout -b feature/my-feature

3. Implement your changes
4. Commit using the issue key format
5. Submit a Pull Request referencing the issue

---

# Final Note

Moonlander is both a learning project and a technical experiment.

The goal is to create a clean, scalable simulation framework for
spacecraft dynamics and control systems.

Every contributor influences the architecture of the project.

Please keep clarity, modularity and scientific consistency
in mind when contributing.
