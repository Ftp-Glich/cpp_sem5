#include "wrapper.hpp"

class Engine {
    std::unordered_map<std::string, std::unique_ptr<BaseCommand>> commands;

public:
    void register_command(Wrapper* wrapper, const std::string& name);

    std::any execute(const std::string& name, const std::unordered_map<std::string, std::any>& args) const;
};