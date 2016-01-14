#pragma once
#include <cstdio>
#include <cmath>

class Vec {
private:
    float x;
    float y;
    float z;

public:
    Vec():
        x(0.0f), y(0.0f), z(0.0f) 
    {}

    Vec(const float _x, const float _y, const float _z):
        x(_x), y(_y), z(_z) 
    {}

    Vec(const Vec& v):
        x(v.x), y(v.y), z(v.z)
    {}

    void print() const
    {
        std::printf("[%f, %f, %f]\n", x, y, z);
    }

    void set(const float _x, const float _y, const float _z)
    {
        this->x = _x;
        this->y = _y;
        this->z = _z;
    }

    void set(const Vec& vec)
    {
        this->x = vec.x;
        this->y = vec.y;
        this->z = vec.z;
    }

    Vec operator+(const Vec& vec) const
    {
        Vec v(
            this->x + vec.x,
            this->y + vec.y,
            this->z + vec.z);
        return v;
    }

    Vec operator-(const Vec& vec) const
    {
        Vec v(
            this->x - vec.x,
            this->y - vec.y,
            this->z - vec.z);
        return v;
    }

    Vec operator-() const
    {
        Vec v(
            - this->x,
            - this->y,
            - this->z);
        return v;
    }

    template<class T>
    Vec operator*(const T n) const
    {
        Vec v(
            this->x * n,
            this->y * n,
            this->z * n);
        return v;
    }

    template<class T>
    Vec operator/(const T n) const
    {
        Vec v(
            this->x / n,
            this->y / n,
            this->z / n);
        return v;
    }

    float dot(const Vec& vec) const
    {
        float ret =
            this->x * vec.x +
            this->y * vec.y +
            this->z * vec.z;
        return ret;
    }

    float mag() const
    {
        return std::sqrtf(this->magSg());
    }

    float magSg() const
    {
        float ret =
            this->x * this->x +
            this->y * this->y +
            this->z * this->z;
        return ret;
    }

    Vec normalize() const
    {
        return (*this) * (1 / this->mag());
    }
};

