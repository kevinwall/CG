#ifndef SHAPE_H
#define SHAPE_H

#include "rt3.h"
#include "surfel.h"
#include <math.h>
#include <stdio.h>
#include "ray.h"

namespace rt3
{

	class Surfel;

	class Shape
	{
		public:
			bool flip_normals;

			Shape(){}

			Shape(bool flip_n)
			{
				this->flip_normals = flip_n;
			}

			virtual Vector3<float> world_bounds()
			{
				return Vector3<float>{1,2,3};
			}

			virtual bool intersect( const Ray& r, Surfel* &sf ) const
			{
				return 0;
			}

			virtual bool intersect_p( const Ray& r ) const
			{
				return 0;
			}
	};

	class Sphere : public Shape
	{
		public:
			Point3f center;
			float radius;

			float rmin;
			float rmax;

			Sphere(){/*empty*/}

			Sphere(Point3f center, float radius);

			bool intersect( const Ray& r, Surfel*& sf ) const override;

			bool intersect_p( const Ray& r ) const override;
	};
}

#endif