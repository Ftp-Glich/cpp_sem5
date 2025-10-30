#pragma once

#ifdef _WIN32
  #ifdef PLUGIN_EXPORTS
    #define PLUGIN_API __declspec(dllexport)
  #else
    #define PLUGIN_API __declspec(dllimport)
  #endif
#else
  #define PLUGIN_API
#endif

#include <string>

class PLUGIN_API IOperation {
public:
    virtual ~IOperation() = default;
    virtual std::string name() const = 0;
    virtual double operate(double a, double b = 0) const = 0;
    unsigned int arity;
    bool right_associative;
    bool is_operator;
    int precedence = 0; 
};

extern "C" PLUGIN_API IOperation* create_plugin();

extern "C" PLUGIN_API void destroy_plugin(IOperation* plugin);
