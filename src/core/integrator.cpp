#include "integrator.h"

namespace rt3{

    //Integrator::Integrator(Camera * c) : camera(c) { /* Empty */ }

    void FlatIntegrator::render(Scene* & scene){
        Point2i res = camera->film->m_full_resolution;
        int w = res.x;
        int h = res.y;

        // render background

        for (int i = 0; i < h; i++){
            for (int j = 0; j < w; j++){
                camera->film->add_sample({j,i}, scene->bg->sampleXYZ({ ((float) j)/w, ((float) i)/h }));

                Ray ray = camera->generate_ray(w-j,h-i);
                ray.normalize();

                Surfel* sf = new Surfel(Vector3<float>{1,1,1}, Vector3<float>{1,1,1}, Vector3<float>{1,1,1}, 0.f, Vector2<float>{j,i});

                if (scene->objects->intersect(ray, sf))
                {
                    camera->film->add_sample({j,i}, sf->primitive->get_material()->get_color());
                }

                delete sf;
            }
        }
        
        camera->film->write_image();
    }

    void BlinnPhongIntegrator::render(Scene* & scene){
        Point2i res = camera->film->m_full_resolution;
        int w = res.x;
        int h = res.y;

        // render background

        for (int i = 0; i < h; i++){
            for (int j = 0; j < w; j++){
                ColorXYZ bgc = scene->bg->sampleXYZ({ ((float) j)/w, ((float) i)/h });

                Ray ray = camera->generate_ray(w-j,h-i);
                ray.normalize();

                // Encapsular isso no Li
                auto L = Li(ray, scene, 0, i,j, bgc);
                camera->film->add_sample({j,i}, L);
            }
        }
        
        camera->film->write_image();
    }

    ColorXYZ BlinnPhongIntegrator::Li(Ray & ray, Scene * & scene, int depth, int i, int j, ColorXYZ bgc){
        Surfel* sf = new Surfel(Vector3<float>{1,1,1}, Vector3<float>{1,1,1}, Vector3<float>{1,1,1}, 0.f, Vector2<float>{j,i});
        //Vector3f L = Vector3f{0,0,0};
        Vector3f L = bgc;
        L.x /= 255.0f;
        L.y /= 255.0f;
        L.z /= 255.0f;


        if (scene->objects->intersect(ray, sf) and sf->time > 0)
        {
            L = Vector3f{0,0,0};
            BlinnPhong_material * bf  = (BlinnPhong_material *) sf->primitive->get_material();

            Vector3<float> * wi;

            AmbientLight * al{nullptr};

            for (Light * light : lights){
                
                auto v = light->sample_Li( *sf, wi);
                
                Ray shadow ;

                if (light->type == 2){
                    shadow = Ray(sf->p, *wi);
                } else if (light->type != 1) {
                    Vector3f from;
                    Vector3f tol = sf->p;

                    if (light->type == 3){
                        PointLight * pl = (PointLight *) light;
                        from = pl->from;
                    } else if (light->type == 4){
                        SpotLight * sl = (SpotLight *) light;
                        from = sl->from;
                    }

                    float dist = sqrt( pow(from.x - tol.x,2) + pow(from.y - tol.y,2) + pow(from.z - tol.z,2)  );

                    shadow = Ray(sf->p, *wi);
                    shadow.limit = dist;
                } else if (light->type == 1){
                    al = (AmbientLight *) light;
                }

                if (!scene->objects->intersect_p(shadow, sf->primitive)){
                    L += v;
                }

            
                
            }

            if (al != nullptr){
                L += al->lumus * bf->ka;
            }



            if (depth < mdepth){


                
                //cout << " [depth : " << depth << "] ";
                Ray reflection = Ray(sf->p, ray.d - sf->n*(2.f*(dot(ray.d,sf->n))));
                ColorXYZ pp = Li(reflection, scene, depth+1, i,j,bgc);
                //cout << "ENTRE :" << pp << " ";
                L += bf->m * pp;
                //cout << "DEPOIS :" << L << "\n\n";
            }



            if (L.x >= 1.0) L.x = 0.9999;
            if (L.y >= 1.0) L.y = 0.9999;
            if (L.z >= 1.0) L.z = 0.9999;

            
            delete sf;
            //return L;
        } 
        
        return L;
        
        //if (depth > 0)
        //    return ColorXYZ{0,0,0};

        

    }

    
}