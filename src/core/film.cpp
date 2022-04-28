#include "film.h"

namespace rt3 {

    //=== Film Method Definitions
    Film::Film( const Point2i &resolution, const std::string &filename , image_type_e imgt )
    {
        m_full_resolution = resolution;
        m_filename = filename;
        image_type = imgt;

        this->img_obj = ColorMatrix(resolution[1], resolution[0]);
    }

    Film::Film(const Film& other)
    {
        this->m_full_resolution = other.m_full_resolution;
        this->m_filename = other.m_filename;
        this->image_type = other.image_type;
        this->img_obj = other.img_obj;
    }

    void Film::operator=(const Film& other)
    {
        this->m_full_resolution = other.m_full_resolution;
        this->m_filename = other.m_filename;
        this->image_type = other.image_type;
        this->img_obj = other.img_obj;
    }

    Film::~Film()
    {
    }

    /// Add the color to image.
    void Film::add_sample( const Point2f &pixel_coord, const ColorXYZ &pixel_color )
    {
        Vector3<int> color;

        // TODO: Verificar se todas as cores estão dentro do range 0-255 ou 0-1.
        if(pixel_color.x != (int) pixel_color.x or pixel_color.y != (int) pixel_color.y or pixel_color.z != (int) pixel_color.z)
        {
            color.x = (int) (pixel_color.x * 255);
            color.y = (int) (pixel_color.y * 255);
            color.z = (int) (pixel_color.z * 255);
        }
        else
        {
            color.x = (int) pixel_color.x;
            color.y = (int) pixel_color.y;
            color.z = (int) pixel_color.z;
        }

       this->img_obj.insertColor(pixel_coord[1], pixel_coord[0], color);
    }

    unsigned char* Film::convertImage() const
    {
        int* img = this->img_obj.convertIntoArray();

        int dim = this->img_obj.height*this->img_obj.width*3;

        unsigned char * buffer = new unsigned char[dim];

        for(int i{0}; i < dim; i++)
        {
            buffer[i] = (unsigned char) img[i];
        }

        return buffer;
    }

    /// Convert image to RGB, compute final pixel values, write image.
    void Film::write_image(void) const
    {
        unsigned char * img  = convertImage();

        if(this->image_type == Film::image_type_e::PNG)
        {
            save_png( img, this->m_full_resolution[0], this->m_full_resolution[1], 3, this->m_filename + ".png");
        }
        else if(this->image_type == Film::image_type_e::PPM3)
        {
            save_ppm3( img, this->m_full_resolution[0], this->m_full_resolution[1], 3, this->m_filename + ".ppm");
        }
        else if(this->image_type == Film::image_type_e::PPM6)
        {
            save_ppm6( img, this->m_full_resolution[0], this->m_full_resolution[1], 3, this->m_filename + ".ppm" );
        }
    }
    
    // Factory function pattern.
    // This is the function that retrieves from the ParamSet object
    // all the information we need to create a Film object.
    Film * create_film( const ParamSet &ps )
    {
        std::cout << ">>> Inside create_film()\n";
        std::string filename;
        // Let us check whether user has provided an output file name via
        // command line arguments in main().
        if ( API::curr_run_opt.outfile != "")
        {
            // Retrieve filename defined via CLI.
            filename = API::curr_run_opt.outfile;
            // Retrieve another filename, now from the ParamSet object.
            std::string filename_from_file = retrieve( ps, "filename", std::string{""} );
            if ( filename_from_file != "") // We also get a filename from scene file...
                RT3_WARNING( string{"Output filename supplied on command line, \""} +
                         API::curr_run_opt.outfile +
                         string{"\" is overriding filename provided in scene description file, \""} +
                         filename_from_file );
        }
        else
        {
            // Try yo retrieve filename from scene file.
            filename = retrieve( ps, "filename", std::string{"image.png"} );
        }

        // Read resolution.
        // Aux function that retrieves info from the ParamSet.
        int xres = retrieve( ps, "x_res", int(1280) );
        // Aux function that retrieves info from the ParamSet.
        int yres = retrieve( ps, "y_res", int(720) );
        // Quick render?
        if ( API::curr_run_opt.quick_render == true)
        {
            // decrease resolution.
            xres = std::max(1, xres / 4);
            yres = std::max(1, yres / 4);
        }


        string img_type_temp = retrieve( ps, "img_type", std::string{"png"} );

        Film::image_type_e img_type;

        if(img_type_temp == "png")
        {
            img_type = Film::image_type_e::PNG;
        }
        else if(img_type_temp == "ppm3")
        {
            img_type = Film::image_type_e::PPM3;
        }
        else if(img_type_temp == "ppm6")
        {
            img_type = Film::image_type_e::PPM6;
        }

        // TODO
        // Read crop window information.

        // Note that the image type is fixed here. Must be read from ParamSet, though.
        return new Film( Point2i{ xres, yres}, filename, img_type );
    }
}  // namespace pbrt
