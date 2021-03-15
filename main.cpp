#include <CImg.h>

#include <cxxopts.hpp>
#include <iostream>

#include "src/ImageProcessor.h"

/**
 * Morph the image into its susbtring-ified form
 * @tparam T The type of pixels stored in the image
 * @param input_image The image to morph
 * @param base_string The string to morph based on
 * @param meme_string The string whose components will be used to generate the final image
 * @return cimg_library::CImg<T> The morphed image
 */
template <typename T>
static cimg_library::CImg<T> morph_image(
	cimg_library::CImg<T> input_image, const std::string &base_string, const std::string &meme_string) {
	ImageProcessor<unsigned char> processor(input_image, base_string);
	return processor.morph_image(meme_string);
}

/**
 * Output the image based on the input argument specifications
 *
 * @param resultingImage The image to output
 * @param force_display Whether or not the image should be forcefully displayed
 * @param output_location The file that should be written to, if at all
 */
static void output_image(
	cimg_library::CImg<unsigned char> resultingImage, bool force_display, std::optional<std::string> output_location) {
	if (output_location.has_value()) {
		resultingImage.save(output_location->c_str());
	}

	// Do not display unless forced, or if there is no file to output to
	if (force_display || !output_location.has_value()) {
		resultingImage.display();
	}
}

int main(int argc, char *argv[]) {
	bool force_display = false;
	std::optional<std::string> output_location;
	std::string in_file;
	std::string base_string;
	std::string meme_string;

	cxxopts::Options options("bill-bill-bill", "Morph an image into a substr-ed repeated variant.");
	// clang-format off
	options.add_options()
		("d,display", "Display the image, even when an output is specified.", cxxopts::value(force_display))
		("o,output", "The output filename for the image. Enabling disables display if --display is not specified", cxxopts::value<std::optional<std::string>>(output_location), "filename")
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

	if (in_file.empty() || base_string.empty() || meme_string.empty()) {
		std::cerr << options.help() << std::endl;
		return 1;
	}

	std::optional<cimg_library::CImg<unsigned char>> image;
	try {
		image = cimg_library::CImg<unsigned char>(in_file.c_str());
	} catch (const cimg_library::CImgIOException &e) {
		std::cerr << "Failed to open image: " << e.what() << std::endl << std::endl;
		return 2;
	}

	auto morphed_image = morph_image(*image, base_string, meme_string);

	try {
		output_image(morphed_image, force_display, output_location);
		return 0;
	} catch (const cimg_library::CImgIOException &e) {
		std::cerr << "Failed to write image: " << e.what() << std::endl;
		return 3;
	}
}
