#include "ray.h"

namespace rt3
{
	void Ray::print_ray()
	{
		std::cout << "Origin of the ray: " << this->o << std::endl << "Direction of the ray: " << this->d << std::endl;
	}

	Ray::Ray(const Ray& other)
	{
		this->o = other.o;
		this->d = other.d;
	}

	void Ray::operator=(const Ray& other)
	{
		this->o = other.o;
		this->d = other.d;
	}

	void Ray::normalize(){
		this->d.make_unit_vector();
	}

	Ray operator-(const Ray& ray)
    {
        return Ray(ray.o, -ray.d);
    }
}