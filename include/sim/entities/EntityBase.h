/**
 * @file EntityBase.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef ENTITY_BASE_H_
#define ENTITY_BASE_H_

#include <memory>
#include <string>
#include <typeinfo>

#include "core/IEntity.h"
#include "sim/Vector3.h"

// Chia Chi Lin
#include "sim/motions/IMotion.h"

/**
 * @brief EntityBase is a base class that has basic functionality that is shared
 * across entities.
 */
class EntityBase : public IEntity {
 public:
  /**
   * @brief Default constructor.
   */
  EntityBase();

  /**
   * @brief Virtual destructor to enable proper polymorphic cleanup.
   */
  virtual ~EntityBase() {}

  /**
   * @brief Gets the name.
   * @return The entity name.
   */
  virtual const std::string& GetName() const { return name; }

  /**
   * @brief Gets the unique entity id.
   * @return The entity id.
   */
  virtual int GetId() const { return id; }

  /**
   * @brief Gets the entity's position.
   * @return Pointer to the position array.
   */
  virtual const double* GetPosition() const { return pos.GetArray(); }

  /**
   * @brief Gets the entity's direction as a unit vector with 3 components.
   * @return Pointer to the direction array.
   */
  virtual const double* GetDirection() const { return dir.GetArray(); }

  /**
   * @brief Gets the entity's current speed.
   * @return The current speed.
   */
  virtual double GetSpeed() const { return speed; }

  /**
   * @brief Gets the entity's radius.
   * @return The radius.
   */
  virtual double GetRadius() const { return radius; }

  /**
   * @brief Updates the entity based on a timestep.
   * @param dt A delta timestep from the last update.
   */
  virtual void Update(double dt) override;

  /**
   * @brief Sets the motion strategy for this entity.
   * @param motion The motion strategy to transfer into this entity.
   */
  void SetMotion(std::unique_ptr<IMotion> motion);

  /**
   * @brief Gets the current motion strategy.
   * @return Raw pointer to the current motion strategy, or nullptr if none.
   */
  IMotion* GetMotion() const;

  /**
   * @brief Returns information about the specific C++ type of entity.
   * This is used to efficiently check types rather than dynamic casting.
   * @return The C++ type information for this entity.
   */
  const std::type_info& GetType() const { return *type; }

  // By Chia Chi lin 0330
  /**
   * @brief Sets the entity position.
   * @param position_1 The new position.
   */
  void SetPosition(const Vector3& position_1) { pos = position_1; }

  /**
   * @brief Sets the entity direction.
   * @param direction_1 The new direction.
   */
  void SetDirection(const Vector3& direction_1) { dir = direction_1; }

  /**
   * @brief Sets the entity speed.
   * @param speed_1 The new speed.
   */
  void SetSpeed(double speed_1) { speed = speed_1; }

  /**
   * @brief Gets the entity position as a Vector3.
   * @return The current position vector.
   */
  Vector3 GetPositionVector() const { return pos; }

  /**
   * @brief Gets the entity direction as a Vector3.
   * @return The current direction vector.
   */
  Vector3 GetDirectionVector() const { return dir; }

 protected:
  /**
   * @brief Sets the runtime type information and display name.
   * @tparam T The concrete entity type.
   * @param typeName Human-readable entity type name.
   * @param currentObject Pointer to the current object instance.
   */
  template <typename T>
  void SetType(const std::string& typeName, T* currentObject) {
    static const std::type_info& type = typeid(T);
    this->type = &type;
    this->typeName = typeName;
    this->currentObject = currentObject;
    this->name = typeName + " " + std::to_string(id);
  }

  /**
   * @brief Sets the runtime type information and display name.
   * @tparam T The concrete entity type.
   * @param typeName Human-readable entity type name.
   */
  template <typename T>
  void SetType(const std::string& typeName) {
    static const std::type_info& type = typeid(T);
    this->type = &type;
    this->typeName = typeName;
    this->currentObject = this;
    this->name = typeName + " " + std::to_string(id);
  }

  /**
   * @brief Gets the stored type name string.
   * @return The entity type name.
   */
  virtual const std::string& GetTypeName() const { return typeName; }

  /**
   * @brief Returns the stored instance for composed / disguised entities.
   * @param type Requested type information.
   * @return The stored entity instance.
   */
  IEntity* GetInstance(const std::type_info& type) override {
    (void)type;
    return currentObject;
  }

 private:
  int id;
  const std::type_info* type;
  std::string typeName;
  IEntity* currentObject;

 protected:
  std::string name;
  Vector3 pos;
  Vector3 dir;
  double speed;
  double radius;

  /**
   * @brief Motion strategy used to update this entity.
   */
  std::unique_ptr<IMotion> motion_1;
};

#endif