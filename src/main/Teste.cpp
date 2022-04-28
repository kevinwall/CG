/*
#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include "../core/triangle.h"

int main(void)
{
    // Here I'm "simulating" a ParamSet and manually filling in that object
    // with information about the triangle mesh. In particular, I'm creating
    // a situation in which the XML file has the following tag.
    // <object type="trianglemesh" filename="source/data/plane.obj" back_face_cull="on"
    //  reverse_vertex_order="false" compute_normals="fase"\>
    //
    rt3::ParamSet ps;
    //ps["filename"] = "source/data/teapot-low.obj";
    std::shared_ptr<Value<string>> p(new Value<string>("source/data/teapot-low.obj"));
    ps.insert({"filename", p});
    // ps["filename"] = "source/data/plane.obj";

    //ps["backface_cull"] = "on";

    std::shared_ptr<Value<string>> p2(new Value<string>("on"));
    ps.insert({"backface_cull", p2});

    //ps["reverse_vertex_order"] = "false";

    std::shared_ptr<Value<string>> p3(new Value<string>("false"));
    ps.insert({"reverse_vertex_order", p3});

    //ps["compute_normals"] = "false";

    std::shared_ptr<Value<string>> p4(new Value<string>("false"));
    ps.insert({"compute_normals", p4});

    bool flip_normals{false}; // This info comes from an API command.

    // Create triangle mesh. Entry-point for our mesh handling code.
    auto tri_list = rt3::create_triangle_mesh_shape( flip_normals, ps ); 

    std::cout << "\n\n>>> Accessing mesh data:\n";

    // DEBUG: Printing out the triangles.
    size_t tri_count{0};
    for ( const auto & tri : tri_list )
    {
        std::cout << "Triangle #" << ++tri_count << '\n';
        // Cast shape back to a triangle.
        const rt3::Triangle* pt_tri = dynamic_cast< rt3::Triangle* >( tri.get() );
        std::cout << *pt_tri << std::endl;
    }

    return 0;
}
*/