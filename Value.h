//
// Created by Admin on 22.07.2026.
//

#ifndef INTERPRETER_ANNA_MOHIL_VALUE_H
#define INTERPRETER_ANNA_MOHIL_VALUE_H
#include <string>

class Value {
public:
    enum class Type{Number, Function};

    Type type;
    double number = 0.0;
    std::string functionName;

    static Value num(double n) {
        Value v;
        v.type = Type::Number;
        v.number = n;
        return v;
    }

    static Value func(const std::string& name) {
        Value v;
        v.type = Type::Function;
        v.functionName = name;
        return v;
    }

    double asNumber() const {
        if (type != Type::Number) {
            throw std::runtime_error("Expected a number, but: " + functionName);
        }
        return number;
    }
    bool isFunction() const {
        return type == Type::Function;
    }

};
#endif //INTERPRETER_ANNA_MOHIL_VALUE_H
