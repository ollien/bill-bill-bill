#include <CImg.h>

#include <cxxopts.hpp>
#include <iostream>

#include "src/ImageProcessor.h"

int main(int argc, char *argv[]) {
	bool should_display = false;
	std::optional<std::string> output_location;
	std::string in_file;
	std::string base_string;
	std::string meme_string;

	cxxopts::Options options("bill-bill-bill", "Morph an image into a substr-ed repeated variant.");
	// clang-format off
	options.add_options()
		("d,display", "Display the image", cxxopts::value(should_display))
		("o,output", "The output filename for the image", cxxopts::value<std::optional<std::string>>(output_location), "filename")
		("in_file", "The input image", cxxopts::value<std::string>(in_file))
		("base_string", "The string to morph from", cxxopts::value<std::string>(base_string))
		("meme_string", "The string to morph to", cxxopts::value<std::string>(meme_string))
		;
	//clang-format on

	options.positional_help("<in_file> <base_string> <meme_string>");
	options.parse_positional({"in_file", "base_string", "meme_string"});
	try {
		options.parse(argc, argv);
	} catch (const cxxopts::OptionParseException &e) {
		std::cerr << e.what() << std::endl << std::endl;
		std::cerr << options.help() << std::endl;
		return 1;
	}

	cimg_library::CImg<unsigned char> image;
	try {
		image = cimg_library::CImg<unsigned char>(in_file.c_str());
	} catch (const cimg_library::CImgIOException &e) {
		std::cerr << "Failed to open image: " << e.what() << std::endl << std::endl;
		return 2;
	}

	ImageProcessor<unsigned char> processor(image, base_string.data());
	auto res = processor.morph_image(meme_string.data());
	if (should_display) {
		res.display();
	}

	if (output_location.has_value()) {
		try {
			res.save(output_location->c_str());
		} catch (const cimg_library::CImgIOException &e) {
			std::cerr << "Failed to write image: " << e.what() << std::endl;
			return 3;
		}
	}
}
