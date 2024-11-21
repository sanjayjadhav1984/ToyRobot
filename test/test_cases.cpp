#include "gtest/gtest.h"
#include "ToyRobot.h"

// Test ToyRobot
TEST(ToyRobotTest, isNotPlacedInitially) {
    ToyRobot robot;
    EXPECT_EQ(robot.x, -1);
    EXPECT_EQ(robot.y, -1);
    EXPECT_EQ(robot.facing, "");
    EXPECT_FALSE(robot.isPlaced());
}

// Test PlaceCommand
TEST(PlaceCommandTest, ValidPlacement) {
    ToyRobot robot;
    auto cmd = std::make_unique<PlaceCommand>(2, 2, "NORTH");
    cmd->execute(robot);

    EXPECT_EQ(robot.x, 2);
    EXPECT_EQ(robot.y, 2);
    EXPECT_EQ(robot.facing, "NORTH");
}

TEST(PlaceCommandTest, InvalidPlacement) {
    ToyRobot robot;
    auto cmd = std::make_unique<PlaceCommand>(-1, 2, "NORTH");
    cmd->execute(robot);

    EXPECT_EQ(robot.x, -1); // Robot should not be placed
    EXPECT_EQ(robot.y, -1);
    EXPECT_EQ(robot.facing, "");
}

TEST(PlaceCommandTest, ParseValidCommand) {
    std::string command = "PLACE 1,2,SOUTH";
    auto cmd = PlaceCommand::parse(command);

    EXPECT_NE(cmd, nullptr); // Command should be parsed successfully
    ToyRobot robot;
    cmd->execute(robot);

    EXPECT_EQ(robot.x, 1);
    EXPECT_EQ(robot.y, 2);
    EXPECT_EQ(robot.facing, "SOUTH");
}

TEST(PlaceCommandTest, ParseInvalidCommand) {
    std::string command = "PLACE 1,2";
    auto cmd = PlaceCommand::parse(command);

    EXPECT_EQ(cmd, nullptr); // Invalid command should return nullptr
}

// Test MoveCommand
TEST(MoveCommandTest, MoveNorth) {
    ToyRobot robot;
    robot.x = 1;
    robot.y = 1;
    robot.facing = "NORTH";

    auto cmd = std::make_unique<MoveCommand>();
    cmd->execute(robot);

    EXPECT_EQ(robot.x, 1);
    EXPECT_EQ(robot.y, 2);
}

TEST(MoveCommandTest, PreventOutOfBoundsMove) {
    ToyRobot robot;
    robot.x = 0;
    robot.y = 4;
    robot.facing = "NORTH";

    auto cmd = std::make_unique<MoveCommand>();
    cmd->execute(robot);

    EXPECT_EQ(robot.x, 0);
    EXPECT_EQ(robot.y, 4); // Should not move out of bounds
}

// Test LeftCommand
TEST(LeftCommandTest, TurnLeft) {
    ToyRobot robot;
    robot.x = 0;
    robot.y = 0;
    robot.facing = "NORTH";

    auto cmd = std::make_unique<LeftCommand>();
    cmd->execute(robot);

    EXPECT_EQ(robot.facing, "WEST");
}

// Test RightCommand
TEST(RightCommandTest, TurnRight) {
    ToyRobot robot;
    robot.x = 0;
    robot.y = 0;
    robot.facing = "NORTH";

    auto cmd = std::make_unique<RightCommand>();
    cmd->execute(robot);

    EXPECT_EQ(robot.facing, "EAST");
}

// Test ReportCommand
TEST(ReportCommandTest, ReportPosition) {
    ToyRobot robot;
    robot.x = 1;
    robot.y = 2;
    robot.facing = "EAST";

    auto cmd = std::make_unique<ReportCommand>();
    testing::internal::CaptureStdout(); // Capture stdout for testing
    cmd->execute(robot);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "1,2,EAST\n");
}

TEST(ReportCommandTest, ReportUnplacedRobot) {
    ToyRobot robot;

    auto cmd = std::make_unique<ReportCommand>();
    testing::internal::CaptureStderr(); // Capture stderr for testing
    cmd->execute(robot);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(output, "Robot is not placed on the grid.\n");
}

// Test PlaceCommand through CommandFactory
TEST(CommandFactoryTest, PlaceCommandValid) {
    std::string input = "PLACE 2,3,NORTH";
    auto cmd = CommandFactory::createCommand(input);

    EXPECT_NE(cmd, nullptr); // Command should be created successfully

    ToyRobot robot;
    cmd->execute(robot);

    EXPECT_EQ(robot.x, 2);
    EXPECT_EQ(robot.y, 3);
    EXPECT_EQ(robot.facing, "NORTH");
    EXPECT_TRUE(robot.isPlaced());
}

TEST(CommandFactoryTest, PlaceCommandInvalid) {
    std::string input = "PLACE -1,5,WEST";
    auto cmd = CommandFactory::createCommand(input);

    EXPECT_NE(cmd, nullptr); // Command is created, but execution fails

    ToyRobot robot;
    cmd->execute(robot);

    EXPECT_EQ(robot.x, -1); // Robot is not placed
    EXPECT_EQ(robot.y, -1);
    EXPECT_EQ(robot.facing, "");
    EXPECT_FALSE(robot.isPlaced());
}

TEST(CommandFactoryTest, PlaceCommandMalformed) {
    std::string input = "PLACE 1,2";
    auto cmd = CommandFactory::createCommand(input);

    EXPECT_EQ(cmd, nullptr); // Malformed command returns nullptr
}

// Test MoveCommand
TEST(CommandFactoryTest, MoveCommand) {
    ToyRobot robot;
    robot.x = 2;
    robot.y = 2;
    robot.facing = "NORTH";

    auto cmd = CommandFactory::createCommand("MOVE");
    cmd->execute(robot);

    EXPECT_EQ(robot.x, 2);
    EXPECT_EQ(robot.y, 3);
}

TEST(CommandFactoryTest, MoveCommandOutOfBounds) {
    ToyRobot robot;
    robot.x = 0;
    robot.y = 4;
    robot.facing = "NORTH";

    auto cmd = CommandFactory::createCommand("MOVE");
    cmd->execute(robot);

    EXPECT_EQ(robot.x, 0);
    EXPECT_EQ(robot.y, 4); // Should not move out of bounds
}

// Test LeftCommand
TEST(CommandFactoryTest, LeftCommand) {
    ToyRobot robot;
    robot.x = 1;
    robot.y = 1;
    robot.facing = "NORTH";

    auto cmd = CommandFactory::createCommand("LEFT");
    cmd->execute(robot);

    EXPECT_EQ(robot.facing, "WEST");
}

// Test RightCommand
TEST(CommandFactoryTest, RightCommand) {
    ToyRobot robot;
    robot.x = 1;
    robot.y = 1;
    robot.facing = "NORTH";

    auto cmd = CommandFactory::createCommand("RIGHT");
    cmd->execute(robot);

    EXPECT_EQ(robot.facing, "EAST");
}

// Test ReportCommand
TEST(CommandFactoryTest, ReportCommandValid) {
    ToyRobot robot;
    robot.x = 1;
    robot.y = 2;
    robot.facing = "EAST";

    auto cmd = CommandFactory::createCommand("REPORT");

    testing::internal::CaptureStdout(); // Capture standard output
    cmd->execute(robot);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "1,2,EAST\n");
}

TEST(CommandFactoryTest, ReportCommandUnplacedRobot) {
    ToyRobot robot;

    auto cmd = CommandFactory::createCommand("REPORT");

    testing::internal::CaptureStderr(); // Capture standard error
    cmd->execute(robot);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(output, "Robot is not placed on the grid.\n");
}

// Test Invalid Command
TEST(CommandFactoryTest, InvalidCommand) {
    std::string input = "JUMP";
    auto cmd = CommandFactory::createCommand(input);

    EXPECT_EQ(cmd, nullptr); // Invalid command should return nullptr
}


// Main function for running all tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}