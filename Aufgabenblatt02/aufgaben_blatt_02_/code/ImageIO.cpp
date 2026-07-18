#include "ImageIO.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace cg
{
	namespace image_io
	{
		namespace
		{
			/// <summary>
			/// Struct for storing header data
			/// </summary>
			struct header
			{
				enum file_t
				{
					INVALID, PLAIN_PBM, PLAIN_PGM, PLAIN_PPM, PBM, PGM, PPM
				}
				file_type;

				unsigned int width;
				unsigned int height;
				unsigned int max_value;
			};

			/// <summary>
			/// Read file header
			/// </summary>
			/// <param name="stream">Input stream</param>
			/// <returns>File header</returns>
			header load_header(std::ifstream& stream);

			/// <summary>
			/// Write file header
			/// </summary>
			/// <param name="stream">Output stream</param>
			/// <param name="file_header">File header</param>
			void save_header(std::ofstream& stream, const header& file_header);

			/// <summary>
			/// Load plain PBM image
			/// </summary>
			/// <param name="stream">Input stream</param>
			/// <param name="header">File header</param>
			/// <returns>Load black-and-white image</returns>
			image<color_space_t::BW> load_plain_pbm(std::ifstream& stream, const header& header);

			/// <summary>
			/// Load plain PGM image
			/// </summary>
			/// <param name="stream">Input stream</param>
			/// <param name="header">File header</param>
			/// <returns>Grayscale image</returns>
			image<color_space_t::Gray> load_plain_pgm(std::ifstream& stream, const header& header);

			/// <summary>
			/// Load plain PPM image
			/// </summary>
			/// <param name="stream">Input stream</param>
			/// <param name="header">File header</param>
			/// <returns>RGB image</returns>
			image<color_space_t::RGB> load_plain_ppm(std::ifstream& stream, const header& header);

			/// <summary>
			/// Load PBM image
			/// </summary>
			/// <param name="stream">Input stream</param>
			/// <param name="header">File header</param>
			/// <returns>Load black-and-white image</returns>
			image<color_space_t::BW> load_pbm(std::ifstream& stream, const header& header);

			/// <summary>
			/// Load PGM image
			/// </summary>
			/// <param name="stream">Input stream</param>
			/// <param name="header">File header</param>
			/// <returns>Grayscale image</returns>
			image<color_space_t::Gray> load_pgm(std::ifstream& stream, const header& header);

			/// <summary>
			/// Load PPM image
			/// </summary>
			/// <param name="stream">Input stream</param>
			/// <param name="header">File header</param>
			/// <returns>RGB image</returns>
			image<color_space_t::RGB> load_ppm(std::ifstream& stream, const header& header);

			/// <summary>
			/// Save plain PBM image
			/// </summary>
			/// <param name="stream">Output stream</param>
			/// <param name="image">Load black-and-white image</param>
			void save_plain_pbm(std::ofstream& stream, const image<color_space_t::BW>& image);

			/// <summary>
			/// Save plain PGM image
			/// </summary>
			/// <param name="stream">Output stream</param>
			/// <param name="image">Grayscale image</param>
			void save_plain_pgm(std::ofstream& stream, const image<color_space_t::Gray>& image);

			/// <summary>
			/// Save plain PPM image
			/// </summary>
			/// <param name="stream">Output stream</param>
			/// <param name="image">RGB image</param>
			void save_plain_ppm(std::ofstream& stream, const image<color_space_t::RGB>& image);

			/// <summary>
			/// Save PBM image
			/// </summary>
			/// <param name="stream">Output stream</param>
			/// <param name="image">Load black-and-white image</param>
			/// <param name="max_value">Maximum value</param>
			void save_pbm(std::ofstream& stream, const image<color_space_t::BW>& image);

			/// <summary>
			/// Save PGM image
			/// </summary>
			/// <param name="stream">Output stream</param>
			/// <param name="image">Grayscale image</param>
			/// <param name="max_value">Maximum value</param>
			void save_pgm(std::ofstream& stream, const image<color_space_t::Gray>& image, unsigned int max_value = 255);

			/// <summary>
			/// Save PPM image
			/// </summary>
			/// <param name="stream">Output stream</param>
			/// <param name="image">RGB image</param>
			/// <param name="max_value">Maximum value</param>
			void save_ppm(std::ofstream& stream, const image<color_space_t::RGB>& image, unsigned int max_value = 255);

			/// <summary>
			/// Read a value
			/// </summary>
			/// <param name="stream">Input stream</param>
			/// <returns>Value</returns>
			unsigned int read_value(std::ifstream& stream);

			cg::image_io::header load_header(std::ifstream& stream)
			{
				header file_header;

				// Read magic number
				std::array<char, 2> magic_number;
				stream.read(magic_number.data(), magic_number.size());

				if (magic_number[0] == 'P')
				{
					if (magic_number[1] == '1')
					{
						file_header.file_type = header::file_t::PLAIN_PBM;
					}
					else if (magic_number[1] == '2')
					{
						file_header.file_type = header::file_t::PLAIN_PGM;
					}
					else if (magic_number[1] == '3')
					{
						file_header.file_type = header::file_t::PLAIN_PPM;
					}
					else if (magic_number[1] == '4')
					{
						file_header.file_type = header::file_t::PBM;
					}
					else if (magic_number[1] == '5')
					{
						file_header.file_type = header::file_t::PGM;
					}
					else if (magic_number[1] == '6')
					{
						file_header.file_type = header::file_t::PPM;
					}
					else
					{
						file_header.file_type = header::file_t::INVALID;
					}
				}
				else
				{
					file_header.file_type = header::file_t::INVALID;
				}

				if (file_header.file_type == header::file_t::INVALID)
				{
					throw std::runtime_error("Invalid file format");
				}

				// Read extents (width, height, max. value)
				file_header.width = read_value(stream);
				file_header.height = read_value(stream);

				if (file_header.file_type == header::file_t::PGM || file_header.file_type == header::file_t::PPM)
				{
					file_header.max_value = read_value(stream);
				}

				return file_header;
			}

			void save_header(std::ofstream& stream, const cg::image_io::header& file_header)
			{
				// Save magic number
				switch (file_header.file_type)
				{
				case header::file_t::PLAIN_PBM:
					stream << "P1" << std::endl;

					break;
				case header::file_t::PLAIN_PGM:
					stream << "P2" << std::endl;

					break;
				case header::file_t::PLAIN_PPM:
					stream << "P3" << std::endl;

					break;
				case header::file_t::PBM:
					stream << "P4" << std::endl;

					break;
				case header::file_t::PGM:
					stream << "P5" << std::endl;

					break;
				case header::file_t::PPM:
					stream << "P6" << std::endl;

					break;
				default:
					throw std::runtime_error("Invalid header");
				}

				// Save extents
				stream << file_header.width << " " << file_header.height << "\n";

				if (file_header.file_type == header::file_t::PGM || file_header.file_type == header::file_t::PPM)
				{
					stream << file_header.max_value << std::endl;
				}
			}

			cg::image<cg::color_space_t::BW> load_plain_pbm(std::ifstream& stream, const header& header)
			{
				// Create image
				cg::image<cg::color_space_t::BW> image(header.width, header.height);

				std::size_t index = 0;

				for (unsigned int j = 0; j < header.height; ++j)
				{
					for (unsigned int i = 0; i < header.width; ++i)
					{
						image(i, j)[0] = (read_value(stream) != 1) ? 1.0f : 0.0f;
					}
				}

				return image;
			}

			cg::image<cg::color_space_t::Gray> load_plain_pgm(std::ifstream& stream, const header& header)
			{
				// Create image
				cg::image<cg::color_space_t::Gray> image(header.width, header.height);

				std::size_t index = 0;

				for (unsigned int j = 0; j < header.height; ++j)
				{
					for (unsigned int i = 0; i < header.width; ++i)
					{
						image(i, j)[0] = static_cast<float>(read_value(stream)) / static_cast<float>(header.max_value);
					}
				}

				return image;
			}

			cg::image<cg::color_space_t::RGB> load_plain_ppm(std::ifstream& stream, const header& header)
			{
				// Create image
				cg::image<cg::color_space_t::RGB> image(header.width, header.height);

				std::size_t index = 0;

				for (unsigned int j = 0; j < header.height; ++j)
				{
					for (unsigned int i = 0; i < header.width; ++i)
					{
						image(i, j)[0] = static_cast<float>(read_value(stream)) / static_cast<float>(header.max_value);
						image(i, j)[1] = static_cast<float>(read_value(stream)) / static_cast<float>(header.max_value);
						image(i, j)[2] = static_cast<float>(read_value(stream)) / static_cast<float>(header.max_value);
					}
				}

				return image;
			}

			cg::image<cg::color_space_t::BW> load_pbm(std::ifstream& stream, const header& header)
			{
				// Read image
				std::vector<char> buffer((header.width + header.width % 8) * header.height / 8);
				const auto* cbuffer = reinterpret_cast<unsigned char*>(buffer.data());

				stream.read(buffer.data(), buffer.size());

				// Create image
				cg::image<cg::color_space_t::BW> image(header.width, header.height);

				std::size_t index = buffer.size() - 1;

				for (int j = header.height - 1; j >= 0; --j)
				{
					for (int i = (header.width + header.width % 8) - 1; i >= 0; i -= 8)
					{
						image((i - 0) % header.width, j)[0] = ((cbuffer[index] & 1) != 0) ? 0.0f : 1.0f;
						image((i - 1) % header.width, j)[0] = ((cbuffer[index] & 2) != 0) ? 0.0f : 1.0f;
						image((i - 2) % header.width, j)[0] = ((cbuffer[index] & 4) != 0) ? 0.0f : 1.0f;
						image((i - 3) % header.width, j)[0] = ((cbuffer[index] & 8) != 0) ? 0.0f : 1.0f;
						image((i - 4) % header.width, j)[0] = ((cbuffer[index] & 16) != 0) ? 0.0f : 1.0f;
						image((i - 5) % header.width, j)[0] = ((cbuffer[index] & 32) != 0) ? 0.0f : 1.0f;
						image((i - 6) % header.width, j)[0] = ((cbuffer[index] & 64) != 0) ? 0.0f : 1.0f;
						image((i - 7) % header.width, j)[0] = ((cbuffer[index] & 128) != 0) ? 0.0f : 1.0f;

						--index;
					}
				}

				return image;
			}

			cg::image<cg::color_space_t::Gray> load_pgm(std::ifstream& stream, const header& header)
			{
				// Read image
				std::vector<char> buffer(header.width * header.height * ((header.max_value >= 256) ? 2 : 1));
				const auto* cbuffer = reinterpret_cast<unsigned char*>(buffer.data());
				const auto* wbuffer = reinterpret_cast<char16_t*>(buffer.data());

				stream.read(buffer.data(), buffer.size());

				// Create image
				cg::image<cg::color_space_t::Gray> image(header.width, header.height);

				std::size_t index = 0;

				for (unsigned int j = 0; j < header.height; ++j)
				{
					for (unsigned int i = 0; i < header.width; ++i)
					{
						if (header.max_value < 256)
						{
							image(i, j)[0] = static_cast<float>(cbuffer[index++]) / static_cast<float>(header.max_value);
						}
						else
						{
							image(i, j)[0] = static_cast<float>(wbuffer[index++]) / static_cast<float>(header.max_value);
						}
					}
				}

				return image;
			}

			cg::image<cg::color_space_t::RGB> load_ppm(std::ifstream& stream, const header& header)
			{
				// Read image
				std::vector<char> buffer(3 * header.width * header.height * ((header.max_value >= 256) ? 2 : 1));
				const auto* cbuffer = reinterpret_cast<unsigned char*>(buffer.data());
				const auto* wbuffer = reinterpret_cast<char16_t*>(buffer.data());

				stream.read(buffer.data(), buffer.size());

				// Create image
				cg::image<cg::color_space_t::RGB> image(header.width, header.height);

				std::size_t index = 0;

				for (unsigned int j = 0; j < header.height; ++j)
				{
					for (unsigned int i = 0; i < header.width; ++i)
					{
						if (header.max_value < 256)
						{
							image(i, j)[0] = static_cast<float>(cbuffer[index++]) / static_cast<float>(header.max_value);
							image(i, j)[1] = static_cast<float>(cbuffer[index++]) / static_cast<float>(header.max_value);
							image(i, j)[2] = static_cast<float>(cbuffer[index++]) / static_cast<float>(header.max_value);
						}
						else
						{
							image(i, j)[0] = static_cast<float>(wbuffer[index++]) / static_cast<float>(header.max_value);
							image(i, j)[1] = static_cast<float>(wbuffer[index++]) / static_cast<float>(header.max_value);
							image(i, j)[2] = static_cast<float>(wbuffer[index++]) / static_cast<float>(header.max_value);
						}
					}
				}

				return image;
			}

			void save_plain_pbm(std::ofstream& stream, const cg::image<cg::color_space_t::BW>& image)
			{
				for (unsigned int j = 0; j < image.get_height(); ++j)
				{
					for (unsigned int i = 0; i < image.get_width() - 1; ++i)
					{
						stream << ((image(i, j)[0] != 0.0f) ? 0 : 1) << " ";
					}

					stream << ((image(image.get_width() - 1, j)[0] != 0.0f) ? 0 : 1) << std::endl;
				}
			}

			void save_plain_pgm(std::ofstream& stream, const cg::image<cg::color_space_t::Gray>& image)
			{
				for (unsigned int j = 0; j < image.get_height(); ++j)
				{
					for (unsigned int i = 0; i < image.get_width() - 1; ++i)
					{
						stream << static_cast<unsigned int>(image(i, j)[0] * 255.0f) << " ";
					}

					stream << static_cast<unsigned int>(image(image.get_width() - 1, j)[0] * 255.0f) << std::endl;
				}
			}

			void save_plain_ppm(std::ofstream& stream, const cg::image<cg::color_space_t::RGB>& image)
			{
				for (unsigned int j = 0; j < image.get_height(); ++j)
				{
					for (unsigned int i = 0; i < image.get_width() - 1; ++i)
					{
						stream << static_cast<unsigned int>(image(i, j)[0] * 255.0f) << " ";
						stream << static_cast<unsigned int>(image(i, j)[1] * 255.0f) << " ";
						stream << static_cast<unsigned int>(image(i, j)[2] * 255.0f) << "\t";
					}

					stream << static_cast<unsigned int>(image(image.get_width() - 1, j)[0] * 255.0f) << " ";
					stream << static_cast<unsigned int>(image(image.get_width() - 1, j)[1] * 255.0f) << " ";
					stream << static_cast<unsigned int>(image(image.get_width() - 1, j)[2] * 255.0f) << std::endl;
				}
			}

			void save_pbm(std::ofstream& stream, const cg::image<cg::color_space_t::BW>& image)
			{
				// Create buffer
				std::vector<char> buffer((image.get_width() + image.get_width() % 8) * image.get_height() / 8);
				auto* cbuffer = reinterpret_cast<unsigned char*>(buffer.data());

				std::size_t index = 0;

				for (unsigned int j = 0; j < image.get_height(); ++j)
				{
					for (unsigned int i = 0; i < image.get_width(); i += 8)
					{
						cbuffer[index] = static_cast<unsigned char>(0);

						cbuffer[index] |= (image((i + 0) % image.get_width(), j)[0] != 0.0f) ? 0 : 128;
						cbuffer[index] |= (image((i + 1) % image.get_width(), j)[0] != 0.0f) ? 0 : 64;
						cbuffer[index] |= (image((i + 2) % image.get_width(), j)[0] != 0.0f) ? 0 : 32;
						cbuffer[index] |= (image((i + 3) % image.get_width(), j)[0] != 0.0f) ? 0 : 16;
						cbuffer[index] |= (image((i + 4) % image.get_width(), j)[0] != 0.0f) ? 0 : 8;
						cbuffer[index] |= (image((i + 5) % image.get_width(), j)[0] != 0.0f) ? 0 : 4;
						cbuffer[index] |= (image((i + 6) % image.get_width(), j)[0] != 0.0f) ? 0 : 2;
						cbuffer[index] |= (image((i + 7) % image.get_width(), j)[0] != 0.0f) ? 0 : 1;

						++index;
					}
				}

				// Save to file
				stream.write(buffer.data(), buffer.size());
			}

			void save_pgm(std::ofstream& stream, const cg::image<cg::color_space_t::Gray>& image, const unsigned int max_value)
			{
				// Create buffer
				std::vector<char> buffer(image.get_width() * image.get_height() * ((max_value >= 256) ? 2 : 1));
				auto* cbuffer = reinterpret_cast<unsigned char*>(buffer.data());
				auto* wbuffer = reinterpret_cast<char16_t*>(buffer.data());

				std::size_t index = 0;

				for (unsigned int j = 0; j < image.get_height(); ++j)
				{
					for (unsigned int i = 0; i < image.get_width(); ++i)
					{
						if (max_value < 256)
						{
							cbuffer[index++] = static_cast<unsigned char>(image(i, j)[0] * max_value);
						}
						else
						{
							wbuffer[index++] = static_cast<char16_t>(image(i, j)[0] * max_value);
						}
					}
				}

				// Save to file
				stream.write(buffer.data(), buffer.size());
			}

			void save_ppm(std::ofstream& stream, const cg::image<cg::color_space_t::RGB>& image, const unsigned int max_value)
			{
				// Create buffer
				std::vector<char> buffer(3 * image.get_width() * image.get_height() * ((max_value >= 256) ? 2 : 1));
				auto* cbuffer = reinterpret_cast<unsigned char*>(buffer.data());
				auto* wbuffer = reinterpret_cast<char16_t*>(buffer.data());

				std::size_t index = 0;

				for (unsigned int j = 0; j < image.get_height(); ++j)
				{
					for (unsigned int i = 0; i < image.get_width(); ++i)
					{
						if (max_value < 256)
						{
							cbuffer[index++] = static_cast<unsigned char>(image(i, j)[0] * max_value);
							cbuffer[index++] = static_cast<unsigned char>(image(i, j)[1] * max_value);
							cbuffer[index++] = static_cast<unsigned char>(image(i, j)[2] * max_value);
						}
						else
						{
							wbuffer[index++] = static_cast<char16_t>(image(i, j)[0] * max_value);
							wbuffer[index++] = static_cast<char16_t>(image(i, j)[1] * max_value);
							wbuffer[index++] = static_cast<char16_t>(image(i, j)[2] * max_value);
						}
					}
				}

				// Save to file
				stream.write(buffer.data(), buffer.size());
			}

			unsigned int read_value(std::ifstream& stream)
			{
				std::vector<char> buffer;
				char next_char;

				enum state_t
				{
					READY,		// Ready to read in the value
					COMMENT,	// Inside a comment
					READING,	// Currently reading (part of) the value
					FINISHED	// Finished reading the value
				} state = READY;

				while (state != FINISHED)
				{
					stream.read(&next_char, 1);

					switch (state)
					{
					case READY:
						if (next_char == '#')
						{
							state = COMMENT;
						}
						else if (next_char != '\n' && next_char != '\r' && next_char != '\t' && next_char != ' ')
						{
							state = READING;
							buffer.push_back(next_char);
						}

						break;
					case COMMENT:
						if (next_char == '\n' || next_char == '\r')
						{
							state = READY;
						}

						break;
					case READING:
						if (next_char != '\n' && next_char != '\r' && next_char != '\t' && next_char != ' ')
						{
							buffer.push_back(next_char);
						}
						else
						{
							state = FINISHED;
						}
					}
				}

				std::string buffer_string(buffer.begin(), buffer.end());
				return static_cast<unsigned int>(std::stoul(buffer_string));
			}
		}
	}
}

std::shared_ptr<cg::image_base> cg::image_io::load_image(const std::string& path)
{
	std::ifstream image_file(path, std::iostream::in | std::iostream::binary);

	if (image_file.is_open() && image_file.good())
	{
		auto header = load_header(image_file);
		image_file.close();

		switch (header.file_type)
		{
		case cg::image_io::header::PLAIN_PBM:
		case cg::image_io::header::PBM:
			return std::make_shared<cg::image<cg::color_space_t::BW>>(load_bw_image(path));

			break;
		case cg::image_io::header::PLAIN_PGM:
		case cg::image_io::header::PGM:
			return std::make_shared<cg::image<cg::color_space_t::Gray>>(load_grayscale_image(path));

			break;
		case cg::image_io::header::PLAIN_PPM:
		case cg::image_io::header::PPM:
			return std::make_shared<cg::image<cg::color_space_t::RGB>>(load_rgb_image(path));
		}

		throw std::runtime_error("Unknown image file format");
	}

	throw std::runtime_error("Unable to open file");
}

void cg::image_io::save_image(const std::string& path, const std::shared_ptr<cg::image_base>& image, bool double_prec, const bool plain)
{
	const auto* bw_image = dynamic_cast<cg::image<cg::color_space_t::BW>*>(image.get());
	const auto* gray_image = dynamic_cast<cg::image<cg::color_space_t::Gray>*>(image.get());
	const auto* rgb_image = dynamic_cast<cg::image<cg::color_space_t::RGB>*>(image.get());

	if (bw_image != nullptr)
	{
		save_bw_image(path, *bw_image, plain);
	}
	else if(gray_image != nullptr)
	{
		save_grayscale_image(path, *gray_image, double_prec, plain);
	}
	else if (rgb_image != nullptr)
	{
		save_rgb_image(path, *rgb_image, double_prec, plain);
	}
}

cg::image<cg::color_space_t::BW> cg::image_io::load_bw_image(const std::string& path)
{
	std::ifstream image_file(path, std::iostream::in | std::iostream::binary);

	if (image_file.is_open() && image_file.good())
	{
		auto header = load_header(image_file);

		if (header.file_type == cg::image_io::header::PBM)
		{
			return load_pbm(image_file, header);
		}
		else if (header.file_type == cg::image_io::header::PLAIN_PBM)
		{
			return load_plain_pbm(image_file, header);
		}

		throw std::runtime_error("Black-and-white images can only be loaded from PBM files");
	}

	throw std::runtime_error("Unable to open file");
}

cg::image<cg::color_space_t::Gray> cg::image_io::load_grayscale_image(const std::string& path)
{
	std::ifstream image_file(path, std::iostream::in | std::iostream::binary);

	if (image_file.is_open() && image_file.good())
	{
		auto header = load_header(image_file);

		if (header.file_type == cg::image_io::header::PGM)
		{
			return load_pgm(image_file, header);
		}
		else if (header.file_type == cg::image_io::header::PLAIN_PGM)
		{
			return load_plain_pgm(image_file, header);
		}

		throw std::runtime_error("Grayscale images can only be loaded from PGM files");
	}

	throw std::runtime_error("Unable to open file");
}

cg::image<cg::color_space_t::RGB> cg::image_io::load_rgb_image(const std::string& path)
{
	std::ifstream image_file(path, std::iostream::in | std::iostream::binary);

	if (image_file.is_open() && image_file.good())
	{
		auto header = load_header(image_file);

		if (header.file_type == cg::image_io::header::PPM)
		{
			return load_ppm(image_file, header);
		}
		else if (header.file_type == cg::image_io::header::PLAIN_PPM)
		{
			return load_plain_ppm(image_file, header);
		}

		throw std::runtime_error("RGB images can only be loaded from PPM files");
	}

	throw std::runtime_error("Unable to open file");
}

void cg::image_io::save_bw_image(const std::string& path, const cg::image<cg::color_space_t::BW>& image, const bool plain)
{
	std::ofstream image_file(path, std::iostream::out | std::iostream::binary);

	if (image_file.is_open() && image_file.good())
	{
		cg::image_io::header header;
		header.file_type = plain ? cg::image_io::header::PLAIN_PBM : cg::image_io::header::PBM;
		header.width = image.get_width();
		header.height = image.get_height();

		save_header(image_file, header);
		plain ? save_plain_pbm(image_file, image) : save_pbm(image_file, image);
	}
	else
	{
		throw std::runtime_error("Unable to open file");
	}
}

void cg::image_io::save_grayscale_image(const std::string& path, const cg::image<cg::color_space_t::Gray>& image, bool double_prec, const bool plain)
{
	std::ofstream image_file(path, std::iostream::out | std::iostream::binary);

	if (image_file.is_open() && image_file.good())
	{
		cg::image_io::header header;
		header.file_type = plain ? cg::image_io::header::PLAIN_PGM : cg::image_io::header::PGM;
		header.width = image.get_width();
		header.height = image.get_height();
		header.max_value = double_prec ? 65535 : 255;

		save_header(image_file, header);
		plain ? save_plain_pgm(image_file, image) : save_pgm(image_file, image, header.max_value);
	}
	else
	{
		throw std::runtime_error("Unable to open file");
	}
}

void cg::image_io::save_rgb_image(const std::string& path, const cg::image<cg::color_space_t::RGB>& image, bool double_prec, const bool plain)
{
	std::ofstream image_file(path, std::iostream::out | std::iostream::binary);

	if (image_file.is_open() && image_file.good())
	{
		cg::image_io::header header;
		header.file_type = plain ? cg::image_io::header::PLAIN_PPM : cg::image_io::header::PPM;
		header.width = image.get_width();
		header.height = image.get_height();
		header.max_value = double_prec ? 65535 : 255;

		save_header(image_file, header);
		plain ? save_plain_ppm(image_file, image) : save_ppm(image_file, image, header.max_value);
	}
	else
	{
		throw std::runtime_error("Unable to open file");
	}
}
