#include "api.h"

#define TINYOBJLOADER_IMPLEMENTATION

namespace rt3 {
            
//=== API's static members declaration and initialization.
API::APIState API::curr_state = APIState::Uninitialized;
RunningOptions API::curr_run_opt;
unique_ptr< RenderOptions > API::render_opt;
// GraphicsState API::curr_GS;

// THESE FUNCTIONS ARE NEEDED ONLY IN THIS SOURCE FILE (NO HEADER NECESSARY)
// ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ

Film * make_film( const std::string &type, const ParamSet &ps )
{
    std::cout << ">>> Inside API::make_film()\n";
    Film *film{ nullptr };
    film = create_film( ps );

    // Return the newly created film.
    return film;
}

Background * make_background( const std::string &type, const ParamSet& ps )
{
    std::cout << ">>> Inside API::background()\n";
    Background *bkg{ nullptr };
    bkg = create_color_background( ps );

    // Return the newly created background.
    return bkg;
}

Camera * make_camera(const std::string &type, const ParamSet& cps, const ParamSet& lps, Film & film){

    std::cout << ">>> Inside API::make_camera()\n";
    Camera *camera{ nullptr };


    if (type == "perspective")
        camera = create_perspective_camera(cps, lps, film);
    else if (type == "orthographic")
        camera = create_orthographic_camera(cps, lps, film);


    return camera;

}

Material * make_material(const ParamSet &ps){
    std::cout << ">>> Inside API::make_material()\n";
    Material *material{ nullptr };

    string type = retrieve(ps, "type", string("flat"));
    ColorXYZ color = retrieve(ps, "color", ColorXYZ{255,0,0});

    if (type == "flat"){
        material = new Flat_material(color);
    } else if (type == "blinn") {
        Vector3f ambient = retrieve(ps, "ambient", Vector3f{0,0,0});
        Vector3f diffuse = retrieve(ps, "diffuse", Vector3f{0,0,0});
        Vector3f specular = retrieve(ps, "specular", Vector3f{0,0,0});
        Vector3f mirror = retrieve(ps, "mirror", Vector3f{0,0,0});
        float glossiness = retrieve(ps, "glossiness", float{0});
        material = new BlinnPhong_material(color,ambient,diffuse,specular,glossiness, mirror);
    }
    

    return material;
}



ParamSet API::get_material(int id){
    return render_opt->materials[id];
}


Shape * create_sphere(const ParamSet &ps)
{
    float radius = retrieve(ps, "radius", 1.f);
    Point3f center = retrieve(ps, "center", Point3f{0,0,0});

    Shape * prim = new Sphere(center,radius);

    return prim;
}

vector<Shape *> create_triangle_mesh(const ParamSet &ps)
{

    vector< Shape * > triangles = rt3::create_triangle_mesh_shape(false, ps);
    /*
    for (auto & x : triangles)
        t.push_back(x.get());

    for (auto & x : t){
        Triangle * tr = (Triangle *) x;
        cout << *tr << endl;
    }
    */
    return triangles;
}

Primitive* create_geometric_primitive(Shape* s, Material* m)
{
    Primitive* prim = new GeometricPrimitive(s, m);
}
/*
Primitive* create_geometric_form(Shape* s, Material* m)
{
    Primitive* prim = new GeometricPrimitive(s, m);
}
*/

vector<Primitive *> make_object(const ParamSet &ps){
    std::cout << ">>> Inside API::make_object()\n";
    vector<Primitive *> primitives;

    string type = retrieve(ps, "type", string("sphere"));

    if(type == "sphere")
    {
        Shape* s{nullptr};
        Material* m{nullptr};

        s = create_sphere(ps);

        int mtr_id = retrieve(ps, "material", int{-1});

        if (mtr_id >= 0){
            m = make_material( API::get_material(mtr_id) );
        }
        else 
        {
            m = new Flat_material(ColorXYZ{0,255,0}); //makes a material;
        }

        primitives.push_back(create_geometric_primitive(s, m));

    } else if (type == "trianglemesh"){
        auto triangles = create_triangle_mesh(ps);

        for (auto & s : triangles) {
            Material* m{nullptr};

            int mtr_id = retrieve(ps, "material", int{-1});

            if (mtr_id >= 0){
                m = make_material( API::get_material(mtr_id) );
            }
            else 
            {
                m = new Flat_material(ColorXYZ{0,255,0}); //makes a material;
            }

            primitives.push_back(create_geometric_primitive(s, m));
        }

        
    }

    return primitives;
}

Scene * make_scene(vector<ParamSet>& ps, Background* & background ){
    std::cout << ">>> Inside API::make_scene()\n";

    vector<Primitive*> objects;

    for(ParamSet& e : ps)
    {
        vector<Primitive*> spl = make_object(e);
        for (auto & sp : spl)
            objects.push_back(sp);
    }

    AggregatePrimitive* prim = new PrimList(objects);

    Scene *scene = new Scene{ background, prim };

    return scene;
}

Light * make_light(const ParamSet &ps){
    std::cout << ">>> Inside API::make_light()\n";
    Light * light{nullptr};

    string type = retrieve(ps, "type", string("ambient"));

    if (type == "ambient"){
        light = create_ambient_light(ps);
    } else if (type == "directional"){
        light = create_directional_light(ps);
    } else if (type == "point"){
        light = create_point_light(ps);
    } else if (type == "spot"){
        light = create_spot_light(ps);
    }

    return light;
}

Integrator * make_integrator(const std::string &type, Camera * & camera, vector<ParamSet> & lights, const ParamSet &ps ){
    std::cout << ">>> Inside API::make_integrator()\n";

    Integrator * integrator{nullptr};

    if (type == "flat"){
        integrator = new FlatIntegrator{ camera };
    } else if (type == "blinn_phong"){
        int depth = retrieve(ps, "depth", int{0});
        integrator = new BlinnPhongIntegrator{ camera, depth };
        
        
    }

    for (auto x : lights){
        integrator->lights.push_back(make_light(x));
    }

    return integrator;
}

/*
void render( shared_ptr<Scene> & scene){
    Point2i res = scene->camera->film->m_full_resolution;
    int w = res.x;
    int h = res.y;

    // render background

    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            //std::cout << "Testando o filme no passo 1" << std::endl;
            scene->camera->film->add_sample({j,i}, scene->bg->sampleXYZ({ ((float) j)/w, ((float) i)/h }));
            //std::cout << "Fim do teste do filme no passo 1" << std::endl;

            //Ray ray = scene->camera->generate_ray(w-j,h-i);
            Ray ray = scene->camera->generate_ray(w-j,h-i);
            ray.normalize();
            //ray.d = -ray.d;
            //scene->film->add_sample({20,20}, ColorXYZ{255,0,0});

            for(shared_ptr<Primitive> & p : scene->objects){
                if (p->intersect_p(ray)){
                    //std::cout << "Testando o filme no passo 2" << std::endl;
                    scene->camera->film->add_sample({j,i}, p->get_material()->get_color());
                    //std::cout << "Fim do teste do filme no passo 2" << std::endl;
                }

            }
            
        }
    }
    
    scene->camera->film->write_image();

    
}
*/

// ˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆ

// END OF THE AUXILIARY FUNCTIONS
// =========================================================================

//=== API's public methods implementation
void API::init_engine( const RunningOptions & opt )
{
   // Save running option sent from the main().
   curr_run_opt = opt;
   // Check current machine state.
   if ( curr_state != APIState::Uninitialized )
       RT3_ERROR( "API::init_engine() has already been called! ");
   // Set proper machine state
   curr_state = APIState::SetupBlock;
   // Preprare render infrastructure for a new scene.
   render_opt = make_unique< RenderOptions >(); 
   // Create a new initial GS
   // curr_GS = GraphicsState();
   RT3_MESSAGE( "[1] Rendering engine initiated.\n" );
}

void API::clean_up( void )
{
    // Check for correct machine state
    if ( curr_state == APIState::Uninitialized )
        RT3_ERROR("API::clean_up() called before engine initialization.");
    else if ( curr_state == APIState::WorldBlock )
        RT3_ERROR("API::clean_up() called inside world definition section.");
    curr_state = APIState::Uninitialized;

    RT3_MESSAGE( "[4] Rendering engine clean up concluded. Shutting down...\n" );
}

void API::run( void )
{
    // Try to load and parse the scene from a file.
    RT3_MESSAGE( "[2] Beginning scene file parsing...\n" );
    // Recall that the file name comes from the running option struct.
    parse( curr_run_opt.filename.c_str() );
}


void API::world_begin( void )
{
    VERIFY_SETUP_BLOCK("API::world_begin");      // check for correct machine state.
    curr_state = APIState::WorldBlock;      // correct machine state.
}

void API::world_end( void )
{
    //VERIFY_WORLD_BLOCK("API::world_end");
    // The scene has been properly set up and the scene has
    // already been parsed. It's time to render the scene.

    // At this point, we have the background as a solitary pointer here.
    // In the future, the background will be parte of the scene object.
    Background* the_background{ make_background(render_opt->bkg_type, render_opt->bkg_ps) };
    // Same with the film, that later on will belong to a camera object.
    Film* the_film{ make_film(render_opt->film_type, render_opt->film_ps) };

    Camera* the_camera{ make_camera(render_opt->camera_type, render_opt->camera_ps, render_opt->lookat_ps, *the_film) };

    //Material* the_material { make_material(render_opt->material_type, render_opt->material_ps)};

    Scene* the_scene{ make_scene(render_opt->objects, the_background) };

    Integrator * integrator { make_integrator(render_opt->integrator_type, the_camera, render_opt->lights, render_opt->integrator_ps) };

    // Run only if we got film and background.
    if ( the_film and the_background )
    {
        RT3_MESSAGE( "    Parsing scene successfuly done!\n" );
        RT3_MESSAGE( "[2] Starting ray tracing progress.\n" );

        // Structure biding, c++17.
        auto res  = the_film->get_resolution();
        size_t w = res[0];
        size_t h = res[1];
        RT3_MESSAGE( "    Image dimensions in pixels (W x H): " + std::to_string(w) + " x " + std::to_string(h) + ".\n" );
        RT3_MESSAGE( "    Ray tracing is usually a slow process, please be patient: \n" );

        //================================================================================
        auto start = std::chrono::steady_clock::now();
        integrator->render(the_scene); // TODO: This is the ray tracer's  main loop.
        auto end = std::chrono::steady_clock::now();
        //================================================================================
        auto diff = end - start; //Store the time difference between start and end
        // Seconds
        auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
        RT3_MESSAGE( "    Time elapsed: " + std::to_string(diff_sec.count()) + " seconds (" +
                 std::to_string( std::chrono::duration <double, std::milli> (diff).count() ) +  " ms) \n" );
    }
    // [4] Basic clean up
    curr_state = APIState::SetupBlock;      // correct machine state.
    reset_engine();
}



/// This api function is called when we need to re-render the *same* scene (i.e. objects, lights, materials, etc) , maybe with different integrator, and camera setup.
/// Hard reset on the engine. User needs to setup all entities, such as camera, integrator, accelerator, etc.
void API::reset_engine(void)
{
    // curr_GS = GraphicsState();
    // This will delete all information on integrator, cameras, filters,
    // acceleration structures, etc., that has been set previously.
    //render_opt.reset( new RenderOptions );

    render_opt->lights.clear();
}

void API::background( const ParamSet& ps )
{
    std::cout << ">>> Inside API::background()\n";
    VERIFY_WORLD_BLOCK("API::background");

    // retrieve type from ps.
    string type = retrieve( ps, "type", string{"unknown"} );
    render_opt->bkg_type = type;
    // Store current background object.
    render_opt->bkg_ps = ps;
}

void API::film( const ParamSet &ps )
{
    std::cout << ">>> Inside API::film()\n";
    VERIFY_SETUP_BLOCK("API::film");

    // retrieve type from ps.
    string type = retrieve( ps, "type", string{"unknown"} );
    render_opt->film_type = type;
    render_opt->film_ps = ps;

}

void API::camera( const ParamSet &ps )
{
    std::cout << ">>> Inside API::camera()\n";
    VERIFY_SETUP_BLOCK("API::camera");

    // retrieve type from ps.
    string type = retrieve( ps, "type", string{"unknown"} );
    render_opt->camera_type = type;
    render_opt->camera_ps = ps;

}

void API::lookat( const ParamSet &ps )
{
    std::cout << ">>> Inside API::lookat()\n";
    VERIFY_SETUP_BLOCK("API::lookat");

    render_opt->lookat_ps = ps;

}

void API::material( const ParamSet &ps )
{
    std::cout << ">>> Inside API::material()\n";

    render_opt->materials.push_back(ps);
    render_opt->material_id++;

}

void API::make_named_material( const ParamSet &ps )
{
    std::cout << ">>> Inside API::make_named_material()\n";

    string name = retrieve( ps, "name", string{"unknown"} );

    //render_opt->named_materials.insert(pair<string, ParamSet>(name, ps));
    render_opt->named_materials[name] = ps;
}

void API::named_material( const ParamSet &ps )
{
    std::cout << ">>> Inside API::named_material()\n";

    string name = retrieve( ps, "name", string{"unknown"} );

    //render_opt->materials.push_back( render_opt->named_materials.find(name).second );
    render_opt->materials.push_back( render_opt->named_materials[name]);
    render_opt->material_id++;

}


void API::object( ParamSet &ps )
{
    std::cout << ">>> Inside API::object()\n";
    //VERIFY_SETUP_BLOCK("API::object");

    if (render_opt->material_id > 0){
        std::string mtr = "material";
        const char * cname = const_cast<char *>(mtr.c_str());
        
        std::shared_ptr<Value<int>> p(new Value<int>(render_opt->material_id - 1));
        ps.insert({cname, p});
    }

    render_opt->objects.push_back(ps);

}

void API::integrator( const ParamSet &ps )
{
    std::cout << ">>> Inside API::integrator()\n";
    //VERIFY_SETUP_BLOCK("API::integrator");

    string type = retrieve( ps, "type", string{"unknown"} );
    render_opt->integrator_type = type;
    render_opt->integrator_ps = ps;

}

void API::light_source( const ParamSet &ps )
{
    std::cout << ">>> Inside API::light_source()\n";


    render_opt->lights.push_back(ps);

}


}  // namespace rt3
