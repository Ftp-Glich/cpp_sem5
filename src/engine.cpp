#include "../include/engine.h"

void Engine::register_command(Wrapper* wrapper, const std::string& name) {
    commands.insert_or_assign(name, wrapper->cmd);
}

std::any Engine::execute(const std::string& name, const std::unordered_map<std::string, std::any>& args) const {
    auto command = commands.find(name);
    if (command == commands.end()) {
        throw std::runtime_error("Unknown function" + name);
    }
    return command->second->execute(args);
}