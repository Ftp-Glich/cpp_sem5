#pragma once
#include "command.hpp"
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <any>

class Wrapper {
public:
    Wrapper& operator=(const Wrapper&) = delete;

    Wrapper(Wrapper&& other) noexcept : 
        cmd(other.cmd), deleter(other.deleter), executor(other.executor) 
    {
        other.cmd = nullptr;
        other.deleter = nullptr;
        other.executor = nullptr;
    }

    Wrapper& operator=(Wrapper&& other) noexcept {
        if (this != &other) {
            reset();
            cmd = other.cmd;
            deleter = other.deleter;
            executor = other.executor;
            other.cmd = nullptr;
            other.deleter = nullptr;
            other.executor = nullptr;
        }
        return *this;
    }

    template <typename T, typename Ret, typename... Args, ArgumentContainer Container>
    Wrapper(T* obj, Ret (T::*method)(Args...), Container&& defaultArgs) {
        auto command = new Command<T, Ret, Args...>(
            obj, method, std::forward<Container>(defaultArgs)
        );

        cmd = command;

        deleter = [](void* p) {
            delete static_cast<Command<T, Ret, Args...>*>(p);
        };

        executor = [](void* p, const std::vector<std::pair<std::string, std::any>>& args) -> std::any {
            auto* cmd = static_cast<Command<T, Ret, Args...>*>(p);
            return cmd->execute(args);
        };
    }

    template <ArgumentContainer Container>
    std::any execute(Container&& args) const {
        return execute_impl(convert_to_vector(std::forward<Container>(args)));
    }

    std::any execute_impl(const std::vector<std::pair<std::string, std::any>>& args) const {
        if (!executor) throw std::runtime_error("Wrapper is empty");
        return executor(cmd, args);
    }

    ~Wrapper() { reset(); }

private:
    void reset() {
        if (cmd && deleter) {
            deleter(cmd);
        }
        cmd = nullptr;
        deleter = nullptr;
        executor = nullptr;
    }

    template <ArgumentContainer Container>
    static std::vector<std::pair<std::string, std::any>> convert_to_vector(Container&& args) {
        std::vector<std::pair<std::string, std::any>> result;
        result.reserve(std::distance(std::begin(args), std::end(args)));
        
        for (auto&& item : args) {
            result.emplace_back(
                static_cast<std::string>(item.first),
                std::any(std::forward<decltype(item.second)>(item.second))
            );
        }
        return result;
    }
    void* cmd = nullptr;
    void (*deleter)(void*) = nullptr;
    std::any (*executor)(void*, const std::vector<std::pair<std::string, std::any>>& args) = nullptr;
};