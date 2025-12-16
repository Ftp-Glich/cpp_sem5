#pragma once
#include "wrapper.hpp"
#include <unordered_map>
#include <stdexcept>

class Engine {
    std::unordered_map<std::string, Wrapper> commands;

public:
    void register_command(Wrapper&& wrapper, std::string name) {
        if (commands.contains(name)) {
            throw std::runtime_error("Command already registered: " + name);
        }
        commands.emplace(std::move(name), std::move(wrapper));
    }

    template <ArgumentContainer Container>
    std::any execute(const std::string& name, Container&& args) {
        auto it = commands.find(name);
        if (it == commands.end()) {
            throw std::runtime_error("Command not found: " + name);
        }
        return it->second.execute(std::forward<Container>(args));
    }

    std::any execute(
        const std::string& name, 
        const std::vector<std::pair<std::string, std::any>>& args
    ) {
        auto it = commands.find(name);
        if (it == commands.end()) {
            throw std::runtime_error("Command not found: " + name);
        }
        return it->second.execute_impl(args);
    }
};