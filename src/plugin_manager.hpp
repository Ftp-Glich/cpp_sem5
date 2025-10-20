#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <unordered_map>

struct Plugin {
    HMODULE handle;
    std::string name;
    double (*operate)(double, double);
    bool is_func;
};

class PluginManager {
public:
    explicit PluginManager(std::string&& dir);
    void load_plugins();
    double execute(const std::string& op, double a, double b = 0);
    bool is_func(const std::string& name);
    std::vector<std::string> get_op_list();
private:
    std::unordered_map<std::string, Plugin> plugins_;
    std::string directory;
};
