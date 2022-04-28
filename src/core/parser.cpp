/*!
 * Implementation of XML processing functions.
 * @file parser.h
 */

#include "paramset.h"
#include "parser.h"
#include "api.h"
#include "rt3.h"

#include<sstream>

// === Function Implementation

namespace rt3 {

using rt3::Point3f;
using rt3::Vector3f;
using rt3::Vector3i;

/// This is the entry function for the parsing process.
void parse( const char* scene_file_name )
{
    tinyxml2::XMLDocument xml_doc;

    // Load file.
    if ( xml_doc.LoadFile( scene_file_name ) != tinyxml2::XML_SUCCESS )
        RT3_ERROR( std::string{"The file \""} + scene_file_name + std::string{"\" either is not available OR contains an invalid RT3 scene provided!"} );

    // ===============================================
    // Get a pointer to the document's root node.
    tinyxml2::XMLNode * p_root = xml_doc.FirstChild();
    if ( p_root == nullptr )
        RT3_ERROR( "Error while trying to find \"RT3\" tag in the scene file." );
    // ===============================================

    // Get the first-level tag inside the root node.
    auto *p_child { p_root->FirstChildElement( ) };
    if ( p_child == nullptr)
        RT3_ERROR( "No \"children\" tags found inside the \"RT3\" tag. Empty scene file?" );

    parse_tags( p_child, /* initial level */ 0 );
}

/// Main loop that handles each possible tag we may find in a RT3 scene file.
void parse_tags(  tinyxml2::XMLElement *p_element, int level )
{
    // All messages sent to std::clog is for DEBUG purposes.
    std::clog << "[parse_tags()]: level is " << level << std::endl;

    // Traverse all items on the children's level. 
    while ( p_element != nullptr )
    {
        // Convert the attribute name to lowecase before testing it.
        auto tag_name = CSTR_LOWERCASE( p_element->Value() );
        clog << "\n"<< setw(level*3) << "" << "***** Tag id is `" << tag_name << "`, at level " << level << std::endl;

        // Big switch for each possible RT3 tag type.
        if ( tag_name == "background" )
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING  , "type"        },
                { param_type_e::STRING  , "filename"    }, // Texture file name.
                { param_type_e::STRING  , "mapping"     }, // Type of mapping required.
                { param_type_e::COLOR   , "color"       }, // Single color for the entire background.
                { param_type_e::COLOR   , "tl"          }, // Top-left corner
                { param_type_e::COLOR   , "tr"          }, // Top-right corner
                { param_type_e::COLOR   , "bl"          }, // Bottom-left corner
                { param_type_e::COLOR   , "br"          }  // Bottom-right corner
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            // Calling the corresponding API method.
            API::background( ps );
        }
        else if ( tag_name == "film" )
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING   , "type"            },
                { param_type_e::STRING   , "filename"        },
                { param_type_e::STRING   , "img_type"        },
                { param_type_e::INT      , "x_res"           },
                { param_type_e::INT      , "y_res"           },
                { param_type_e::ARR_REAL , "crop_window"     },
                { param_type_e::STRING   , "gamma_corrected" } // bool
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            // Calling the corresponding API method.
            API::film( ps );
        }
        else if ( tag_name == "world_begin" )
        {
            //std::clog << ">>> Entering WorldBegin, at level " << level+1 << std::endl;
            // We should get only one `world` tag per scene file.
            API::world_begin();
        }
        else if ( tag_name == "world_end" )
        {
            API::world_end();
            //std::clog << ">>> Leaving WorldBegin, at level " << level+1 << std::endl;
        } 
        else if ( tag_name == "render_again" )
        {
            API::world_end();
            //std::clog << ">>> Leaving WorldBegin, at level " << level+1 << std::endl;
        } 
        else if ( tag_name == "lookat")
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::VEC3F   , "look_from" },
                { param_type_e::VEC3F   , "look_at"   },
                { param_type_e::VEC3F   , "up"        }
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            API::lookat(ps);
        }
        else if (tag_name == "camera")
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING   , "type"           },
                { param_type_e::ARR_REAL , "screen_window"  },
                { param_type_e::REAL     , "fovy"           }
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            API::camera(ps);
        } 
        else if (tag_name == "integrator")
        {
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING , "type" },
                { param_type_e::INT    , "depth" }
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            API::integrator(ps);
        } 
        else if (tag_name == "material")
        {
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING   , "type"       },
                { param_type_e::COLOR    , "color"      },
                { param_type_e::VEC3F    , "ambient"    },
                { param_type_e::VEC3F    , "diffuse"    },
                { param_type_e::VEC3F    , "specular"   },
                { param_type_e::VEC3F    , "mirror"     },
                { param_type_e::REAL     , "glossiness" }
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            API::material(ps);
        } 
        else if (tag_name == "object"){
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING   , "type"                 },
                { param_type_e::REAL     , "radius"               },
                { param_type_e::VEC3F    , "center"               },
                { param_type_e::INT      , "ntriangles"           },
                { param_type_e::ARR_INT  , "indices"              },
                { param_type_e::ARR_REAL , "vertices"             },
                { param_type_e::ARR_REAL , "normals"              },
                { param_type_e::ARR_REAL , "uv"                   },
                { param_type_e::STRING   , "reverse_vertex_order" },
                { param_type_e::STRING   , "compute_normals"      },
                { param_type_e::STRING   , "backface_cull"        },
                { param_type_e::STRING   , "filename"             }
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            API::object(ps);
        } else if (tag_name == "light_source"){
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING   , "type"   },
                { param_type_e::VEC3F    , "L"      },
                { param_type_e::VEC3F    , "I"      },
                { param_type_e::VEC3F    , "scale"  },
                { param_type_e::VEC3F    , "from"   },
                { param_type_e::VEC3F    , "to"     },
                { param_type_e::REAL     , "cutoff" },
                { param_type_e::REAL     , "flux"   },
                { param_type_e::REAL     , "faloff" }
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            API::light_source(ps);
        } else if (tag_name == "make_named_material"){
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING   , "type"       },
                { param_type_e::COLOR    , "color"      },
                { param_type_e::STRING   , "name"       },
                { param_type_e::VEC3F    , "ambient"    },
                { param_type_e::VEC3F    , "diffuse"    },
                { param_type_e::VEC3F    , "specular"   },
                { param_type_e::VEC3F    , "mirror"     },
                { param_type_e::REAL     , "glossiness" }
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            API::make_named_material(ps);
        } else if (tag_name == "named_material"){
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING   , "name" }
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            API::named_material(ps);

        } else if (tag_name == "object"){
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING   , "type"       },
                { param_type_e::COLOR    , "color"      },
                { param_type_e::STRING   , "name"       },
                { param_type_e::VEC3F    , "ambient"    },
                { param_type_e::VEC3F    , "diffuse"    },
                { param_type_e::VEC3F    , "specular"   },
                { param_type_e::VEC3F    , "mirror"     },
                { param_type_e::REAL     , "glossiness" }
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            API::object(ps);
        }
        //else RT3_WARNING( "Undefined tag `" + tag_name + "` found!" );

        // Get next (to the right) sibling on this tree level.
        p_element = p_element->NextSiblingElement();
    }
}

/// Universal parameters parser.
/*!
 * This function receives a list of pairs <param_type, name>, traverse all the attributes found
 * in `p_element` and extract the attribute values into the `ps_out` `ParamSet` object.
 * Only named attributes found are read into the `ps_out`.
 *
 * @param p_element XML element we are extracting information from.
 * @param param_list List of pairs <param_type, name> we need to extract from the XML element.
 * @param ps_out The `ParamSet` object we need to fill in with parameter information extracted from the XML element.
 */
void parse_parameters( tinyxml2::XMLElement * p_element,
        const vector<std::pair<param_type_e, string>> param_list, ParamSet *ps_out )
{
    //std::clog << "parse_parameters(): p_element = " << p_element << endl;

    // Traverse the list of paramters pairs: type + name.
    for ( const auto& e : param_list )
    {
        const auto & [ type, name ] = e; // structured binding, requires C++ 17
        std::clog << "---Parsing att \"" << name << "\", type = " << (int)type << "\n";

        const char * cname = const_cast<char *>(name.c_str());

        

        // This is just a dispatcher to the proper extraction functions.
        switch ( type )
        {
            // TODO: Provide code or function call to each case you need at this point.
            //=============================================================================
            // ATTENTION: We do not parse bool from the XML file because TinyXML2 cannot parse one.
            // Bools are treated as strings.
            // case param_type_e::BOOL:
                // parse_single_BASIC_attrib<bool>( p_element, ps_out, name );
                // break;
            //=============================================================================
            case param_type_e::UINT:
                break;
            case param_type_e::INT:
                if (p_element->Attribute(cname) != NULL){
                    auto v = p_element->Attribute(cname);
                    std::shared_ptr<Value<int>> p(new Value<int>(std::stoi(v)));
                    ps_out->insert({cname, p});
                }
                break;
            case param_type_e::REAL:
                if (p_element->Attribute(cname) != NULL){
                    auto v = p_element->Attribute(cname);

                    std::shared_ptr<Value<float>> p(new Value<float>(std::stof(v)));
                    ps_out->insert({cname, p});
                }
                break;
            case param_type_e::STRING:

                if (p_element->Attribute(cname) != NULL){
                    auto v = p_element->Attribute(cname);
                    std::shared_ptr<Value<string>> p(new Value<string>(v));
                    ps_out->insert({cname, p});
                }
                
                break;
            case param_type_e::BOOL:

                if (p_element->Attribute(cname) != NULL){
                    auto v = p_element->Attribute(cname);
                    std::shared_ptr<Value<bool>> p(new Value<bool>(v));
                    ps_out->insert({cname, p});
                }
                
                break;
            case param_type_e::VEC3F:
                if (p_element->Attribute(cname) != NULL){

                    auto v = p_element->Attribute(cname);
                    std::stringstream ss(v);
                    float x,y,z;

                    ss >> x >> y >> z;

                    Vector3f vec = {x,y,z};
                    
                    std::shared_ptr<Value<Vector3f>> p(new Value<Vector3f>(vec));
                    ps_out->insert({cname, p});
                }
                break;
            case param_type_e::VEC3I:
                if (p_element->Attribute(cname) != NULL){

                    auto v = p_element->Attribute(cname);
                    std::stringstream ss(v);
                    int x,y,z;

                    ss >> x >> y >> z;

                    Vector3i vec = {x,y,z};
                    
                    std::shared_ptr<Value<Vector3i>> p(new Value<Vector3i>(vec));
                    ps_out->insert({cname, p});
                }
                break;
            case param_type_e::NORMAL3F:
                break;
            case param_type_e::POINT3F:
                break;
            // case param_type_e::POINT2I:
                // parse_single_COMPOSITE_attrib<int, Point2i, int(2)>( p_element, ps_out, name );
                // break;
            case param_type_e::COLOR:
                if (p_element->Attribute(cname) != NULL){

                    auto v = p_element->Attribute(cname);
                    std::stringstream ss(v);
                    float r,g,b;

                    ss >> r >> g >> b;

                    ColorXYZ color = {r,g,b};
                    
                    std::shared_ptr<Value<ColorXYZ>> p(new Value<ColorXYZ>(color));
                    ps_out->insert({cname, p});
                }
                break;
            case param_type_e::SPECTRUM:
                break;
            case param_type_e::ARR_REAL:
                if (p_element->Attribute(cname) != NULL){

                    auto v = p_element->Attribute(cname);
                    std::stringstream ss(v);
                    std::vector<float> arr;

                    float x;

                    while(ss >> x){
                        arr.push_back(x);
                    }
                    
                    std::shared_ptr<Value<std::vector<float>>> p(new Value<std::vector<float>>(arr));
                    ps_out->insert({cname, p});
                }

                break;
            case param_type_e::ARR_INT:
                if (p_element->Attribute(cname) != NULL){

                    auto v = p_element->Attribute(cname);
                    std::stringstream ss(v);
                    std::vector<int> arr;

                    int x;

                    while(ss >> x){
                        arr.push_back(x);
                    }
                    
                    std::shared_ptr<Value<std::vector<int>>> p(new Value<std::vector<int>>(arr));
                    ps_out->insert({cname, p});
                }
                break;
            case param_type_e::ARR_VEC3F:
                break;
            case param_type_e::ARR_VEC3I:
                break;
            case param_type_e::ARR_NORMAL3F:
                break;
            case param_type_e::ARR_POINT3F:
                break;
            case param_type_e::ARR_COLOR:
                break;
            default:
                RT3_WARNING( string{"parse_params(): unkonwn param type received!" } );
                break;
        }
        clog << "---Done!\n";
    }
}


//-------------------------------------------------------------------------------

} // namespace rt3
