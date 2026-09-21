/**
 * @file ISimulationModel.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef ISIMULATION_MODEL_H_
#define ISIMULATION_MODEL_H_

#include <vector>

#include "json.hpp"
using json = nlohmann::json;

#include "IEntity.h"

/**
 * @brief Represents an abstract simulation model that contains entities in a
 * scene.
 */
class ISimulationModel {
 public:
  /**
   * @brief Virtual destructor is needed for dynamic memory management in
   * inherited classes.
   */
  virtual ~ISimulationModel() {}

  /**
   * @brief Creates a scene for a simulation (e.g. sets boundaries and initial
   * entities).
   * @param data Data structure that stores scene data using json library.
   * @return Modifications to the scene based on the simulation.
   */
  virtual json CreateScene(const json& data) = 0;

  /**
   * @brief Updates a simulation by incrementing the time.
   * @param dt A time delta from the last time Update was called.
   */
  virtual void Update(double dt) = 0;

  /**
   * @brief Creates an entity based on json data structure.
   * @param data Data structure for an entity.
   */
  virtual IEntity* CreateEntity(const json& data) const = 0;

  /**
   * @brief Adds an entity to the simulation model.
   * @param entity An entity object.
   */
  virtual void AddEntity(IEntity* entity) = 0;

  /**
   * @brief Handles input events (e.g. keyboard or mouse input).  Events can
   * also be generic external input.
   * @param event The event name.
   * @param data Additional event data.
   */
  virtual void HandleEvent(const std::string& event, const json& data) = 0;

  /**
   * @brief Gets all the entities in the simulation.
   * @return A vector of entities.
   */
  virtual const std::vector<IEntity*>& GetEntities() const = 0;
};

#endif