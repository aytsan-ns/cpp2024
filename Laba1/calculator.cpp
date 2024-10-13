#include <iostream>
#include <locale.h>
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

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "Russian");

    try {
        PluginLoader loader("C:\\Users\\nasty\\3D Objects\\ÍÏ C++\\ÄÇ\\Laba1\\x64\\Debug\\plugins");
        loader.loadPlugins();

        std::string expression;
        std::cout << "Enter an expression (or 'exit' to complete): " << std::endl;

        while (true) {
            std::cout << "> ";
            std::getline(std::cin, expression);

            if (expression == "exit") break;

            try {
                double result = evaluateExpression(expression, loader);
                std::cout << "Result: " << result << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error initializing plugins: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}