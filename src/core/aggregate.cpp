#include "aggregate.h"

namespace rt3
{
    Vector3<float> PrimList::world_bounds()
    {
        return Vector3<float>{1,1,1};
    }
    // Olhar o maior Z
    bool PrimList::intersect( const Ray& r, Surfel* &sf ) const
    {
        std::vector<Surfel*> surfs;
        std::vector<Primitive*> aux_prim;

        for(Primitive* p : primitives)
        {
            Surfel* aux = new Surfel(sf->p, sf->n, sf->wo, sf->time, sf->uv, p);
            if (p->intersect(r, aux))
            {
                surfs.push_back(aux);
                aux_prim.push_back(p);
            }
        }

        if(!surfs.empty())
        {

            float min_time = std::numeric_limits<float>::max();

            int i{0};
            Primitive* aux_p;

            for(Surfel* e : surfs)
            {	
                if(e->time < min_time)
                {
                    min_time = e->time;
                    sf = e;
                    aux_p = aux_prim[i];
                }
                i++;
            }

            //sf = new Surfel(sf->p, sf->n, sf->wo, sf->time, sf->uv, aux_p);

            return true;
        }
        else
        {
            return false;
        }
    }

    bool PrimList::intersect_p( const Ray& r ) const
    {
        for(Primitive* p : primitives)
        {
            if (p->intersect_p(r))
            {
                return true;
            }
        }
    }


    bool PrimList::intersect_p( const Ray& r , Primitive * & p) const
    {
        for(Primitive* pr : primitives)
        {
            if (p != pr && pr->intersect_p(r))
            {
                return true;
            }
        }

        return false;
    }
}