#pragma once
#include "vec.h"

class Ray {
public:
    Vec start;
    Vec direction;

    Ray() {}

    Ray(Vec s):
        start(s)
    {}

    /*
     * (x, y) at
     *
     * (-1, 1, 0)  - (1, 1, 0)
     * (-1, -1, 0) - (1, -1, 0)
     */
    void setDirection(const int x, const int y, const int WIDTH, const int HEIGHT)
    {
        Vec transform(
            2.0f * x / (WIDTH - 1) - 1.0f,
            - 2.0f * y / (HEIGHT- 1) + 1.0f,
            0);

        direction = transform - start;
    }

    Vec getPoint(float t) const
    {
        return start + direction * t;
    }
};

