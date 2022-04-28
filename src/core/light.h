#ifndef LIGHT_H
#define LIGHT_H

#include "surfel.h"
#include "Geometry.hpp"
#include "paramset.h"
#include <algorithm>
#include <cmath>

namespace rt3
{
    class Light 
    {
	    public:
		    Vector3<float> lumus;
            int type;
	    public:
		    virtual ~Light(){}
		    Light(){}
		    
		    virtual Vector3<float> sample_Li( Surfel& hit  /*in*/, Vector3<float> * & wi  /*out*/ /*, VisbilityTester *vis removed in this instance for testing*//*out*/ )
            {
                return Vector3<float>{1,1,1};
            }

		    //virtual void preprocess( const Scene & ) {}
    };

    class PointLight : public Light
    {
        public:
            Vector3<float> scale;
            Vector3<float> from;

            PointLight(Vector3<float> lumus, Vector3<float> scale, Vector3<float> from);

            Vector3<float> sample_Li(Surfel& hit  /*in*/, Vector3<float> * & wi) override;
    };

    class DirectionalLight : public Light
    {
        public:
            Vector3<float> scale;
            Vector3<float> from;
            Vector3<float> to;

            DirectionalLight(Vector3<float> lumus, Vector3<float> scale, Vector3<float> from, Vector3<float> to);

            Vector3<float> sample_Li( Surfel& hit  /*in*/, Vector3<float> * & wi) override;
    };

    class AmbientLight : public Light
    {
        public:
            AmbientLight(Vector3<float> lumus);

            Vector3<float> sample_Li( Surfel& hit  /*in*/, Vector3<float> * & wi) override; 
    };

    class SpotLight : public Light
    {
        public:
            Vector3<float> scale;
            Vector3<float> from;
            Vector3<float> to;
            float cutoff;
            float fallof;

            SpotLight(Vector3<float> lumus, Vector3<float> scale, Vector3<float> from, Vector3<float> to, float cutoff, float fallof);
            
            Vector3<float> sample_Li( Surfel& hit  /*in*/, Vector3<float> * & wi) override;
    };
    
    Light * create_ambient_light( const ParamSet &ps );

    Light * create_directional_light( const ParamSet &ps );

    Light * create_point_light( const ParamSet &ps );

    Light * create_spot_light( const ParamSet &ps );
}

#endif