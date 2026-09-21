

#ifndef I_MOTION_H_
#define I_MOTION_H_

class EntityBase;

/**
 * @brief Strategy interface for entity motion behaviors.
 */
class IMotion {
 public:
  /**
   * @brief Virtual destructor for derived motion strategies.
   */
  virtual ~IMotion() {}

  /**
   * @brief Updates an entity according to this motion strategy.
   * @param entity The entity being updated.
   * @param dt The elapsed timestep.
   */
  virtual void Update(EntityBase& entity, double dt) = 0;
};

#endif