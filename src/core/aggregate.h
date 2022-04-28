#ifndef AGREGATE_H
#define AGREGATE_H

#include "rt3-base.h"
#include "Geometry.hpp"
#include <stdio.h>
#include <vector>
#include <limits>

namespace rt3
{
	class Material;

	class Surfel;

	class Primitive;

	class AggregatePrimitive
	{
		public:
			AggregatePrimitive(){ /*Empty*/ }

			virtual Material* get_material()
			{
				std::cout << "============================= You shoud not be calling this method!!! =============================" << std::endl;
			}

			virtual bool intersect( const Ray& r, Surfel* &sf ) const
			{
				return 0;
			}

			virtual bool intersect_p( const Ray& r ) const
			{
				return 0;
			}

			virtual bool intersect_p( const Ray& r, Primitive * & p ) const
			{
				return 0;
			}
	};

	class PrimList : public AggregatePrimitive
	{
		public:
			std::vector<Primitive*> primitives;

			PrimList(vector<Primitive*> & objects) : primitives(objects){ }

			Vector3<float> world_bounds();
			// Olhar o maior Z
			bool intersect( const Ray& r, Surfel* &sf ) const override;

			bool intersect_p( const Ray& r ) const override;

			bool intersect_p( const Ray& r, Primitive * & p ) const override;
	};
}

#endif