#pragma once
#include "shape.h"
#include "light.h"
#include "fcolor.h"
#include "ray.h"

struct InterSectionTestResult {
    Shape* shape;
    InterSectionPoint intersectionPoint;
};

class Scene {
public:
    std::vector<Shape*> shapes;
    std::vector<LightSource*> lightSources;
    FColor ambientIntensity;

    InterSectionTestResult testIntersectionWithAll(Ray eye) const
    {
        InterSectionTestResult result;
        InterSectionPoint point;

        // 全ての物体に対して交差判定を行う
        for (Shape* s : shapes) {
            if(s->testIntersection(eye, point)) {
                point.distance = (point.position - eye.start).magSg();
                // first shape
                if (result.intersectionPoint.distance < 0) {
                    result.shape = s;
                    result.intersectionPoint = point;
                } else {
                    if (point.distance < result.intersectionPoint.distance) {
                        result.shape = s;
                        result.intersectionPoint = point;
                    }
                }
            }
        }

        return result;
    }

    InterSectionTestResult testIntersectionWithAll(Ray ray, float maxDist) const
    {
        InterSectionTestResult result;
        InterSectionPoint point;

        // 影判定
        // 全ての物体に対して交差判定を行う
        for (Shape* s : shapes) {
            if(s->testIntersection(ray, point)) {
                point.distance = (point.position - ray.start).magSg();
                if (point.distance < maxDist) {
                    result.shape = s;
                    result.intersectionPoint = point;
                    break;
                }
            }
        }

        return result;
    }
};
