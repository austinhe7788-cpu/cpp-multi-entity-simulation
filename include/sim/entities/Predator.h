#ifndef PREDATOR_H_
#define PREDATOR_H_

#include <string>

#include "sim/entities/Robot.h"

class Predator : public Robot {
 public:
  Predator(Vector3 pos, Vector3 dir, double radius, double arenaWidth,
           double arenaHeight, const json& motion, double capacity,
           const json& disguise = json::object());

  void Update(double dt) override;

  const std::string& GetVisibleType() const;

  const std::string& GetDisguiseType() const { return disguiseType_1; }

  bool IsDisguised() const { return !revealed_1; }

  void Reveal();

  double Attack(Robot& target);

 protected:
  const std::string& GetTypeName() const override;

 private:
  std::string disguiseType_1;
  bool revealed_1;
};

#endif
