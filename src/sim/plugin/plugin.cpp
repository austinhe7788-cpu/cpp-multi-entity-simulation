#include <algorithm>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <vector>

#include "sim/SimulationModel.h"

std::ostringstream strCout;
std::vector<IEntity*> standaloneEntities;

struct StandaloneEntityCleaner {
  ~StandaloneEntityCleaner() {
    for (IEntity* entity : standaloneEntities) {
      delete entity;
    }
    standaloneEntities.clear();
  }
};

StandaloneEntityCleaner standaloneCleaner;

extern "C" {

ISimulationModel* createSimulationModel() {
  std::cout.rdbuf(strCout.rdbuf());

  ISimulationModel* model = new SimulationModel();
  return model;
}

void deleteSimulationModel(ISimulationModel* model) {
  for (IEntity* entity : standaloneEntities) {
    delete entity;
  }
  standaloneEntities.clear();

  delete model;
}

void update(ISimulationModel* model, double dt) { model->Update(dt); }

void createScene(ISimulationModel* model, const char* input) {
  json data = json::parse(input);
  model->CreateScene(data);
}

void handleEvent(ISimulationModel* model, const char* event,
                 const char* input) {
  json data = json::parse(input);
  model->HandleEvent(std::string(event), data);
}

IEntity* createEntity(ISimulationModel* model, const char* input) {
  json data = json::parse(input);
  IEntity* entity = model->CreateEntity(data);

  if (entity) {
    standaloneEntities.push_back(entity);
  }

  return entity;
}

void deleteEntity(IEntity* entity) {
  if (!entity) {
    return;
  }

  auto it =
      std::find(standaloneEntities.begin(), standaloneEntities.end(), entity);
  if (it != standaloneEntities.end()) {
    standaloneEntities.erase(it);
  }

  delete entity;
}

void addEntity(ISimulationModel* model, IEntity* entity) {
  auto it =
      std::find(standaloneEntities.begin(), standaloneEntities.end(), entity);
  if (it != standaloneEntities.end()) {
    standaloneEntities.erase(it);
  }

  model->AddEntity(entity);
}

int getNumEntities(ISimulationModel* model) {
  return model->GetEntities().size();
}

IEntity* getEntity(ISimulationModel* model, int index) {
  return model->GetEntities()[index];
}

const char* getEntityName(IEntity* entity) { return entity->GetName().c_str(); }

void getEntityPosition(IEntity* entity, double* pos) {
  for (int i = 0; i < 3; i++) {
    pos[i] = entity->GetPosition()[i];
  }
}

void getEntityDirection(IEntity* entity, double* vel) {
  for (int i = 0; i < 3; i++) {
    vel[i] = entity->GetDirection()[i];
  }
}

double getEntityRadius(IEntity* entity) { return entity->GetRadius(); }

double getEntitySpeed(IEntity* entity) { return entity->GetSpeed(); }

int getEntityId(IEntity* entity) { return entity->GetId(); }

const char* getEntityType(IEntity* entity) { return entity->GetType().name(); }

void updateEntity(IEntity* entity, double dt) { entity->Update(dt); }
}
