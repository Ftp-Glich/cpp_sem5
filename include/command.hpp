#include <any>
#include <map>
#include <memory>
#include <string>

class BaseCommand {
public:
    virtual ~BaseCommand() = default;
    virtual std::any execute(const std::map<std::string, std::any>& args) = 0;
};

class Command: BaseCommand {

};