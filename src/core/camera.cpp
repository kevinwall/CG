#include "camera.h"

namespace rt3
{	
	PerspectiveCamera::PerspectiveCamera(Film* film, Point2i res, float fovy, float aspect_ratio, Vector3<float> position, Vector3<float> look_at, Vector3<float> up)
	{
		this->film = film;
		this->res = res;
		this->position = position;
		this->look_at = look_at;
		this->up = up;
		this->fovy = fovy;
		this->aspect_ratio = aspect_ratio;
		this->w = unit_vector(look_at - position);
		this->u = unit_vector(cross(this->up, this->w));
		this->v = unit_vector(cross(this->w, this->u));

		//float h = tan( (this->fovy * 180) / (2 * M_PI) ); // Asuming the focal distance f = 1;

		float h = tan( (this->fovy/2.f)*(M_PI/180.0f) );


		this->l = (-this->aspect_ratio) * h; // L
		this->r = this->aspect_ratio * h;    // R
		this->b = -h;                        // B
		this->t = h;                         // T
	}

	void PerspectiveCamera::print_camera()
	{
		std::cout << "-----------Printando os dados da câmera--------------------" << std::endl;
		std::cout << "Posição: " << this->position << std::endl << "Look at: " << this->look_at << std::endl
		<< "Up: [ " << this->up << " ]" << std::endl << "Fovy: " << this->fovy << std::endl << "Aspect Ratio: " << this->aspect_ratio << std::endl
		<< "W: [ " << this->w << " ]" << std::endl << "U: [ " << this->u << " ]" << std::endl << "V: [ " << this->v << " ]" << std::endl
		<< "Screen Window: [ " << this->l << " " << this->r << " " << this->b << " " << this->t << " ]" << std::endl;   
	}

	Ray PerspectiveCamera::generate_ray(int i, int j)
	{
		
		float u = this->l + (this->r - this->l) * (i+0.5) / res.x; // where 1920 is the number of pixels in the horizontal plane of the image

		float v = this->b + (this->t - this->b) * (j+0.5) / res.y; // where 1080 is the number of pixels in the vertical plane of the image

		return Ray(this->position, this->w + (u*this->u) + (v*this->v));
	}

	OrthographicCamera::OrthographicCamera(Film* film, Point2i res, std::vector<float> screen_window, Vector3<float> position, Vector3<float> look_at, Vector3<float> up)
	{
		this->film = film;
		this->res = res;
		this->position = position;
		this->look_at = look_at;
		this->up = up;
		this->w = unit_vector(look_at - position);
		this->u = unit_vector(cross(this->up, this->w));
		this->v = unit_vector(cross(this->w, this->u));

		this->l = screen_window[0];
		this->r = screen_window[1];
		this->b = screen_window[2];
		this->t = screen_window[3];
	}

	Ray OrthographicCamera::generate_ray(int i, int j)
	{
		float u = this->l + (this->r - this->l) * (i+0.5) / res.x; // where 1920 is the number of pixels in the horizontal plane of the image

		float v = this->b + (this->t - this->b) * (j+0.5) / res.y; // where 1080 is the number of pixels in the vertical plane of the image

		return Ray(this->position + (u*this->u) + (v*this->v), this->w);
	}

	Camera * create_perspective_camera( const ParamSet &cps, const ParamSet & lps, Film & film )
    {

        //Camera * c{};
        Point2i res = film.get_resolution();

		float fovy = retrieve(cps, "fovy",float{0.0});
		float aspect_ratio = ((float) res.x)/res.y;
		Vector3<float> position = retrieve(lps, "look_from", Vector3f{0,0,0});
		Vector3<float> look_at = retrieve(lps, "look_at", Vector3f{0,0,0});
		Vector3<float> up = retrieve(lps, "up", Vector3f{0,1,0});

		//PerspectiveCamera pc(res, fovy, aspect_ratio, position, look_at, up);

		Film *f2 = new Film(film);

		std::cout << "Creating camera" << std::endl;
		PerspectiveCamera * pc = new PerspectiveCamera{f2, res, fovy, aspect_ratio, position, look_at, up};
		std::cout << "After create camera" << std::endl;
		//PerspectiveCamera * pc = new PerspectiveCamera(Point2i{40,60}, 30, 1080.00/1920.00, Vector3<float>{0,4,-11}, Vector3<float>{0,1,0}, Vector3<float>{0,1,0});
		//c = &pc;

		//pc->generate_ray(0,0).print_ray();

		return pc;
		
		
    }

	Camera * create_orthographic_camera( const ParamSet &cps, const ParamSet & lps, Film & film )
    {
    	Point2i res = film.get_resolution();

    	std::vector<float> sw = retrieve(cps, "screen_window",std::vector<float>{0.0, 0.0, 0.0, 0.0});
		Vector3<float> position = retrieve(lps, "look_from", Vector3f{0,0,0});
		Vector3<float> look_at = retrieve(lps, "look_at", Vector3f{0,0,0});
		Vector3<float> up = retrieve(lps, "up", Vector3f{0,1,0});


		Film *f2 = new Film(film);
		OrthographicCamera * oc = new OrthographicCamera{f2, res, sw, position, look_at, up};

		return oc;
    }
}

/*
int main(int argc, char const *argv[])
{
	//rt3::Film film(Vector2<int>{1920,1080}, "Saida.png" , rt3::Film::image_type_e::PNG);

	rt3::Camera *c;
	
	rt3::PerspectiveCamera d(30, 1080.00/1920.00, Vector3<float>{0,4,-11}, Vector3<float>{0,1,0}, Vector3<float>{0,1,0});

	c = &d;

	rt3::Ray r = c->generate_ray(1,0);

	r.print_ray();

	return 0;
}
*/