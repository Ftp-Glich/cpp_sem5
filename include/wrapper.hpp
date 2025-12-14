#include "command.hpp"

class Wrapper {
    std::unique_ptr<BaseCommand> cmd;

public:
    template <typename T, typename Ret, typename... Args>
    Wrapper(T* obj, Ret (T::*method)(Args...), std::map<std::string, std::any> defaultArgs) {
        std::vector<std::string> paramOrder;
        paramOrder.reserve(defaultArgs.size());
        for(const auto& [key, val]: defaultArgs) {
            paramOrder.push_back(key);
        }
        cmd = Command(obj, method, std::move(paramOrder), defaultArgs);
    }

    friend class Engine;
};