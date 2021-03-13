#include "src/ImageProcessor.h"
#include <iostream>

#include <CImg.h>

int main(int argc, char *argv[]) {
	if (argc != 4) {
		std::cerr << "Usage: ./bill-bill-bill infile base_string meme_string" << std::endl;
		return 1;
	}

	const std::string_view infile(argv[1]);
	const std::string_view base_string(argv[2]);
	const std::string_view meme_string(argv[3]);
	cimg_library::CImg<unsigned char> image(infile.data());
	ImageProcessor<unsigned char> processor(image, base_string.data());
	auto res = processor.morph_image(meme_string.data());

	res.display();
}
