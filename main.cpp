#include <CImg.h>
// Workaround for https://github.com/jarro2783/cxxopts/issues/303
// clang-format off
#include <version>
#include <cxxopts.hpp>

// clang-format on
#include <iostream>

#include "src/ImageProcessor.hpp"
#include "src/cli/StatusCodeException.hpp"

/**
 * Open the given image.
 *
 * This is primarily in its own method just for consistency when using StatusCodeExceptions. It is not _really_ needed.
 *
 * @param filename The image to open
 * @return cimg_library::CImg<unsigned char> The opened image
 */
static cimg_library::CImg<unsigned char> open_image(const std::string &filename) try {
	return cimg_library::CImg<unsigned char>(filename.c_str());
} catch (const cimg_library::CImgIOException &e) {
	throw StatusCodeException(2, std::string("Failed to open image: ") + e.what());
}

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
	cimg_library::CImg<T> input_image, const std::string &base_string, const std::string &meme_string) try {
	ImageProcessor<unsigned char> processor(input_image, base_string);
	return processor.morph_image(meme_string);
} catch (const std::exception &e) {
	throw StatusCodeException(3, std::string("Failed to process image: ") + e.what());
}

/**
 * Output the image based on the input argument specifications
 *
 * @param resultingImage The image to output
 * @param force_display Whether or not the image should be forcefully displayed
 * @param output_location The file that should be written to, if at all
 */
static void output_image(
	cimg_library::CImg<unsigned char> resultingImage,
	bool force_display,
	std::optional<std::string> output_location) try {
	if (output_location.has_value()) {
		resultingImage.save(output_location->c_str());
	}

	// Do not display unless forced, or if there is no file to output to
	if (force_display || !output_location.has_value()) {
		resultingImage.display();
	}
} catch (const cimg_library::CImgIOException &e) {
	throw StatusCodeException(4, std::string("Failed to output image: ") + e.what());
}

int main(int argc, char *argv[]) try {
	bool force_display = false;
	bool show_help = false;
	std::optional<std::string> output_location;
	std::string in_file;
	std::string base_string;
	std::string meme_string;

	// Disable Cimg error messages; we print them ourselves
	cimg_library::cimg::exception_mode(0);
	cxxopts::Options options("bill-bill-bill", "Morph an image into a substr-ed repeated variant.");
	// clang-format off
	options.add_options()
		("d,display",
		 "Display the image, even when an output is specified.",
		 cxxopts::value(force_display))
		("o,output",
		 "The output filename for the image. Enabling disables display if --display is not specified.",
		 cxxopts::value<std::optional<std::string>>(output_location), "filename")
		("h",
		 "Display help message",
		 cxxopts::value(show_help))
		("in_file",
		 "The input image",
		 cxxopts::value<std::string>(in_file))
		("base_string",
		 "The string to morph from",
		 cxxopts::value<std::string>(base_string))
		("meme_string",
		 "The string to morph to",
		 cxxopts::value<std::string>(meme_string))
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

	if (show_help || in_file.empty() || base_string.empty() || meme_string.empty()) {
		std::cerr << options.help() << std::endl;
		return 1;
	}

	auto image = open_image(in_file);
	auto morphed_image = morph_image(image, base_string, meme_string);
	output_image(morphed_image, force_display, output_location);

	return 0;
} catch (const StatusCodeException &e) {
	std::cerr << e.what() << std::endl;
	return e.get_status_code();
}
