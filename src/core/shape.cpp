#include "shape.h"

namespace rt3
{
    Sphere::Sphere(Point3f center, float radius)
    {
        this->center = center;
        this->radius = radius;
        this->flip_normals = false;
    }

    // A = d * d
    // B = 2(o - c) * d
    // C = (o - c) * (o - c) - r^2
    // Deltha = B^2 - 4AC
    // d = ray direction vector
    // o = ray origin
    // c = sphere center
    // r = sphere radius

    bool Sphere::intersect( const Ray& r, Surfel*& sf ) const
    {
        float result;

        float A, B, C;

        A = dot(r.d, r.d);
        B = 2.0 * dot((r.o - this->center), r.d);
        C = dot((r.o - this->center), (r.o - this->center)) - pow(this->radius,2);

        result = pow(B,2) - 4.0*A*C;

        // t1 = (-(o - c) * d + sqrt(result)) / d * d
        // t2 = (-(o - c) * d - sqrt(result)) / d * d

        if(result >= 0)
        {
            float t1, t2;

            t1 = (-B - std::sqrt(result)) / (2*A);
            t2 = (-B + std::sqrt(result)) / (2*A);
            
            if(t1 < t2)
            {
                sf->p = r(t1);
                sf->n = -unit_vector(this->center - sf->p);
                sf->wo = -(r.d);
                sf->time = t1;
            }else
            {
                sf->p = r(t2);
                sf->n = -unit_vector(this->center - sf->p);
                sf->wo = -(r.d);
                sf->time = t2;
            }

            return true;
        }

         return false;
    }

    bool Sphere::intersect_p( const Ray& r ) const
    {
        float result;

        float A, B, C;

        A = dot(r.d, r.d);
        B = 2.0 * dot((r.o - this->center), r.d);
        C = dot((r.o - this->center), (r.o - this->center)) - pow(this->radius,2);



        result = pow(B,2) - 4.0*A*C;

        if(result >= 0.0 & result < 10000.0)
        {
            auto t  = (-B - sqrt(result)) / (2.0*A);


            if(t > r.min && t < r.limit){
                return true;
            }

            t = (-B + sqrt(result)) / (2.0*A);

            if(t > r.min && t < r.limit){
                return true;
            }

        }

        return false;
        
    }
}