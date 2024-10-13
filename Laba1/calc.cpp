#include <iostream>
#include <sstream>
#include <windows.h>
#include <map>
#include <string>
#include <stack>
#include <functional>
#include <stdexcept>
#include <filesystem>
#include "plugin.h"
#include "calc.h"

double applyOperator(char operation, double a, double b) {
    switch (operation) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': if (b == 0) throw std::runtime_error("Division by zero"); return a / b;
    default: throw std::runtime_error("Unknown operator");
    }
}

int precedence(char operation) {
    if (operation == '+' || operation == '-') return 1;
    if (operation == '*' || operation == '/') return 2;
    return 0;
}

double evaluateExpression(const std::string& expression, PluginLoader& loader) {
    std::stack<double> values;
    std::stack<char> operators;

    std::istringstream tokens(expression);
    std::string token;

    while (tokens >> token) {

        if (isdigit(token[0])) values.push(std::stod(token));

        else if (token == "+" || token == "-" || token == "*" || token == "/") {
            char op = token[0];
            while (!operators.empty() && precedence(operators.top()) >= precedence(op)) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char prevOp = operators.top(); operators.pop();
                values.push(applyOperator(prevOp, a, b));
            }
            operators.push(op);
        }

        else if (token == "(") {
            operators.push('(');
        }

        else if (token == ")") {
            while (operators.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOperator(op, a, b));
            }
            operators.pop();
        }

        else if (loader.getFunction(token) != nullptr) {
            std::string funcArg;
            std::function<double(double)> funcName = loader.getFunction(token);
            tokens >> token;
            if (token != "(") throw std::runtime_error("Expected '(' after function name");
            tokens >> funcArg;
            tokens >> token;
            if (token != ")") throw std::runtime_error("Expected ')' after function argument");
            double argValue = std::stod(funcArg);
            try {
                double result = funcName(argValue);
                values.push(result);
            }
            catch (const std::exception& e) {
                std::cerr << "Error in the plugin function: " << e.what() << std::endl;
                return 1;
            }
        }
    }

    while (!operators.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = operators.top(); operators.pop();
        values.push(applyOperator(op, a, b));
    }

    return values.top();
}