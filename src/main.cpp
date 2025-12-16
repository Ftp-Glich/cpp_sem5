#include <iostream>
#include <map>
#include "engine.hpp"

class TestClass {
public:
    int add(int a, int b) { return a + b; }
    std::string greet(const std::string& name) { return "Hello, " + name; }
};

void test_generic_container() {
    Engine engine;
    TestClass obj;
    std::vector<std::pair<std::string, std::any>> defaults = {{"a", 5}, {"b", 7}};
    Wrapper wrapper(&obj, &TestClass::add, defaults);
    engine.register_command(std::move(wrapper), "add");
    
    std::map<std::string, std::any> args = {{"b", 3}};
    auto result = engine.execute("add", args);
    if (std::any_cast<int>(result) != 8) {
        throw std::runtime_error("test_generic_container failed");
    }
}

void test_vector_container() {
    Engine engine;
    TestClass obj;
    std::vector<std::pair<std::string, std::any>> defaults = {{"name", std::string("World")}};
    Wrapper wrapper(&obj, &TestClass::greet, defaults);
    engine.register_command(std::move(wrapper), "greet");
    
    std::vector<std::pair<std::string, std::any>> args = {{"name", std::string("Alice")}};
    auto result = engine.execute("greet", args);
    if (std::any_cast<std::string>(result) != "Hello, Alice") {
        throw std::runtime_error("test_vector_container failed");
    }
}

void test_command_not_found() {
    Engine engine;
    try {
        engine.execute("missing", std::map<std::string, std::any>{});
        throw std::runtime_error("test_command_not_found failed: exception not thrown");
    } catch (const std::runtime_error& e) {
        if (std::string(e.what()) != "Command not found: missing") {
            throw;
        }
    }
}

int main() {
    try {
        test_generic_container();
        test_vector_container();
        test_command_not_found();
        std::cout << "All tests passed." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}