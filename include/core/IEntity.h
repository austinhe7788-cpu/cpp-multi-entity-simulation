/**
 * @file IEntity.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef IENTITY_H_
#define IENTITY_H_

#include <string>
#include <typeinfo>

// Json library
#include "json.hpp"
using json = nlohmann::json;

/**
 * @brief The IEntity interface represents an abstract entity used within a
 * simulation.
 */
class IEntity {
 public:
  /**
   * @brief Virtual destructor for dynamic memory of inherited classes.
   */
  virtual ~IEntity() {}

  /**
   * @brief Gets the name.
   */
  virtual const std::string& GetName() const = 0;

  /**
   * @brief Gets the unique entity id.
   */
  virtual int GetId() const = 0;

  /**
   * @brief Gets the entity's position.
   */
  virtual const double* GetPosition() const = 0;

  /**
   * @brief Gets the entity's direction as a unit vector with 3 components.
   */
  virtual const double* GetDirection() const = 0;

  /**
   * @brief Gets the entitiy's current speed.
   */
  virtual double GetSpeed() const = 0;

  /**
   * @brief Gets the entitiy's radius.
   */
  virtual double GetRadius() const = 0;

  /**
   * @brief Updates the entity based on a timestep.
   * @param dt A delta timestep from the last update.
   */
  virtual void Update(double dt) = 0;

  /**
   * @brief Returns information about the specific C++ type of entity.
   * This is used to efficiently check types rather than dynamic casting.
   */
  virtual const std::type_info& GetType() const = 0;

  /**
   * @brief Returns whether or not the entity is of type T.
   * @return True if the entity is of type T (defined by polymorphic method
   * GetType()), false otherwise.
   */
  template <typename T>
  bool IsType() const {
    static const std::type_info& type = typeid(T);
    return GetType() == type;
  }

  /**
   * @brief Uses a static cast to change the entity type if is of that type.
   * @return nullptr if the entity is not type T and the entity as type T if it
   * is.
   */
  template <typename T>
  T* AsType() {
    static const std::type_info& type = typeid(T);
    if (IsType<T>()) {
      return static_cast<T*>(GetInstance(type));
    }

    return nullptr;
  }

 protected:
  /**
   * @brief Polymorphic method that returns the underlying instance of an
   * object. This is useful if we are using composition (e.g. decorator, proxy,
   * or adapter pattern) and we want to cast to an underlying different type.
   * @param type Specific informaiton about the type that we are requesting.
   * @return The instance of the type we would like to use.
   */
  virtual IEntity* GetInstance(const std::type_info& type) { return this; }
};

#endif