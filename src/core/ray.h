#ifndef RAY_H
#define RAY_H

#include "Geometry.hpp"

namespace rt3{
    class Ray 
    {
        public:
            Ray (){}
            Ray (const Vector3<float>& o, const Vector3<float>& d ) : o{o}, d{d} {/*empty*/}
            Ray (const Ray& other);
            void normalize();
            void print_ray();
            void operator=(const Ray& other);
            Vector3<float> operator()(float t) const { return o + d * t; }

            Vector3<float> o; //!< origin (orginalmente era um Point3)
            Vector3<float> d; //!< direction (Originalmente era um Vector3)

            float min = 0.0;
            float limit = 100000.0;
            // ... the methods goes here
    };

    Ray operator-(const Ray& ray);
}

#endif