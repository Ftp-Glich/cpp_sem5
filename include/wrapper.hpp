#include "command.hpp"

class Wrapper {
    std::unique_ptr<BaseCommand> cmd;

public:
    template <typename T, typename Ret, typename... Args>
    Wrapper(T* obj, Ret (T::*method)(Args...), 
            std::map<std::string, std::any> defaultArgs);

    friend class Engine;
};