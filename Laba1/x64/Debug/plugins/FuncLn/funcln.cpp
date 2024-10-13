#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

extern "C" __declspec(dllexport) const char* getFunctionName() {
    return "ln";
}

extern "C" __declspec(dllexport) double calculate(double x) {
    if (x <= 0) {
        throw std::invalid_argument("������: �������� ��� ln ������ ���� �������������.");
    }
    return std::log(x);
}