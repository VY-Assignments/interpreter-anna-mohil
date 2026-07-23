//
// Created by Admin on 23.07.2026.
//

#ifndef INTERPRETER_ANNA_MOHIL_AST_H
#define INTERPRETER_ANNA_MOHIL_AST_H
#include "Environment.h"
#include "Value.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class Expr {
public:
    virtual Value eval(Environment& env) = 0;
    virtual ~Expr() = default;
};

using ExprPtr = std::shared_ptr<Expr>;

class NumExpr: public Expr {
public:
    explicit NumExpr(double v): value(v) {}
    Value eval(Environment&) override {
        return Value::num(value);
    }
private:
    double value;
};

class IdentExpr: public Expr {
public:
    explicit IdentExpr(std::string n) : name(std::move(n)) {}
    Value eval(Environment& env) override {
        if (env.hasVar(name)) {
            return env.getVar(name);
        }

        if (env.isKnownFunc(name)) {
            return Value::func(name);
        }
        throw std::runtime_error("Undefinied identifier: " + name);
    }

private:
    std::string name;
};

class BinaryExpr: public Expr {
public:
    BinaryExpr(char op, ExprPtr l, ExprPtr r): op(op), left(std::move(l)), right(std::move(r)) {}
    Value eval(Environment& env) override {
        double a = left->eval(env).asNumber();
        double b = right->eval(env).asNumber();
        switch (op) {
            case '+': return Value::num(a + b);
            case '-': return Value::num(a - b);
            case '*': return Value::num(a * b);
            case '/':
                if (b == 0.0) throw std::runtime_error("Division by zero");
                return Value::num(a / b);
        }
        throw std::runtime_error("Unknown operator");
    }
private:
    char op;
    ExprPtr left, right;
};

class UnaryMinusExpr: public Expr {
public:
    explicit UnaryMinusExpr(ExprPtr e): inner(std::move(e)) {}
    Value eval(Environment &env) override {
        return Value::num(-inner->eval(env).asNumber());
    }
private:
    ExprPtr inner;
};

class CallExpr:public Expr {
public:
    CallExpr(std::string name, std::vector<ExprPtr> args): name(std::move(name)), args(std::move(args)) {}
    Value eval(Environment &env) override {
        std::string actualFunctionName = name;

        if (env.hasVar(name)) {
            Value v = env.getVar(name);
            if (!v.isFunction()) {
                throw std::runtime_error(name + "is not a function");
            }
            actualFunctionName = v.functionName;
        }
        std::vector<Value> argValues;
        argValues.reserve(args.size());
        for (auto& a: args) {
            argValues.push_back(a->eval(env));
        }
        return env.callFunc(actualFunctionName,argValues);
    }
private:
    std::string name;
    std::vector<ExprPtr> args;
};

class Statement {
public:
    virtual void execute(Environment& env) = 0;
    virtual ~Statement() = default;
};
using StatementPtr = std::shared_ptr<Statement>;

class VarDeclStatement: public Statement {
public:
    VarDeclStatement(std::string name, ExprPtr value): name(std::move(name)), value(std::move(value)) {}
    void execute(Environment &env) override {
        Value v = value->eval(env);
        env.defineVar(name, v);
    }
private:
    std::string name;
    ExprPtr value;
};

class FuncDefStatement: public Statement {
public:
    FuncDefStatement(std::string name, std::vector<std::string> parameters, ExprPtr body): name(std::move(name)), parameters(std::move(parameters)), body(std::move(body)) {}
    void execute(Environment &env) override {
        FunctionDef def;
        def.parameters = parameters;
        def.body = body;
        env.defineFunc(name, def);
    }
private:
    std::string name;
    std::vector<std::string> parameters;
    ExprPtr body;
};

class ExprStatement: public Statement {
public:
    explicit ExprStatement(ExprPtr expr): expr(std::move(expr)) {}
    void execute(Environment &env) override {
        Value v = expr->eval(env);
        if (v.type == Value::Type::Number) {
            double num = v.number;
            if (num == static_cast<long long>(num)) {
                std::cout << static_cast<long long>(num) << std::endl;
            }else {
                std::cout << num << std::endl;
            }
        }else {
            std::cout << "function"<< v.functionName << std::endl;
        }
    }
private:
    ExprPtr expr;
};
#endif //INTERPRETER_ANNA_MOHIL_AST_H
