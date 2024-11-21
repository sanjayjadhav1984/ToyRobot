#include "ToyRobot.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <commands_file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    ToyRobot robot;
    std::string command;

    while (std::getline(file, command)) {
        auto cmd = CommandFactory::createCommand(command);
        if (cmd) {
            cmd->execute(robot);
        }
    }

    return 0;
}
