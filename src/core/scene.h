#ifndef SCENE_H
#define SCENE_H

//#include "primitive.h"
#include <memory>
#include <vector>
#include "aggregate.h"

namespace rt3{

    class AggregatePrimitive;

    class Scene{
        
        public:
        Scene(Background* & bg, AggregatePrimitive* objects) : 
        bg(bg), objects(objects)
        { }

        Background* bg;
        AggregatePrimitive* objects;

    };

}


#endif