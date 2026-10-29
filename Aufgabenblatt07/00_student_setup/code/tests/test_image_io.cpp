// Regression tests for the Netpbm image I/O.
//
// Covers the binary PBM buffer sizing / bit layout (QA-001), the EOF handling
// of the header tokenizer (QA-003) and the PBM polarity required by the format
// specification (a set bit is a black pixel).
#include "ImageIO.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
    int failures = 0;

    void check(const bool condition, const std::string& what)
    {
        if (!condition)
        {
            std::cerr << "FAIL: " << what << '\n';
            ++failures;
        }
    }

    cg::image<cg::color_space_t::BW> make_pattern(const unsigned int width, const unsigned int height)
    {
        cg::image<cg::color_space_t::BW> image(width, height);

        for (unsigned int j = 0; j < height; ++j)
        {
            for (unsigned int i = 0; i < width; ++i)
            {
                image(i, j)[0] = ((i * 7 + j * 3) % 5 == 0) ? 0.0f : 1.0f;
            }
        }

        return image;
    }

    void write_file(const std::string& path, const std::string& content)
    {
        std::ofstream file(path, std::ios::out | std::ios::binary);
        file.write(content.data(), static_cast<std::streamsize>(content.size()));
    }

    void test_round_trip(const unsigned int width, const unsigned int height)
    {
        const std::string path =
            "pbm_round_trip_" + std::to_string(width) + "x" + std::to_string(height) + ".pbm";

        const auto original = make_pattern(width, height);
        cg::image_io::save_bw_image(path, original);

        const auto loaded = cg::image_io::load_bw_image(path);

        check(loaded.get_width() == width, "round trip width of " + path);
        check(loaded.get_height() == height, "round trip height of " + path);

        for (unsigned int j = 0; j < height; ++j)
        {
            for (unsigned int i = 0; i < width; ++i)
            {
                check(loaded(i, j)[0] == original(i, j)[0],
                      "round trip pixel " + std::to_string(i) + "," + std::to_string(j) + " of " + path);
            }
        }

        std::remove(path.c_str());
    }

    void test_pbm_bit_order_and_polarity()
    {
        // P4, 2x1: the most significant bit of the first byte is the left-most
        // pixel, a set bit is black.
        const std::string path = "pbm_spec.pbm";
        write_file(path, std::string("P4\n2 1\n", 7) + static_cast<char>(0x80));

        const auto loaded = cg::image_io::load_bw_image(path);

        check(loaded.get_width() == 2 && loaded.get_height() == 1, "spec image size");
        check(loaded(0, 0)[0] == 0.0f, "left pixel is black");
        check(loaded(1, 0)[0] == 1.0f, "right pixel is white");

        std::remove(path.c_str());
    }

    void test_truncated_header_throws()
    {
        const std::string path = "truncated.pgm";
        write_file(path, "P5\n2 ");

        bool threw = false;

        try
        {
            cg::image_io::load_grayscale_image(path);
        }
        catch (const std::runtime_error&)
        {
            threw = true;
        }

        check(threw, "a truncated header throws instead of hanging");
        std::remove(path.c_str());
    }

    void test_plain_pgm_without_trailing_whitespace()
    {
        const std::string path = "plain_no_trailing_newline.pgm";
        write_file(path, "P2\n2 1\n255\n0 255");

        const auto loaded = cg::image_io::load_grayscale_image(path);

        check(loaded.get_width() == 2 && loaded.get_height() == 1, "plain PGM size");
        check(loaded(0, 0)[0] == 0.0f, "plain PGM first value");
        check(loaded(1, 0)[0] == 1.0f, "plain PGM last value at end of file");

        std::remove(path.c_str());
    }
}

int main()
{
    try
    {
        test_round_trip(1, 1);
        test_round_trip(8, 2);
        test_round_trip(9, 1);
        test_round_trip(17, 3);
        test_pbm_bit_order_and_polarity();
        test_truncated_header_throws();
        test_plain_pgm_without_trailing_whitespace();
    }
    catch (const std::exception& exception)
    {
        std::cerr << "UNEXPECTED EXCEPTION: " << exception.what() << '\n';
        return 1;
    }

    if (failures != 0)
    {
        std::cerr << failures << " check(s) failed\n";
        return 1;
    }

    std::cout << "All image I/O checks passed\n";
    return 0;
}
