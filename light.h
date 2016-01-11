#pragma once
#include <cfloat>
#include "fcolor.h"
#include "vec.h"

struct Lighting {
    float distance;   // 光源までの距離
    FColor intensity; // 強度
    Vec direction;    // 方向
};

class LightSource {
public:
    FColor intensity;

    LightSource() {}

    LightSource(FColor i):
        intensity(i)
    {}

    virtual Lighting lightingAt(Vec pos) = 0;
};

class PointLightSource : public LightSource {
public:
    Vec position;

    PointLightSource() {}

    PointLightSource(FColor i, Vec p):
        LightSource(i), position(p)
    {}

    Lighting lightingAt(Vec pos) override {
        Lighting light;
        light.intensity = intensity;
        light.distance = (pos - position).magSg();
        light.direction = (pos - position).normalize();

        return light;
    }
};

class DirectionalLightSource : public LightSource {
public:
    Vec direction;

    DirectionalLightSource() {}

    DirectionalLightSource(FColor i, Vec d):
        LightSource(i), direction(d)
    {}

    Lighting lightingAt(Vec pos) override {
        Lighting light;
        light.intensity = intensity;
        light.distance = FLT_MAX;
        light.direction = direction.normalize();

        return light;
    }
};

