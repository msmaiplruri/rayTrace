#include <cmath>
#include <algorithm>
#include <vector>
#include <cassert>
#include <png.h>
#include "vec.h"
#include "fcolor.h"
#include "ray.h"
#include "shape.h"
#include "light.h"
#include "scene.h"
#include "image.h"
using namespace std;

const int WIDTH = 512;
const int HEIGHT = 512;
const char* PNG_FILE_NAME = "out.png";
const float C_EPSILON = 1.0f / 512;

int ray_trace(png_bytepp&);

int main()
{
   auto image = new png_bytep[HEIGHT];
    for (auto j = 0; j < HEIGHT; j++) {
        image[j] = new png_byte[WIDTH * 4];
        for (auto i = 0; i < WIDTH * 4; i++) {
            image[j][i] = 0xff;
        }
    }

    if (ray_trace(image)) goto heaven;
    if (write_png(image, PNG_FILE_NAME, WIDTH, HEIGHT)) goto heaven;

heaven:
    for (auto j = 0; j < HEIGHT; j++) delete[] image[j];
    delete[] image;

    return 0;
}

FColor shading(const Scene scene, const InterSectionTestResult result, const Ray eye)
{
    // 放射輝度
    FColor col;

    // Phongの反射モデル
    // 環境光
    col += result.shape->material.ambientFactor * scene.ambientIntensity;

    for (LightSource* light : scene.lightSources) {
        // 光源情報
        Lighting lighting = light->lightingAt(result.intersectionPoint.position);

        // 入射ベクトル
        Vec l = -lighting.direction.normalize();

        // 影判定
        // C_EPSILON だけ光源側にずらして計算する(重なり防止)
        Vec eps = -lighting.direction * C_EPSILON;
        Ray shadowRay;
        shadowRay.start = result.intersectionPoint.position + eps;
        shadowRay.direction = l;
        auto shadowRes = scene.testIntersectionWithAll(shadowRay, lighting.distance - C_EPSILON);
        if (shadowRes.intersectionPoint.exist) {
            // 物体の交点と光源の間に他の物体が在る場合
            continue;
        }

        // (直接光)拡散反射光
        float nl = max(min(result.intersectionPoint.normal.dot(l), 1.0f), 0.0f);
        if (nl > 0) {
            col += (result.shape->material.diffuseFactor * lighting.intensity).mult<float>(nl);
        }
        // (直接光)鏡面反射光
        Vec r = result.intersectionPoint.normal * 2 * nl - l;
        Vec v = -eye.direction.normalize();
        float vr = max(min(v.dot(r), 1.0f), 0.0f);
        if (nl > 0 && vr > 0) {
            col += (result.shape->material.specularFactor * lighting.intensity).mult<float>(powf(vr, result.shape->material.shininess));
        }
    }

    return col.normalize();
}

int ray_trace(png_bytepp& image)
{
    // カメラ位置
    Ray eye(Vec(0, 0, -5.0f));

    // 環境光反射係数
    FColor kAmb = FColor(0.01f, 0.01f, 0.01f);
    // 拡散光反射係数
    FColor kDif = FColor(0.69f, 0.70f, 0.00f);
    // 鏡面反射係数
    FColor kSpe = FColor(0.30f, 0.30f, 0.30f);
    float shininess = 8.0f;    // 光沢度

    Sphere s3(Vec(1, 0, 15.0f), 1.0f);
    Sphere s4(Vec(0, 0, 10.0f), 1.0f);
    Sphere s5(Vec(-1, 0, 5.0f), 1.0f);
    Plane p1(Vec(0, 1, 0), Vec(0, -1, 0));

    s3.setMaterial(kAmb, FColor(0.69f, 0, 0), kSpe, shininess);
    s4.setMaterial(kAmb, FColor(0, 0.69f, 0), kSpe, shininess);
    s5.setMaterial(kAmb, FColor(0, 0, 0.69f), kSpe, shininess);
    p1.setMaterial(kAmb, FColor(0.69f, 0.69f, 0.69f), kSpe, shininess);

    vector<Shape*> shapes;
    shapes.push_back(&s3);
    shapes.push_back(&s4);
    shapes.push_back(&s5);
    shapes.push_back(&p1);

    // 光源
    PointLightSource light1(FColor(0.5f, 0.5f, 0.5f), Vec(-5.0f, 5.0f, -5.0f));
    PointLightSource light2(FColor(0.5f, 0.5f, 0.5f), Vec(5.0f, 0.0f, -5.0f));
    PointLightSource light3(FColor(0.5f, 0.5f, 0.5f), Vec(5.0f, 20.0f, -5.0f));

    vector<LightSource*> lights;
    lights.push_back(&light1);
    lights.push_back(&light2);
    lights.push_back(&light3);

    Scene scene;
    scene.shapes = shapes;
    scene.lightSources = lights;
    // 環境光
    scene.ambientIntensity = FColor(0.1f, 0.1f, 0.1f);

    for (auto y = 0; y < HEIGHT; y++) {
        png_bytep row = image[y];
        for (auto x = 0; x < WIDTH; x++) {
            png_bytep rgba = &(row[x*4]);
            //rgba[3] = 0xff; // alpha

            eye.setDirection(x, y, WIDTH, HEIGHT);

            // 半直線が物体と交差する場合
            // 全ての物体に対して交差判定を行う
            // 視点位置から(x, y)に向かう半直線と物体との交差判定
            auto result = scene.testIntersectionWithAll(eye);

            if (result.intersectionPoint.exist) {
                // シェーディングの処理を行う
                auto col = shading(scene, result, eye);

                rgba[0] *= col.red;
                rgba[1] *= col.green;
                rgba[2] *= col.blue;
            } else {
                // 次の点を処理する(背景色として扱う)
                rgba[0] = 0x64; // red
                rgba[1] = 0x95; // green
                rgba[2] = 0xED; // blue
            }
        }
    }

    return 0;
}
