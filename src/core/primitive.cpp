#include "primitive.h"

namespace rt3
{
    GeometricPrimitive::GeometricPrimitive(Shape* s, Material* m)
    {
        this->shape = s;
        this->material = m;
    }

    Vector3<float> GeometricPrimitive::world_bounds()
    {
        return Vector3<float>{1,2,3};
    }

    bool GeometricPrimitive::intersect( const Ray& r, Surfel* &sf ) const
    {	
        return this->shape->intersect(r, sf);
    }

    bool GeometricPrimitive::intersect_p( const Ray& r ) const
    {
        return this->shape->intersect_p(r);
    }

    Material * GeometricPrimitive::get_material(void)
    {
        return this->material;
    }

    void GeometricPrimitive::set_material(Material* material)
    {
        this->material = material;
    }

    Shape* GeometricPrimitive::get_shape(){
        return shape;
    }
}