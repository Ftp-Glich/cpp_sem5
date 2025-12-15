#include "wrapper.hpp"

template <ArgumentContainer Container>
class Engine {
public:
    void register_command(Wrapper* wrapper, const std::string& name) {
        commands.insert_or_assign(name, std::move(wrapper->cmd));
    }

    std::any execute(const std::string& name, Container&& args)  {
        auto command = commands.find(name);
        if (command == commands.end()) {
            throw std::runtime_error("Unknown function" + name);
        }
        return command->second->execute(std::forward<decltype(args)>(args));
    }
private: 
    std::unordered_map<std::string, std::unique_ptr<BaseCommand>> commands;

};

