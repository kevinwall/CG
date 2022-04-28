#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "film.h"
#include "Geometry.hpp"
#include <stdio.h>
#include <math.h>

namespace rt3 
{
	//class Film;

	class Camera
	{
		public:
			Film* film;
			Point2i res{0,0};
			float l;
			float r;
			float b;
			float t;
			Vector3<float> up; // This probably should be deleted
			Vector3<float> position;
			Vector3<float> look_at;
			Vector3<float> u;
			Vector3<float> w;
			Vector3<float> v;

	    	virtual Ray generate_ray(int i, int j){ return Ray();}
	    	Camera(){}
	    	~Camera()
	    	{
	    		delete film;
	    	}
	};

	class PerspectiveCamera : public Camera 
	{
		public:
			float fovy;
			float aspect_ratio;
			// Gathering all the information that the perspective camera needs to be built
			// Paramethers:
			// film -> the film of the camera, responsable for representing the color of every pixel in the image
			// fovy -> the angle (in degrees, if not mistaken) representing the field of view of the camera (the size of the hole)
			// aspect_ratio -> the proportion of the image (i guess...)
			// position -> the camera's position in space
			// look_at -> the point that the camera is looking at
			// up -> the up vector of the camera
			PerspectiveCamera(Film* film, Point2i res, float fovy, float aspect_ratio, Vector3<float> position, Vector3<float> look_at, Vector3<float> up);
			void print_camera();
			Ray generate_ray(int i, int j) override;
	};
	
	class OrthographicCamera : public Camera 
	{
		public:
			OrthographicCamera(Film* film, Point2i res, std::vector<float> screen_window, Vector3<float> position, Vector3<float> look_at, Vector3<float> up);
			Ray generate_ray(int i, int j) override;
	};

	Camera * create_perspective_camera( const ParamSet &cps, const ParamSet & lps, Film & film  );
	Camera * create_orthographic_camera( const ParamSet &cps, const ParamSet & lps, Film & film );
}

#endif