#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cctype>
#include <iostream>
#include <optional>
#include <algorithm>
#include "calculator.hpp"
#include "plugin_manager.hpp"
#include "plugin_interface.hpp"


class Application {
public:
    Application() = delete;
    Application(const Application& other) = delete;
    Application(Application&& other) = delete;
    Application operator=(const Application& other) = delete;
    Application operator=(Application&& other) = delete;
    ~Application() = default;
    explicit Application(std::string&& plugin_dir);
    void run(std::istream& istream);
    double test(std::string&& input);
private:
    void tokenize(std::string& input, std::vector<Token>& tokens);
    void update_pugins();
    std::unique_ptr<Calculator> calculator_;
    sptr<PluginManager> pm_;
    std::optional<double> prev_result_;
    std::vector<sptr<IOperation>> supported_ops_;
    std::unordered_map<std::string, double> constants_;
};