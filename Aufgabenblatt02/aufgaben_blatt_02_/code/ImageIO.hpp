#pragma once

#include "Image.hpp"

#include <fstream>
#include <memory>
#include <string>

namespace cg
{
	/// <summary>
	/// Namespace for loading and saving images to/from file
	/// This uses the simple Netpbm image formats PBM, PGM and PPM
	/// PBM: Netpbm bi-level image format (http://netpbm.sourceforge.net/doc/pbm.html)
	/// PGM: Netpbm grayscale image format (http://netpbm.sourceforge.net/doc/pgm.html)
	/// PPM: Netpbm color image format (http://netpbm.sourceforge.net/doc/ppm.html)
	/// </summary>
	namespace image_io
	{
		/// <summary>
		/// Load an image from file
		/// </summary>
		/// <param name="path">Path to image file</param>
		/// <returns>Image</returns>
		std::shared_ptr<image_base> load_image(const std::string& path);

		/// <summary>
		/// Save an image to file
		/// </summary>
		/// <param name="path">Path to image file</param>
		/// <param name="image">Image</param>
		/// <param name="double_prec">65536 colors instead of 256</param>
		/// <param name="plain">Plain or binary</param>
		void save_image(const std::string& path, const std::shared_ptr<image_base>& image, bool double_prec = false, bool plain = false);

		/// <summary>
		/// Load black and white image from file
		/// </summary>
		/// <param name="path">Path to image file</param>
		/// <returns>Black and white image</returns>
		image<color_space_t::BW> load_bw_image(const std::string& path);

		/// <summary>
		/// Load grayscale image from file
		/// </summary>
		/// <param name="path">Path to image file</param>
		/// <returns>Grayscale image</returns>
		image<color_space_t::Gray> load_grayscale_image(const std::string& path);

		/// <summary>
		/// Load RGB image from file
		/// </summary>
		/// <param name="path">Path to image file</param>
		/// <returns>RGB image</returns>
		image<color_space_t::RGB> load_rgb_image(const std::string& path);

		/// <summary>
		/// Save black and white image to file
		/// </summary>
		/// <param name="path">Path to image file</param>
		/// <param name="image">Black and white image</param>
		/// <param name="double_prec">65536 colors instead of 256</param>
		/// <param name="plain">Plain or binary</param>
		void save_bw_image(const std::string& path, const image<color_space_t::BW>& image, bool plain = false);

		/// <summary>
		/// Save grayscale image to file
		/// </summary>
		/// <param name="path">Path to image file</param>
		/// <param name="image">Grayscale image</param>
		/// <param name="double_prec">65536 colors instead of 256</param>
		/// <param name="plain">Plain or binary</param>
		void save_grayscale_image(const std::string& path, const image<color_space_t::Gray>& image, bool double_prec = false, bool plain = false);

		/// <summary>
		/// Save RGB image to file
		/// </summary>
		/// <param name="path">Path to image file</param>
		/// <param name="image">RGB image</param>
		/// <param name="double_prec">65536 colors instead of 256</param>
		/// <param name="plain">Plain or binary</param>
		void save_rgb_image(const std::string& path, const image<color_space_t::RGB>& image, bool double_prec = false, bool plain = false);
	}
}