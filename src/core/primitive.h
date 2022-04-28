#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "shape.h"
#include "material.h"
#include <stdio.h>

namespace rt3
{
	class Material;
	class Shape;

	class Primitive {
		public:
			Primitive(){/*empty*/}
			//virtual ~Primitive();
			virtual bool intersect( const Ray& r, Surfel* &sf ) const
			{
				return 0;
			}
			// Simpler & faster version of intersection that only return true/false.
			// It does not compute the hit point information.
			virtual bool intersect_p( const Ray& r ) const
			{
				return 0;
			}
			virtual Material * get_material(void)
			{
				return nullptr;
			}
			virtual Vector3<float> world_bounds()
			{
				return Vector3<float>{1,2,3};
			}
	};

	class GeometricPrimitive : public Primitive
	{
		private:
			Material* material; //  Originaly was: std::shared_ptr<Material> material;
			Shape* shape;

		public:
			GeometricPrimitive(Shape* s, Material* m);

			Vector3<float> world_bounds() override;

			bool intersect( const Ray& r, Surfel* &sf ) const override;

			bool intersect_p( const Ray& r ) const override;

			Material * get_material(void) override;

			void set_material(Material* material);

			Shape* get_shape();
	};
}

#endif