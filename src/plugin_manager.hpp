#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <windows.h>
#include <unordered_map>

struct Plugin {
    HMODULE handle;
    std::string name;
    double (*operate)(double, double);
    bool is_single;
    bool right_associative;
};

class PluginManager {
public:
    explicit PluginManager(std::string&& dir);
    void load_plugins();
    double execute(const std::string& op, double a, double b = 0) const;
    bool exists(const std::string& op) const;
    std::vector<std::string> get_op_list() const;
private:
    std::unordered_map<std::string, Plugin> plugins_;
    std::string directory;
};
