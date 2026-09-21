#ifndef KEYBOARD_MOTION_H_
#define KEYBOARD_MOTION_H_

#include <string>
#include <vector>

#include "sim/motions/IMotion.h"

/**
 * @brief A motion strategy that moves an entity in response to keyboard input.
 */
class KeyboardMotion : public IMotion {
 public:
  /**
   * @brief Constructs a keyboard motion strategy.
   * @param speed_1 Movement speed while a key is pressed.
   * @param arenaWidth_1 The arena width.
   * @param arenaHeight_1 The arena height.
   */
  KeyboardMotion(double speed_1, double arenaWidth_1, double arenaHeight_1,
                 const std::vector<std::string>& keys = {});

  /**
   * @brief Updates an entity using the currently pressed keys.
   * @param entity The entity being updated.
   * @param dt The elapsed timestep.
   */
  void Update(EntityBase& entity, double dt) override;

  /**
   * @brief Marks a keyboard key as pressed.
   * @param key The key string from the frontend event.
   */
  void HandleKeyDown(const std::string& key);

  /**
   * @brief Marks a keyboard key as released.
   * @param key The key string from the frontend event.
   */
  void HandleKeyUp(const std::string& key);

 private:
  /**
   * @brief Returns true if the provided key name maps to the requested control.
   * @param key The frontend key string.
   * @param lower The lowercase single-letter key to compare against.
   * @param arrow The corresponding Arrow* key string.
   * @return Whether the key matches.
   */
  bool MatchesKey(const std::string& key, const std::string& expected,
                  const std::string& arrow) const;

  double speed_1;
  double arenaWidth_1;
  double arenaHeight_1;
  std::string upKey_1;
  std::string rightKey_1;
  std::string downKey_1;
  std::string leftKey_1;
  bool upPressed_1;
  bool downPressed_1;
  bool leftPressed_1;
  bool rightPressed_1;
};

#endif
