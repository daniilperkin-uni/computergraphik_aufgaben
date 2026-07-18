#include <string>

#include "ImageViewer.h"
#include "Image.h"
#include "ImageFilter.h"
#include "ImageIO.h"


//-----------------------------------------------------------------------------
// prototypes
//-----------------------------------------------------------------------------
bool check_file(const std::string& name);
cg::filter::BorderPolicy retrieveBorderPolicy();
std::pair<int, int> retrieveExtent();
float retrieveSigma();
cg::image<cg::color_space_t::RGBA> edgeDetection2D(
    cg::image<cg::color_space_t::RGBA> original);
cg::image<cg::color_space_t::RGBA> gaussian2D(
    cg::image<cg::color_space_t::RGBA> original);
cg::image<cg::color_space_t::RGBA> seperatedGaussian2D(
    cg::image<cg::color_space_t::RGBA> original);

//-----------------------------------------------------------------------------
// implementations
//-----------------------------------------------------------------------------
int main(const int argc, const char** argv)
{
    std::cout << "Uni Stuttgart - CG Exercise 4" << std::endl;

    if (argc == 3)
    {
        // batch/ console mode
        std::string source_file(argv[1]);
        std::string target_file(argv[2]);

        std::cout <<
            "\nProgram running in console mode ..\n"
            "Make sure to request a VIS-Account if you don't have access "
            "to an OpenGL 4.3 compatible computer!!\n" << std::endl;

        std::cout << "Source file: " << source_file << std::endl;
        std::cout << "Target file: " << target_file << std::endl << std::endl;

        // Read RGB source image file
        if (!check_file(source_file))
        {
            std::cerr << "Error: could not open source file" << std::endl;
            return 1;
        }

        cg::image<cg::color_space_t::RGBA> input_image =
            cg::image_io::load_padded_rgba_image(source_file);

        cg::image<cg::color_space_t::RGBA> output_image = input_image;

        int filterOp = 0;
        std::cout << "\nSelect filter operation [1-3]:\n"
            "1 - Edge Detection\n"
            "2 - 2D Gaussian\n"
            "3 - seperated 2D Gaussian\n"
            << std::endl;
        std::cout << "Your selection: ";
        std::cin >> filterOp;

        switch (filterOp)
        {
        case 1:
            output_image = edgeDetection2D(input_image);
            break;

        case 2:
            output_image = gaussian2D(input_image);
            break;

        case 3:
            output_image = seperatedGaussian2D(input_image);
            break;

        default:
            std::cerr << "Error: invalid filter operation" << std::endl;
            return 1;
            break;
        }

        cg::image_io::save_rgba_image(target_file, output_image);
    }
    else
    {
        // GUI mode
        cg::ImageViewer image_viewer;
        std::cout << "OpenGL ImageViewer" << std::endl;
        image_viewer.run();
    }

    return 0;
}

//-----------------------------------------------------------------------------
bool check_file(const std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

//-----------------------------------------------------------------------------
cg::filter::BorderPolicy retrieveBorderPolicy()
{
    int borderPolicy = 0;
    cg::filter::BorderPolicy border = cg::filter::BorderPolicy::CLAMP_TO_EDGE;

    std::cout << "\nSelect border policy [1-3]:\n"
        "1 - CLAMP_TO_EDGE\n"
        "2 - MIRROR\n"
        "3 - REPEAT\n"
        << std::endl;
    std::cout << "Your selection: ";
    std::cin >> borderPolicy;

    switch (borderPolicy)
    {
    case 1:
        border = cg::filter::BorderPolicy::CLAMP_TO_EDGE;
        break;

    case 2:
        border = cg::filter::BorderPolicy::MIRROR;
        break;

    case 3:
        border = cg::filter::BorderPolicy::REPEAT;
        break;

    default:
        std::cerr << "Error: invalid border policy" << std::endl;
        exit(1);
        break;
    }

    return border;
}

//-----------------------------------------------------------------------------
std::pair<int, int> retrieveExtent()
{
    int x = -1, y = -1;

    std::cout << "\nInput horizontal kernel extent (>= 0): ";
    std::cin >> x;
    if (x < 0)
    {
        std::cerr << "Error: invalid horizontal kernel extent" << std::endl;
        exit(1);
    }

    std::cout << "\nInput vertical kernel extent (>= 0): ";
    std::cin >> y;
    if (y < 0)
    {
        std::cerr << "Error: invalid vertical kernel extent" << std::endl;
        exit(1);
    }


    std::pair<int, int> extent(x, y);

    return extent;
}

//-----------------------------------------------------------------------------
float retrieveSigma()
{
    float sigma = -1.f;

    std::cout << "\nInput standard deviation sigma (> 0.f): ";
    std::cin >> sigma;
    if (sigma <= 0.f)
    {
        std::cerr << "Error: invalid standard deviation sigma" << std::endl;
        exit(1);
    }

    return sigma;
}

//-----------------------------------------------------------------------------
cg::image<cg::color_space_t::RGBA> edgeDetection2D(
    cg::image<cg::color_space_t::RGBA> original)
{
    cg::filter::BorderPolicy border = retrieveBorderPolicy();

    cg::image<cg::color_space_t::RGBA> filtered =
        cg::filter::edgeDetection2D(original, border);

    return filtered;
}

//-----------------------------------------------------------------------------
cg::image<cg::color_space_t::RGBA> gaussian2D(
    cg::image<cg::color_space_t::RGBA> original)
{
    cg::filter::BorderPolicy border = retrieveBorderPolicy();
    std::pair<int, int> extent = retrieveExtent();
    float sigma = retrieveSigma();

    cg::image<cg::color_space_t::RGBA> filtered =
        cg::filter::gaussian2D(original, extent, sigma, border);

    return filtered;
}

//-----------------------------------------------------------------------------
cg::image<cg::color_space_t::RGBA> seperatedGaussian2D(
    cg::image<cg::color_space_t::RGBA> original)
{
    cg::filter::BorderPolicy border = retrieveBorderPolicy();
    std::pair<int, int> extent = retrieveExtent();
    float sigma = retrieveSigma();

    cg::image<cg::color_space_t::RGBA> filtered =
        cg::filter::seperatedGaussian2D(original, extent, sigma, border);

    return filtered;
}

