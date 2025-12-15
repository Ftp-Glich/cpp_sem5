#include "command.hpp"

class Wrapper {
    std::unique_ptr<BaseCommand> cmd;

public:
    template <typename T, typename Ret, typename... Args>
    Wrapper(T* obj, Ret (T::*method)(Args...), std::vector<std::pair<std::string, std::any>>&& defaultArgs) {
        cmd = std::make_unique<Command<T, Ret, Args...>>(
            obj, method, std::move(defaultArgs)
        );
    }

    friend class Engine;
};