#pragma once
#include <cstdio>
#include <algorithm>

class FColor {
public:
    float red, green, blue;

    FColor():
        red(0), green(0), blue(0)
    {}

    FColor(float _r, float _g, float _b):
        red(_r), green(_g), blue(_b)
    {}

    FColor(const FColor& c):
        red(c.red), green(c.green), blue(c.blue)
    {}

    void print() const {
        std::printf("[%f, %f, %f]\n", red, green, blue);
    }

    FColor operator+(const FColor& _c) const {
        FColor c(
            red + _c.red,
            green + _c.green,
            blue + _c.blue
            );
        return c;
    }

    void operator+=(const FColor& _c) {
        *this = *this + _c;
    }

    FColor operator*(const FColor& _c) const {
        FColor c(
            red * _c.red,
            green * _c.green,
            blue * _c.blue
            );
        return c;
    }

    template <class T>
    FColor mult(const T n) const {
        FColor c(
            red * n,
            green * n,
            blue * n
            );
        return c;
    }

    FColor normalize() const {
        using namespace std;
        FColor c(
            min(max(red, 0.0f), 1.0f),
            min(max(green, 0.0f), 1.0f),
            min(max(blue, 0.0f), 1.0f)
            );
        return c;
    }
};

