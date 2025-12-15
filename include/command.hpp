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
    virtual std::any execute(const std::vector<std::pair<std::string, std::any>>& args) = 0;
};

template <typename T, typename Ret, typename... Args>
class Command: public BaseCommand {
public:
    Command(T* obj, Ret (T::*method)(Args...), const std::vector<std::pair<std::string, std::any>>&& default_args)
     : obj(obj), method(method) {
        paramOrder.reserve(default_args.size());
        for (auto& [name, value] : default_args) {
            paramOrder.push_back(name);
            defaultArgs[name] = std::move(value);
        }

        if (paramOrder.size() != sizeof...(Args)) {
            throw std::invalid_argument(
                "Parameter count mismatch: " + 
                std::to_string(paramOrder.size()) + " vs " + 
                std::to_string(sizeof...(Args))
            );
        }
    }

    std::any execute(const std::vector<std::pair<std::string, std::any>>& args) {
        auto mergedArgs = defaultArgs;
        for (const auto& [key, value] : args) {
            mergedArgs[key] = value;
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
    Ret (T::*method)(Args...);
    std::vector<std::string> paramOrder; 
    std::unordered_map<std::string, std::any> defaultArgs; 
};