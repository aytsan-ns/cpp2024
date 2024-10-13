#pragma once

double applyOperator(char operation, double a, double b);
int precedence(char operation);
double evaluateExpression(const std::string& expression, PluginLoader& loader);