# The Arena — C++ Multi-Entity Simulation System

An interactive multi-entity simulation system developed in C++ as part of the University of Minnesota's CSCI 3081W Program Design and Development course.

The project features robots, autonomous behaviors, energy management, environmental objects, and entity interactions within a configurable simulation environment.

## Technologies Used

- C++
- Object-Oriented Programming (OOP)
- JSON
- Git
- Docker
- Make
- Google Test

## Key Features

### 1. Multi-Entity Simulation

The simulation supports multiple entity types, including:

- Robot
- Braitenberg Vehicle
- Predator
- Light
- Water
- Energy

Entities are created from JSON configurations and updated through the simulation model.

### 2. Object-Oriented Design

The project uses object-oriented programming principles to organize different entities and movement behaviors.

Inheritance and polymorphism allow entities to share common functionality while supporting their own movement and interaction mechanisms.

### 3. Configurable Movement Behaviors

The simulation supports different movement strategies, including:

- Default movement
- Bouncing movement
- Keyboard-controlled movement
- Back-and-forth movement
- Circular movement
- Composite movement
- Inverse-distance-based movement

### 4. Braitenberg Vehicle Behaviors

Braitenberg Vehicles use sensor readings to respond to nearby objects and environmental stimuli.

Supported behaviors include:

- Explore
- Love
- Fear
- Aggression

Vehicle movement incorporates differential-drive kinematics and configurable behavior parameters.

### 5. Energy Management and Entity Interactions

Robots consume energy during movement and can recharge through interactions with energy entities.

Predators can interact with other robots through proximity-based collision detection and attack mechanisms.

These interactions create a dynamic simulation environment with different entity behaviors.

## Development Work

My work on this project focused on implementing and integrating simulation functionality in C++, including:

- Developing entity behavior and movement logic.
- Implementing JSON-based entity creation and configuration.
- Integrating different entities into the simulation model.
- Implementing robot energy management and entity interactions.
- Debugging simulation behavior and resolving integration issues.
- Building and testing the application throughout development.

The project was developed using a course-provided starter framework.

## Project Structure

```text
apps/       Application and web interface
bin/        Build and execution scripts
data/       JSON scene configurations
env/        Docker environment
gis/        Graphics and terrain support
include/    C++ header files
lib/        Libraries and dependencies
src/        Simulation implementation
tests/      Tests
```

## How to Build and Run

### Prerequisites

- Git
- Docker

### 1. Clone the repository

```bash
git clone https://github.com/austinhe7788-cpu/cpp-multi-entity-simulation.git
cd cpp-multi-entity-simulation
```

### 2. Build the Docker environment

```bash
bin/build-env.sh
```

### 3. Run the Docker environment

```bash
bin/run-env.sh
```

### 4. Build the project

Inside the Docker environment:

```bash
make
```

### 5. Run the simulation

```bash
make run
```

Open the following address in your browser:

http://127.0.0.1:8081/

## Example Simulation

The project includes different JSON scene configurations under:

```text
data/scenes/
```

For example, the Braitenberg Vehicle simulation can be launched using:

```bash
./bin/start.sh data/scenes/bv.json
```

## Acknowledgments

Developed as part of CSCI 3081W at the University of Minnesota, Twin Cities.

The repository builds upon course-provided starter code and includes third-party libraries and assets. Original copyright notices and attribution are retained in the source files.
