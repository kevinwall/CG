#ifndef SURFEL_H
#define SURFEL_H

#include "primitive.h"

namespace rt3
{
	class Primitive;

	class Surfel{
		public:
			Surfel( const Point3f&p,const Vector3f&n, const Vector3f&wo, float time, 
				const Point2f& uv)
				: p{p}, n{n}, wo{wo}, time{time}, uv{uv}
	            {/* empty */};
	        Surfel( const Point3f&p,const Vector3f&n, const Vector3f&wo, float time, 
				const Point2f& uv, Primitive* obj)
				: p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{obj}
	            {/* empty */};


			Point3f p;      //!< Contact point.
			Vector3f n;     //!< The surface normal.
			Vector3f wo;    //!< Outgoing direction of light, which is -ray.
			float time;     //!< Time of contact.
			Point2f uv;     //!< Parametric coordinate (u,v) of the hit surface.
			Primitive *primitive; //!< Pointer to the primitive.
	};
}

#endif