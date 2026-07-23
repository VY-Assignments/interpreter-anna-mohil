//
// Created by Admin on 22.07.2026.
//

#ifndef INTERPRETER_ANNA_MOHIL_ENVIRONMENT_H
#define INTERPRETER_ANNA_MOHIL_ENVIRONMENT_H
#include "Value.h"
#include <vector>
#include <string>
#include <unordered_map>

class Expr;

struct FunctionDef {
    std::vector<std::string> parameters;
    std::shared_ptr<Expr> body;
};

class Environment {
public:
    explicit Environment(Environment* parent = nullptr);

    bool hasVar(const std::string& name) const;
    Value getVar(const std::string& name) const;
    void defineVar(const std::string& name, const Value& value);

    bool isKnownFunc(const std::string& name) const;
    void defineFunc(const std::string& name, const FunctionDef& def);
    Value callFunc(const std::string& name, std::vector<Value>& args);

private:
    Environment* parent;
    std::unordered_map<std::string, Value> variables;
    std::unordered_map<std::string, FunctionDef> functions;

    Environment* root();
    const Environment* root() const;

    static bool isBuiltIn(const std::string& name);
    Value callBuiltIn(const std::string& name, std::vector<Value>& args);
    Value callUserDefined(const std::string& name, std::vector<Value>& args);


};


#endif //INTERPRETER_ANNA_MOHIL_ENVIRONMENT_H
