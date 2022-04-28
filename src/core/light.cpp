#include "light.h"

namespace rt3
{
    PointLight::PointLight(Vector3<float> lumus, Vector3<float> scale, Vector3<float> from)
    {
        this->lumus = lumus;
        this->scale = scale;
        this->from = from;
    }

    Vector3<float> PointLight::sample_Li( Surfel& hit  /*in*/, Vector3<float> * & wi)
    {
        Vector3<float> aux = this->from - hit.p;

        aux.make_unit_vector();

        wi = new Vector3<float>{aux.x, aux.y, aux.z};

        BlinnPhong_material * bf = (BlinnPhong_material*) hit.primitive->get_material();

        auto L1 = bf->kd;
        
        //hit.primitive->get_material()->getKd(); // Placeholder

        L1 *= this->lumus;

        L1 = L1 * std::max(0.f, dot(hit.n, aux)); 

        auto aux2 = hit.wo + aux;

        auto h = aux2 / aux2.length();

        auto g = bf->g;

        auto L2 = bf->ks;

        L2 *= this->lumus;

        L2 = L2 * std::max(0.f, pow(dot(hit.n, h), g));

        auto L3 = L1 + L2;

        return L3;
    }

    DirectionalLight::DirectionalLight(Vector3<float> lumus, Vector3<float> scale, Vector3<float> from, Vector3<float> to)
    {
        this->lumus = lumus;
        this->scale = scale;
        this->from = from;
        this->to = to;
    }

    Vector3<float> DirectionalLight::sample_Li( Surfel& hit  /*in*/, Vector3<float> * & wi)
    {
        Vector3<float> aux = this->from - this->to;

        aux.make_unit_vector();

        wi = new Vector3<float>{aux.x, aux.y, aux.z};
        //wi.x = aux.x;
        //wi.y = aux.y;
        //wi.z = aux.z;

        BlinnPhong_material * bf = (BlinnPhong_material*) hit.primitive->get_material();

        auto L1 = bf->kd;
        
        //hit.primitive->get_material()->getKd(); // Placeholder

        L1 *= this->lumus;

        L1 = L1 * std::max(0.f, dot(hit.n, aux)); 

        auto aux2 = hit.wo + aux;

        auto h = aux2 / aux2.length();

        auto g = bf->g;

        auto L2 = bf->ks;

        L2 *= this->lumus;

        L2 = L2 * std::max(0.f, pow(dot(hit.n, h), g));

        auto L3 = L1 + L2;

        return L3;
    }

    AmbientLight::AmbientLight(Vector3<float> lumus)
    {
        this->lumus = lumus;
    }

    Vector3<float> AmbientLight::sample_Li( Surfel& hit  /*in*/, Vector3<float> * & wi)
    {
        auto ka = Vector3<float>{1,1,1};

        // hiy.material->get_material()->getKa();

        auto La = ka * this->lumus;

        return La; // Return... something...
    }

    SpotLight::SpotLight(Vector3<float> lumus, Vector3<float> scale, Vector3<float> from, Vector3<float> to, float cutoff, float fallof)
    {
        this->lumus = lumus;
        this->scale = scale;
        this->from = from;
        this->to = to;
        this->cutoff = cutoff;
        this->fallof = fallof;
    }
    
    Vector3<float> SpotLight::sample_Li( Surfel& hit  /*in*/, Vector3<float> * & wi)
    {
        Vector3<float> aux = this->from - this->to;
        Vector3<float> vector_point = this->from - hit.p;

        aux.make_unit_vector();
        vector_point.make_unit_vector();

        float point_cos = dot(aux, vector_point);

        float cutoff_cos = cos(this->cutoff*(M_PI/180.0f));

        float fallof_cos = cos(this->fallof*(M_PI/180.0f));

        Vector3<float> lumus;

        if(point_cos >= fallof_cos)
        {
            lumus = this->lumus;
        }
        else if(point_cos <= cutoff_cos)
        {
            lumus = 0.f * this->lumus;
        }
        else
        {
            auto percent = (point_cos - cutoff_cos) / (fallof_cos - cutoff_cos);

            percent = (percent * percent) * (percent * percent);

            lumus = this->lumus * percent;
        }

        wi = new Vector3<float>{aux.x, aux.y, aux.z};

        BlinnPhong_material * bf = (BlinnPhong_material*) hit.primitive->get_material();

        auto L1 = bf->kd;
        
        //hit.primitive->get_material()->getKd(); // Placeholder

        L1 *= lumus;

        L1 = L1 * std::max(0.f, dot(hit.n, aux)); 

        auto aux2 = hit.wo + aux;

        auto h = aux2 / aux2.length();

        auto g = bf->g;

        auto L2 = bf->ks;

        L2 *= lumus;

        L2 = L2 * std::max(0.f, pow(dot(hit.n, h), g));

        auto L3 = L1 + L2;

        return L3;
    }

    Light * create_ambient_light( const ParamSet &ps )
    {
        Vector3<float> lumus = retrieve(ps, "L", Vector3<float>{1.0,1.0,1.0});
		
        AmbientLight * al = new AmbientLight{lumus};
        al->type = 1;
        return al;
    }

    Light * create_directional_light( const ParamSet &ps )
    {
        Vector3<float> lumus = retrieve(ps, "L", Vector3<float>{1.0,1.0,1.0});
		Vector3<float> scale = retrieve(ps, "scale", Vector3<float>{1.0,1.0,1.0});
        Vector3<float> from = retrieve(ps, "from", Vector3<float>{1.0,1.0,1.0});
        Vector3<float> to = retrieve(ps, "to", Vector3<float>{1.0,1.0,1.0});

        DirectionalLight * dl = new DirectionalLight{lumus, scale, from, to};
        dl->type = 2;

        return dl;
    }

    Light * create_point_light( const ParamSet &ps )
    {
        Vector3<float> lumus = retrieve(ps, "I", Vector3<float>{1.0,1.0,1.0});
		Vector3<float> scale = retrieve(ps, "scale", Vector3<float>{1.0,1.0,1.0});
        Vector3<float> from = retrieve(ps, "from", Vector3<float>{1.0,1.0,1.0});

        PointLight * pl = new PointLight{lumus, scale, from};
        pl->type = 3;
        return pl;
    }

    Light * create_spot_light( const ParamSet &ps )
    {
        Vector3<float> lumus = retrieve(ps, "I", Vector3<float>{1.0,1.0,1.0});
		Vector3<float> scale = retrieve(ps, "scale", Vector3<float>{1.0,1.0,1.0});
        Vector3<float> from = retrieve(ps, "from", Vector3<float>{1.0,1.0,1.0});
        Vector3<float> to = retrieve(ps, "to", Vector3<float>{1.0,1.0,1.0});
        float cutoff = retrieve(ps, "cutoff", float{1.0});
        float falloff = retrieve(ps, "falloff", float{1.0});

        SpotLight * sl = new SpotLight{lumus, scale, from, to, cutoff, falloff};
        sl->type = 4;
        return sl;
    }
}