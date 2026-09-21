#include "gtest/gtest.h"
#include "sim/SimulationModel.h"

class ExampleTest : public ::testing::Test {
 public:
  void SetUp() { model = new SimulationModel(); }

  void TearDown() { delete model; }

 protected:
  ISimulationModel* model;
};

TEST_F(ExampleTest, CreateRobot) {
  std::string robotData =
      "{\"type\" : \"robot\", \"image\" : "
      "\"/assets/robot_pack/robot_red.png\",\"motion\": {\"type\": \"bounce\", "
      "\"speed\" : 100.0},\"position\" : [300.0, 100.0, 0], \"direction\" : "
      "[1.0, 1.0, 0], \"speed\": 100.0, \"radius\" : 30.0, \"capacity\": "
      "100.0}";
  json data = json::parse(robotData);
  IEntity* entity = model->CreateEntity(data);
  ASSERT_TRUE(entity) << "The Entity should exist";
  EXPECT_DOUBLE_EQ(entity->GetPosition()[0], 300.0)
      << "The x position of the entity should be near 300.";
}
