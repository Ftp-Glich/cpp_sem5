#include <any>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>

class BaseCommand {
public:
    virtual ~BaseCommand() = default;
    virtual std::any execute(const std::map<std::string, std::any>& args) = 0;
};

template <typename T, typename Ret, typename... Args>
class Command: BaseCommand {
public:
    Command(T* obj, Ret (T::*method)(Args...), std::vector<std::string> param_order, std::unordered_map<std::string, std::any> default_args)
     : obj(obj), method(method), paramOrder(param_order), defaultArgs(default_args) {}

    std::any execute(const std::unordered_map<std::string, std::any>& args) {
        std::unordered_map<std::string, std::any> mergedArgs = defaultArgs;
        for(const auto& [key, value]: args) {
            auto it = mergedArgs.find(key);
            if (it == mergedArgs.end()) {
                throw std::runtime_error("Unknown arg: " + key);
            } 
            mergedArgs[key] = value;
        }
        for (const auto& paramName : paramOrder) {
            if (mergedArgs.find(paramName) == mergedArgs.end()) {
                throw std::runtime_error("Missing required parameter: " + paramName);
            }
        }
        return callMethod(mergedArgs, std::index_sequence_for<Args...>{});
    }

private:
    template <size_t... I>
    std::any callMethod(const std::unordered_map<std::string, std::any>& mergedArgs, 
                        std::index_sequence<I...>) 
    {
        return std::any(
            (obj->*method)(
                std::any_cast<Args>(mergedArgs.at(paramOrder[I]))...
            )
        );
    }

    T* obj;
    Ret (T::*method)(Ards...) method;
    std::unordered_map<std::string, std::any> defaultArgs;
    std::vector<std::stirng> paramOrder;
};