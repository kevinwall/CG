#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt3.h"
#include "surfel.h"
#include "ray.h"

namespace rt3
{
	class Surfel;

	class Material
	{
		public:
			ColorXYZ mat_color;

			Material(){/*empty*/}

			virtual void set_color(ColorXYZ mat_color)
			{
				this->mat_color = mat_color;
			}

			virtual ColorXYZ get_color() const
			{
				return this->mat_color;
			}

			virtual void scatter(Surfel* isec, Ray* r)
			{
				// Empty
			}

	};

	class Flat_material : public Material
	{
		public:
			Flat_material(){/*empty*/}

			Flat_material(ColorXYZ mat_color);

			void set_color(ColorXYZ mat_color) override;

			ColorXYZ get_color() const override;

			void scatter(Surfel* isec, Ray* r) override;
	};

	class BlinnPhong_material : public Material
	{
		public:

			Vector3<float> ka;
			Vector3<float> kd;
			Vector3<float> ks;
			float g;
			Vector3<float> m;

			BlinnPhong_material(){/*empty*/}

			BlinnPhong_material(ColorXYZ mat_color, Vector3<float> ambient, Vector3<float> diffuse, Vector3<float> specular, float glossiness, Vector3<float> mirror);

			void set_color(ColorXYZ mat_color) override;

			ColorXYZ get_color() const override;

			void scatter(Surfel* isec, Ray* r) override;

	};
}


#endif