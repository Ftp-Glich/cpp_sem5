#include <any>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <utility>
#include <type_traits>
#include <iterator>

template <typename Container>
concept ArgumentContainer = requires(Container c) {
    std::begin(c);
    typename std::iterator_traits<decltype(std::begin(c))>::value_type::first_type;
    typename std::iterator_traits<decltype(std::begin(c))>::value_type::second_type;
};

template <typename Derived>
class BaseCommand {
public:
    template <ArgumentContainer Container>
    std::any execute(Container&& args) {
        return static_cast<Derived*>(this)->execute_impl(
            std::forward<Container>(args)
        );
    }
};

template <typename T, typename Ret, typename... Args>
class Command: public BaseCommand<Command<T, Ret, Args...>> {
public:
    template <ArgumentContainer Container>
    Command(
        T* obj, 
        Ret (T::*method)(Args...), 
        Container&& default_args
    ) : obj(obj), method(method) 
    {
        paramOrder.reserve(std::distance(std::begin(default_args), std::end(default_args)));
        
        for (auto&& item : default_args) {
            const std::string& name = item.first;
            auto&& value = item.second;
            
            paramOrder.push_back(name);
            defaultArgs[name] = std::any(std::forward<decltype(value)>(value));
        }

        if (paramOrder.size() != sizeof...(Args)) {
            throw std::invalid_argument(
                "Parameter count mismatch: " + 
                std::to_string(paramOrder.size()) + " vs " + 
                std::to_string(sizeof...(Args))
            );
        }
    }

private:
    template <size_t... I, ArgumentContainer Container>
    std::any callMethod(Container&& mergedArgs, 
                        std::index_sequence<I...>) 
    {
        return std::any(
            (obj->*method)(
                std::any_cast<Args>(mergedArgs.at(paramOrder[I]))...
            )
        );
    }
    
    template <ArgumentContainer Container>
    std::any execute_impl(Container&& args) {
        auto mergedArgs = defaultArgs;
        for (const auto& [key, value] : args) {
            mergedArgs[key] = value;
        }
        return callMethod(std::move(mergedArgs), std::index_sequence_for<Args...>{});
    }

    T* obj;
    Ret (T::*method)(Args...);
    std::vector<std::string> paramOrder; 
    std::unordered_map<std::string, std::any> defaultArgs; 
};