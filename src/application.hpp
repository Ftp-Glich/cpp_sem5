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
private:
    void tokenize(std::string& input, std::vector<std::pair<PluginInfo, tokenType>>& tokens);
    void update_pugins();
    std::unique_ptr<Calculator> calculator_;
    std::shared_ptr<PluginManager> pm_;
    std::optional<double> prev_result_;
    std::vector<PluginInfo> supported_ops_;
};