#pragma once
#include <algorithm>
#include "ray.h"
#include "vec.h"
#include "fcolor.h"

struct InterSectionPoint {
public:
    Vec position;
    Vec normal;
    float distance;
    bool exist;

    InterSectionPoint():
        distance(-1), exist(false)
    {}
};

struct Material {
    FColor ambientFactor;
    FColor diffuseFactor;
    FColor specularFactor;
    float shininess;
};

class Shape {
public:
    Material material;
    void setMaterial(FColor amb, FColor dif, FColor spe, float s)
    {
        material.ambientFactor = amb;
        material.diffuseFactor = dif;
        material.specularFactor = spe;
        material.shininess = s;
    }

    virtual bool testIntersection(const Ray ray, InterSectionPoint& point) = 0;
};

class Sphere : public Shape {
public:
    Vec center;
    float radius;

    Sphere() {}

    Sphere(Vec c, float r):
        center(c), radius(r)
    {}

    bool testIntersection(const Ray ray, InterSectionPoint& point) override {
        Vec s = ray.start - center;
        float t;

        float A = fabs(ray.direction.magSg());
        float B = 2 * ray.direction.dot(s);
        float C = fabs(s.magSg()) - radius*radius;
        float D = B*B - 4*A*C;
        if (D > 0) {
            auto t1 = (- B + sqrtf(D)) / (2 * A);
            auto t2 = (- B - sqrtf(D)) / (2 * A);
            t = std::min(t1, t2);
        } else if (D == 0) {
            t = - B / (2 * A);
        } else {
            t = -1;
        }

        if (t >= 0) {
            point.position = ray.getPoint(t);
            point.normal = (point.position - center).normalize();
            point.exist = true;
        } else {
            point.exist = false;
        }

        return point.exist;
    }
};

class Plane : public Shape {
public:
    Vec normal;
    Vec position;

    Plane() {}

    Plane(Vec n, Vec p):
        normal(n), position(p)
    {}

    bool testIntersection(const Ray ray, InterSectionPoint& point) override {
        auto t = (normal.dot(position - ray.start)) / (normal.dot(ray.direction));
        if (t >= 0) {
            point.position = ray.getPoint(t);
            point.normal = normal.normalize();
            point.exist = true;
        } else {
            point.exist = false;
        }
        return point.exist;
    }
};

