#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>

// Global variable for grid size
const int gridSize = 5;

class ToyRobot {
public:
    int x, y;
    std::string facing;

    ToyRobot() : x(-1), y(-1), facing("") {}

    // Check if the robot is placed on the grid
    bool isPlaced() const {
        return x != -1 && y != -1 && !facing.empty();
    }
};

// Base Command Interface
class Command {
public:
    virtual bool execute(ToyRobot& robot) = 0;
    virtual ~Command() = default;
};

// places the ToyRobot based on the x,y coordinates and direction facing
class PlaceCommand : public Command {
private:
    int x, y;
    std::string facing;
    // using vector of directions, order of entries is not important
    const std::vector<std::string> directions = {"NORTH", "EAST", "SOUTH", "WEST"};

    // Validate if the position and direction are valid
    bool isValid(int x, int y, const std::string& facing) const {
        return x >= 0 && x < gridSize && y >= 0 && y < gridSize &&
               std::find(directions.begin(), directions.end(), facing) != directions.end();
    }

public:
    PlaceCommand(int x, int y, const std::string& facing) : x(x), y(y), facing(facing) {}

    bool execute(ToyRobot& robot) override {
        if (isValid(x, y, facing)) {
            robot.x = x;
            robot.y = y;
            robot.facing = facing;
            return true;
        } else {
            std::cerr << "Invalid PLACE command." << std::endl;
            return false;
        }
    }

    // parse a PLACE command and create a PlaceCommand object
    static std::unique_ptr<PlaceCommand> parse(const std::string& command) {
        if (command.find("PLACE") != 0) return nullptr; // Ensure command starts with "PLACE"

        std::istringstream ss(command.substr(6)); // Skip "PLACE "
        int x, y;
        std::string facing;
        std::string token;

        if (std::getline(ss, token, ',')) x = std::stoi(token);
        if (std::getline(ss, token, ',')) y = std::stoi(token);
        if (std::getline(ss, facing)) {
            return std::make_unique<PlaceCommand>(x, y, facing);
        }

        return nullptr;
    }
};

// moves the ToyRobot one step in the direction of facing
class MoveCommand : public Command {
public:
    bool execute(ToyRobot& robot) override {
        if (!robot.isPlaced()) {
            std::cerr << "Robot is not placed on the grid." << std::endl;
            return false;
        }

        int newX = robot.x, newY = robot.y;
        if (robot.facing == "NORTH") newY++;
        else if (robot.facing == "EAST") newX++;
        else if (robot.facing == "SOUTH") newY--;
        else if (robot.facing == "WEST") newX--;

        if (newX >= 0 && newX < gridSize && newY >= 0 && newY < gridSize) {
            robot.x = newX;
            robot.y = newY;
            return true;
        }
        return false;
    }
};

// turns the ToyRobot by 90% degrees left
class LeftCommand : public Command {
private:
    // using vector of directions in order of 90% movement, order of entries is important 
    // i.e. index 1 ("EAST") -> moved by 90 degree left -> index 0 ("NORTH") and so on
    const std::vector<std::string> directions = {"NORTH", "EAST", "SOUTH", "WEST"};
    const int leftTurnDirectionIndex = 3;
    const int totalDirectionsCount = 4;

public:
    bool execute(ToyRobot& robot) override {
        if (!robot.isPlaced()) {
            std::cerr << "Robot is not placed on the grid." << std::endl;
            return false;
        }
        int currentIndex = std::find(directions.begin(), directions.end(), robot.facing) - directions.begin();
        robot.facing = directions[(currentIndex + leftTurnDirectionIndex) % totalDirectionsCount];
        return true;
    }
};

// turns the ToyRobot by 90% degrees right
class RightCommand : public Command {
private:
    // using vector of directions in order of 90% movement, order of entries is important 
    // i.e. index 0 ("NORTH") -> moved by 90 degree right -> index 1 ("EAST") and so on
    const std::vector<std::string> directions = {"NORTH", "EAST", "SOUTH", "WEST"};
    const int rightTurnDirectionIndex = 1;
    const int totalDirectionsCount = 4;

public:
    bool execute(ToyRobot& robot) override {
        if (!robot.isPlaced()) {
            std::cerr << "Robot is not placed on the grid." << std::endl;
            return false;
        }
        int currentIndex = std::find(directions.begin(), directions.end(), robot.facing) - directions.begin();
        robot.facing = directions[(currentIndex + rightTurnDirectionIndex) % totalDirectionsCount];
        return true;
    }
};

// reports the output to console
class ReportCommand : public Command {
public:
    bool execute(ToyRobot& robot) override {
        if (robot.isPlaced()) {
            std::cout << robot.x << "," << robot.y << "," << robot.facing << std::endl;
            return true;
        } else {
            std::cerr << "Robot is not placed on the grid." << std::endl;
            return false;
        }
    }
};


// creates command based on the command string
class CommandFactory {
public:
    static std::unique_ptr<Command> createCommand(const std::string& input) {
        if (input.find("PLACE") == 0) {
            return PlaceCommand::parse(input);
        } else if (input == "MOVE") {
            return std::make_unique<MoveCommand>();
        } else if (input == "LEFT") {
            return std::make_unique<LeftCommand>();
        } else if (input == "RIGHT") {
            return std::make_unique<RightCommand>();
        } else if (input == "REPORT") {
            return std::make_unique<ReportCommand>();
        } else {
            std::cerr << "Invalid command: " << input << std::endl;
            return nullptr;
        }
    }
};