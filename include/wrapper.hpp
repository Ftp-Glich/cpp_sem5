#include "command.hpp"

template <typename T, typename Ret, typename... Args>
class Wrapper {
public:
    template< ArgumentContainer Container>
    Wrapper(T* obj, Ret (T::*method)(Args...), Container&& defaultArgs) {
        cmd = std::make_unique<Command<T, Ret, Args...>>(
            obj, method, std::froward<decltype(Container)>(defaultArgs)
        );
    }

    friend class Engine;
private:
    std::unique_ptr<BaseCommand<Command<T, Ret, Args...>>> cmd;
};