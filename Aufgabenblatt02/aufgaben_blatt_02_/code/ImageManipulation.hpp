#pragma once

#include "Image.hpp"

namespace cg
{
	/// <summary>
	/// Class for modifying/ manipulating images
	/// </summary>
	class image_manipulation
	{
	public:
		/// <summary>
		/// Creates a Color-Key-Effect image from the lena example
		/// </summary>
		/// <param name="original">Original image</param>
		/// <returns>Modified image</returns>
		static image<color_space_t::HSV> modify_in_hsv(const image<color_space_t::HSV>& original);
	};
}
