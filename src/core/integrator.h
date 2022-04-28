#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "scene.h"
#include "camera.h"
#include "surfel.h"
#include "light.h"
#include <vector>

namespace rt3{

    class Camera;

    class Integrator{
        public:
        Camera * camera;
        std::vector<Light *> lights;

        Integrator(Camera * c) : camera(c) { /* Empty */ }

        virtual void render(Scene* & scene) { /* Empty */ }

    };

    class FlatIntegrator : public Integrator {
        public:

        FlatIntegrator(Camera * c) : Integrator(c) {}

        void render(Scene* & scene) override;

    };

    class BlinnPhongIntegrator : public Integrator {
        public:

        int mdepth = 0;

        BlinnPhongIntegrator(Camera * c, int d = 0) : Integrator(c), mdepth(d) {}
        ColorXYZ Li(Ray & ray, Scene * & scene, int depth, int i, int j, ColorXYZ bgc);

        void render(Scene* & scene) override;

    };
}

#endif