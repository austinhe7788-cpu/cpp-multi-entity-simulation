#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "core/ISimulationModel.h"
#include "sim/SimulationModel.h"
#include "web_app.hpp"

struct SharedState {
  std::string sceneFile;
};

class WebAppController : public WebApp<SharedState> {
 public:
  WebAppController(SharedState& state) : WebApp<SharedState>(state) {
    model = new SimulationModel();
  }

  virtual ~WebAppController() { delete model; }

  void ReceiveCommand(const std::string& cmd, const json& data,
                      json& returnValue) {
    WebApp<SharedState>::ReceiveCommand(cmd, data, returnValue);
  }

  void Setup(json& returnValue) {
    std::ifstream f(state.sceneFile);
    std::stringstream buffer;
    buffer << f.rdbuf();
    json scene = json::parse(buffer.str());
    scene = model->CreateScene(scene);
    returnValue["scene"] = scene;
    width = scene["width"];
    height = scene["height"];
  }

  void Update(double dt, json& returnValue) {
    model->Update(dt);

    json entities = json::array();
    for (int i = 0; i < model->GetEntities().size(); i++) {
      IEntity* entity = model->GetEntities()[i];
      json e;
      json pos = json::array();
      pos.push_back(entity->GetPosition()[0]);
      pos.push_back(entity->GetPosition()[1]);
      pos.push_back(entity->GetPosition()[2]);
      json dir = json::array();
      dir.push_back(entity->GetDirection()[0]);
      dir.push_back(entity->GetDirection()[1]);
      dir.push_back(entity->GetDirection()[2]);
      e["p"] = pos;
      e["i"] = entity->GetId();
      e["r"] = entity->GetRadius();
      e["d"] = dir;
      e["s"] = entity->GetSpeed();
      entities.push_back(e);
    }

    returnValue["e"] = entities;
  }

  void KeyUp(const std::string& key, int keyCode) {
    json data;
    data["key"] = key;
    data["keyCode"] = keyCode;
    model->HandleEvent("KeyUp", data);
  }

  void KeyDown(const std::string& key, int keyCode) {
    json data;
    data["key"] = key;
    data["keyCode"] = keyCode;
    model->HandleEvent("KeyDown", data);
  }

  void MouseMove(float x, float y) {
    json data;
    data["x"] = x;
    data["y"] = y;
    model->HandleEvent("MouseMove", data);
  }

 private:
  ISimulationModel* model;
};

int main(int argc, char** argv) {
  if (argc < 4) {
    std::cout << "Usage: ./build/bin/sim <port> <web dir> <scene_file.json>"
              << std::endl;
    return 1;
  }

  int port = std::atoi(argv[1]);
  std::string webDir = std::string(argv[2]);
  SharedState state;
  state.sceneFile = std::string(argv[3]);
  WebServerWithState<WebAppController, SharedState> server(state, port, webDir);
  while (true) {
    server.service();
  }

  return 0;
}
