#include "sim/motions/KeyboardMotion.h"

#include "sim/entities/EntityBase.h"

KeyboardMotion::KeyboardMotion(double speed_1, double arenaWidth_1,
                               double arenaHeight_1,
                               const std::vector<std::string>& keys)
    : speed_1(speed_1),
      arenaWidth_1(arenaWidth_1),
      arenaHeight_1(arenaHeight_1),
      upKey_1("w"),
      rightKey_1("d"),
      downKey_1("s"),
      leftKey_1("a"),
      upPressed_1(false),
      downPressed_1(false),
      leftPressed_1(false),
      rightPressed_1(false) {
  if (keys.size() == 4) {
    upKey_1 = keys[0];
    rightKey_1 = keys[1];
    downKey_1 = keys[2];
    leftKey_1 = keys[3];
  }
}

void KeyboardMotion::Update(EntityBase& entity, double dt) {
  Vector3 movement(0.0, 0.0, 0.0);

  if (upPressed_1) {
    movement[1] += 1.0;
  }
  if (downPressed_1) {
    movement[1] -= 1.0;
  }
  if (leftPressed_1) {
    movement[0] -= 1.0;
  }
  if (rightPressed_1) {
    movement[0] += 1.0;
  }

  if (movement.Length() == 0.0) {
    entity.SetDirection(Vector3(0.0, 0.0, 0.0));
    entity.SetSpeed(0.0);
    return;
  }

  Vector3 direction = movement.Normalized();
  Vector3 position = entity.GetPositionVector() + direction * speed_1 * dt;
  double radius = entity.GetRadius();

  if (position[0] < radius) {
    position[0] = radius;
  } else if (position[0] > arenaWidth_1 - radius) {
    position[0] = arenaWidth_1 - radius;
  }

  if (position[1] < radius) {
    position[1] = radius;
  } else if (position[1] > arenaHeight_1 - radius) {
    position[1] = arenaHeight_1 - radius;
  }

  entity.SetPosition(position);
  entity.SetDirection(direction);
  entity.SetSpeed(speed_1);
}

void KeyboardMotion::HandleKeyDown(const std::string& key) {
  if (MatchesKey(key, upKey_1, "ArrowUp")) {
    upPressed_1 = true;
  } else if (MatchesKey(key, downKey_1, "ArrowDown")) {
    downPressed_1 = true;
  } else if (MatchesKey(key, leftKey_1, "ArrowLeft")) {
    leftPressed_1 = true;
  } else if (MatchesKey(key, rightKey_1, "ArrowRight")) {
    rightPressed_1 = true;
  }
}

void KeyboardMotion::HandleKeyUp(const std::string& key) {
  if (MatchesKey(key, upKey_1, "ArrowUp")) {
    upPressed_1 = false;
  }

  else if (MatchesKey(key, downKey_1, "ArrowDown")) {
    downPressed_1 = false;
  } else if (MatchesKey(key, leftKey_1, "ArrowLeft")) {
    leftPressed_1 = false;
  } else if (MatchesKey(key, rightKey_1, "ArrowRight")) {
    rightPressed_1 = false;
  }
}

bool KeyboardMotion::MatchesKey(const std::string& key,
                                const std::string& expected,
                                const std::string& arrow) const {
  if (key == arrow || key == expected) {
    return true;
  }

  if (key.size() == 1 && expected.size() == 1) {
    char lower = expected[0];
    return key[0] == lower || key[0] == lower - ('a' - 'A');
  }

  return false;
}
