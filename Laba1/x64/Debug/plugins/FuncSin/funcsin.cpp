#include "pch.h"
#include <cmath>

#define PI 3.141529653589793238462643383

extern "C" {
    __declspec(dllexport) const char* getFunctionName() {
        return "sin";
    }

    __declspec(dllexport) double calculate(double x) {
        return std::sin(x * PI / 180);
    }
}
