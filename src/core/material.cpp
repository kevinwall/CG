#include "material.h"

namespace rt3
{
    Flat_material::Flat_material(ColorXYZ mat_color)
    {
        this->mat_color = mat_color;
    }

    void Flat_material::set_color(ColorXYZ mat_color)
    {
        this->mat_color = mat_color;
    }

    ColorXYZ Flat_material::get_color() const
    {
        return this->mat_color;
    }

    void Flat_material::scatter(Surfel* isec, Ray* r)
    {
        // Empty
    }

    BlinnPhong_material::BlinnPhong_material(ColorXYZ mat_color, Vector3<float> ambient, Vector3<float> diffuse, Vector3<float> specular, float glossiness, Vector3<float> mirror)
    {
        this->mat_color = mat_color;
        ka = ambient;
        kd = diffuse;
        ks = specular;
        g = glossiness;
        m = mirror;

    }

    void BlinnPhong_material::set_color(ColorXYZ mat_color)
    {
        this->mat_color = mat_color;
    }

    ColorXYZ BlinnPhong_material::get_color() const
    {
        return this->mat_color;
    }

    void BlinnPhong_material::scatter(Surfel* isec, Ray* r)
    {
        // Empty
    }
}