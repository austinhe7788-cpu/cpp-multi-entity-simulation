#include "sim/SimulationModel.h"

#include "sim/entities/BraitenbergVehicle.h"
#include "sim/entities/Energy.h"
#include "sim/entities/Light.h"
#include "sim/entities/Predator.h"
#include "sim/entities/Robot.h"
#include "sim/entities/Water.h"
#include "sim/motions/BraitenbergBehaviorMotion.h"
#include "sim/motions/InverseDistanceMotion.h"
#include "sim/motions/KeyboardMotion.h"
SimulationModel::SimulationModel() {}

SimulationModel::~SimulationModel() {
  for (IEntity* entity : entities) {
    delete entity;
  }
  entities.clear();
}

json SimulationModel::CreateScene(const json& data) {
  json returnVal = data;

  width = returnVal["width"];
  height = returnVal["height"];
  json& entities = returnVal["entities"];
  for (int i = 0; i < entities.size(); i++) {
    IEntity* entity = CreateEntity(entities[i]);
    int id = -1;

    if (entity) {
      AddEntity(entity);
      id = entity->GetId();

      Predator* predator = dynamic_cast<Predator*>(entity);
      if (predator) {
        std::string visibleType = predator->GetVisibleType();
        if (visibleType == "bv") {
          visibleType = "braitenbergvehicle";
        }
        entities[i]["type"] = visibleType;
      }
    }

    entities[i]["id"] = id;
  }

  return returnVal;
}

void SimulationModel::Update(double dt) {
  auto& ents = GetEntities();

  for (IEntity* entity : ents) {
    EntityBase* base = dynamic_cast<EntityBase*>(entity);
    if (base && base->GetMotion()) {
      if (auto* behaviorMotion =
              dynamic_cast<BraitenbergBehaviorMotion*>(base->GetMotion())) {
        behaviorMotion->SetEntities(&ents);
      }
    }

    entity->Update(dt);
  }

  for (IEntity* entity : ents) {
    Predator* predator = dynamic_cast<Predator*>(entity);
    if (!predator) {
      continue;
    }

    for (IEntity* other : ents) {
      Robot* prey = dynamic_cast<Robot*>(other);
      if (!prey || prey == predator) {
        continue;
      }
      if (dynamic_cast<Predator*>(other)) {
        continue;
      }

      Vector3 p1(predator->GetPosition()[0], predator->GetPosition()[1],
                 predator->GetPosition()[2]);
      Vector3 p2(prey->GetPosition()[0], prey->GetPosition()[1],
                 prey->GetPosition()[2]);

      double contact = predator->GetRadius() + prey->GetRadius();
      if ((p1 - p2).Length() <= contact) {
        predator->Attack(*prey);
      }
    }
  }

  for (IEntity* entity : ents) {
    Robot* robot = dynamic_cast<Robot*>(entity);
    if (!robot) {
      continue;
    }

    for (IEntity* other : ents) {
      Energy* energy = dynamic_cast<Energy*>(other);
      if (!energy) {
        continue;
      }

      Vector3 p1(robot->GetPosition()[0], robot->GetPosition()[1],
                 robot->GetPosition()[2]);
      Vector3 p2(energy->GetPosition()[0], energy->GetPosition()[1],
                 energy->GetPosition()[2]);

      double contact = robot->GetRadius() + energy->GetRadius();
      if ((p1 - p2).Length() <= contact) {
        robot->Recharge(energy->GetIntensity());
      }
    }
  }
}

IEntity* SimulationModel::CreateEntity(const json& data) const {
  if (data["type"] == "robot") {
    json pos = data["position"];
    json dir = data["direction"];

    double capacity_1 = 100.0;
    if (data.contains("capacity")) {
      capacity_1 = data["capacity"];
    }

    return new Robot(Vector3(pos[0], pos[1], pos[2]),
                     Vector3(dir[0], dir[1], dir[2]).Normalized(),
                     data["radius"], width, height, data["motion"], capacity_1);
  }
  if (data["type"] == "water") {
    json pos = data["position"];
    json dir = data["direction"];
    return new Water(Vector3(pos[0], pos[1], pos[2]),
                     Vector3(dir[0], dir[1], dir[2]).Normalized(),
                     data["radius"], width, height, data["motion"]);
  }
  if (data["type"] == "light") {
    json pos = data["position"];
    json dir = data["direction"];
    return new Light(Vector3(pos[0], pos[1], pos[2]),
                     Vector3(dir[0], dir[1], dir[2]).Normalized(),
                     data["radius"], width, height, data["motion"]);
  }
  if (data["type"] == "energy") {
    json pos = data["position"];
    json dir = data["direction"];
    return new Energy(Vector3(pos[0], pos[1], pos[2]),
                      Vector3(dir[0], dir[1], dir[2]).Normalized(),
                      data["radius"], width, height, data["motion"]);
  }
  if (data["type"] == "bv") {
    json pos = data["position"];
    json dir = data["direction"];

    double capacity_1 = 100.0;
    if (data.contains("capacity")) {
      capacity_1 = data["capacity"];
    }

    return new BraitenbergVehicle(Vector3(pos[0], pos[1], pos[2]),
                                  Vector3(dir[0], dir[1], dir[2]).Normalized(),
                                  data["radius"], width, height, data["motion"],
                                  capacity_1);
  }
  if (data["type"] == "predator") {
    json pos = data["position"];
    json dir = data["direction"];

    double capacity_1 = 100.0;
    if (data.contains("capacity")) {
      capacity_1 = data["capacity"];
    }

    json disguise_1 = json::object();
    if (data.contains("disguise")) {
      disguise_1 = data["disguise"];
    }

    return new Predator(Vector3(pos[0], pos[1], pos[2]),
                        Vector3(dir[0], dir[1], dir[2]).Normalized(),
                        data["radius"], width, height, data["motion"],
                        capacity_1, disguise_1);
  }

  return nullptr;
}

void SimulationModel::AddEntity(IEntity* entity) { entities.push_back(entity); }

void SimulationModel::HandleEvent(const std::string& event, const json& data) {
  if (event == "KeyUp" || event == "KeyDown") {
    if (!data.contains("key")) {
      return;
    }

    const std::string key = data["key"];
    for (IEntity* entity : entities) {
      EntityBase* baseEntity = dynamic_cast<EntityBase*>(entity);
      if (!baseEntity || !baseEntity->GetMotion()) {
        continue;
      }

      KeyboardMotion* keyboardMotion =
          dynamic_cast<KeyboardMotion*>(baseEntity->GetMotion());
      if (!keyboardMotion) {
        continue;
      }

      if (event == "KeyDown") {
        keyboardMotion->HandleKeyDown(key);
      } else {
        keyboardMotion->HandleKeyUp(key);
      }
    }
  }
}
