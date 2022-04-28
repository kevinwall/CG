#include "background.h"

namespace rt3 {

    float spectrum_channel(const Spectrum & spectrum, int channel){
        if (channel == 0)
            return spectrum.x;
        else if (channel == 1)
            return spectrum.y;
        else if (channel == 2)
            return spectrum.z;
    }

    bool has_decimal(float value){
        return ((int) value) != value;
    }

    Spectrum normalize_color(const Spectrum & spectrum){
        Spectrum new_spectrum{0.0,0.0,0.0};



        if (!has_decimal(spectrum.x))
            new_spectrum.x = spectrum.x/255.0;
        else
            new_spectrum.x = spectrum.x;
        if (!has_decimal(spectrum.y))
            new_spectrum.y = spectrum.y/255.0;
        else
            new_spectrum.y = spectrum.y;
        if (!has_decimal(spectrum.z))
            new_spectrum.z = spectrum.z/255.0;
        else
            new_spectrum.z = spectrum.z;

        return new_spectrum;
    }

    Spectrum Background::sampleXYZ( const Point2f &pixel_ndc ) const
    {
        return Spectrum{0,0,0};
    }

    /*!
     * Samples a color base on spherical interpolation of an image ou colored background.
     *
     * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
     * \return The interpolated color.
     */
    Spectrum BackgroundColor::sampleXYZ( const Point2f &pixel_ndc ) const
    {

        Spectrum out{0,0,0};

        if (using_color){
            //std::cout << color << " COR NORMALIZADA\n";
            out.x = (int) (color.x*255);
            out.y = (int) (color.y*255);
            out.z = (int) (color.z*255);
            //std::cout << out << " COR 255\n";
            return out;
        }

        //Spectrum out{0,0,0};

        for (int i = 0;i<3; i++){
            auto b = Lerp(pixel_ndc.x, 
                          spectrum_channel(corners[Corners_e::bl], i), 
                          spectrum_channel(corners[Corners_e::br], i));
            auto t = Lerp(pixel_ndc.x, 
                          spectrum_channel(corners[Corners_e::tl], i), 
                          spectrum_channel(corners[Corners_e::tr], i));

            auto v = Lerp(pixel_ndc.y, t,b);

            out[i] = (int) (v*255);
        }
        return out;
    }

    BackgroundColor * create_color_background( const ParamSet &ps )
    {
        auto result = ps.find( "color" );
        if ( result != ps.end() ){
            std::cout << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 1\n";

            auto x = retrieve(ps,"color", Spectrum{0,0,0});
            std::cout << x << std::endl;

            auto color = normalize_color(retrieve(ps,"color", Spectrum{0,0,0}));
            return new BackgroundColor(color);
        } else {
            std::cout << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 2\n";
            auto bl = normalize_color(retrieve(ps,"bl",Spectrum{0,0,0}));
            auto tl = normalize_color(retrieve(ps,"tl",Spectrum{0,0,0}));
            auto tr = normalize_color(retrieve(ps,"tr",Spectrum{0,0,0}));
            auto br = normalize_color(retrieve(ps,"br",Spectrum{0,0,0}));
            return new BackgroundColor(bl,tl,tr,br);
        }

    }

}
