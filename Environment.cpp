//
// Created by Admin on 22.07.2026.
//

#include "Environment.h"
#include "AST.h"
#include <stdexcept>

Environment::Environment(Environment *parent): parent(parent) {}

Environment *Environment::root() {
    Environment* e = this;
    while (e->parent) {
        e = e->parent;
    }
    return e;
}

const Environment *Environment::root() const {
    const Environment* e = this;
    while (e->parent) {
        e = e->parent;
    }
    return e;
}

bool Environment::hasVar(const std::string& name) const {
    if (variables.count(name)) return true;
    if (parent) return parent->hasVar(name);
    return false;
}

Value Environment::getVar(const std::string& name) const {
    auto it = variables.find(name);
    if (it != variables.end()) return it->second;
    if (parent) return parent->getVar(name);
    throw std::runtime_error("Undefined variable: " + name);
}

void Environment::defineVar(const std::string& name, const Value& value) {
    if (variables.count(name)) {
        throw std::runtime_error(name + "is already defined");
    }
    variables[name] = value;
}

bool Environment::isBuiltIn(const std::string& name) {
    static const std::vector<std::string> builtins = {"pow", "abs", "max", "min", "sum", "integral"};
    return std::find(builtins.begin(), builtins.end(), name) != builtins.end();
}

bool Environment::isKnownFunc(const std::string& name) const {
    if (isBuiltIn(name)) return true;
    return root()->functions.count(name) > 0;
}

void Environment::defineFunc(const std::string& name, const FunctionDef& def) {
    Environment* r = root();
    if (isBuiltIn(name)) {
        throw std::runtime_error("Cannot redefine function:" + name);
    }
    if (r->functions.count(name)) {
        throw std::runtime_error(name + "is already defined");
    }
    r->functions[name] = def;
}

Value Environment::callFunc(const std::string& name, std::vector<Value>& args) {
    if (isBuiltIn(name)) {
        return callBuiltIn(name, args);
    }
    return callUserDefined(name, args);
}

Value Environment::callBuiltIn(const std::string& name, std::vector<Value>& args) {
    if (name == "pow") {
        if (args.size() != 2) throw std::runtime_error("expects 2 arguments");
        return Value::num(std::pow(args[0].asNumber(), args[1].asNumber()));
    }
    if (name == "abs") {
        if (args.size() != 1) throw std::runtime_error("expects 1 argument");
        return Value::num(std::fabs(args[0].asNumber()));
    }
    if (name == "max") {
        if (args.size() != 2) throw std::runtime_error("expects 2 arguments");
        return Value::num(std::max(args[0].asNumber(), args[1].asNumber()));
    }
    if (name == "min") {
        if (args.size() != 2) throw std::runtime_error("expects 2 arguments");
        return Value::num(std::min(args[0].asNumber(), args[1].asNumber()));
    }
    if (name == "sum") {
        if (args.size() != 3) throw std::runtime_error("expects 3 arguments");
        if (!args[0].isFunction()) throw std::runtime_error("first argument must be a function");
        std::string f = args[0].functionName;
        int a = static_cast<int>(std::llround(args[1].asNumber()));
        int b = static_cast<int>(std::llround(args[2].asNumber()));
        double total = 0.0;
        for (int x = a; x <= b; ++x) {
            std::vector<Value> callArgs{Value::num(static_cast<double>(x))};
            total += callFunc(f, callArgs).asNumber();
        }
        return Value::num(total);
    }
    if (name == "integral") {
        if (args.size() != 3) throw std::runtime_error("expects 3 arguments");
        if (!args[0].isFunction()) throw std::runtime_error("first arfument must be a function");
        std::string f = args[0].functionName;
        double a = args[1].asNumber();
        double b = args[2].asNumber();
        const int n = 1000;
        double h = (b - a) / n;

        auto callF = [&](double x) {
            std::vector<Value> callArgs{Value::num(x)};
            return callFunc(f, callArgs).asNumber();
        };

        double total = (callF(a) + callF(b)) / 2.0;
        for (int i = 1; i < n; ++i) {
            total += callF(a + i * h);
        }
        return Value::num(total * h);
    }
    throw std::runtime_error("Unknown function: " + name);
}

Value Environment::callUserDefined(const std::string& name, std::vector<Value>& args) {
    Environment* r = root();
    auto it = r->functions.find(name);
    if (it == r->functions.end()) {
        throw std::runtime_error("Undefined function: " + name);
    }
    const FunctionDef& def = it->second;

    if (args.size() != def.parameters.size()) {
        throw std::runtime_error(name + "expects" + std::to_string(def.parameters.size()) + "arguments");
    }

    Environment local(r);
    for (size_t i = 0; i < def.parameters.size(); ++i) {
        local.defineVar(def.parameters[i], args[i]);
    }
    return def.body->eval(local);
}

